#include "tasks.hpp"
#include "freertos.hpp"
#include "parameters.hpp"
#include "publisher.hpp"
#include "queues.hpp"
#include "rmw_microros/ping.h"
#include "subscriber.hpp"

#include "motor.hpp"
#include "encoder_substep.hpp"
#include "pinout.hpp"
#include "messages.hpp"

#include <hardware/watchdog.h>

namespace freertos {

namespace task {

template <uint i> void motorTask(void* arg) {
    // Create the motor and encoder classes.
    Motor motor(pinout::motorPwmL[i], pinout::motorPwmR[i]);
    encoder::EncoderSubstep encoder(
        pinout::encoderPio, pinout::encoderPioSm[i], pinout::encoderA[i]);
    sleep_ms(10);

    // Create the ros messeages.
    int32_t targetRpmReceived{};
    MotorFeedback feedbackMsgSent{};

    // Integral and previous error to use for PID.
    float integral = 0.0f;
    float errorRpmPrev = 0.0f;

    absolute_time_t lastMsgReceivedTime = get_absolute_time();
    // Get the starting time of the task.
    TickType_t startTick{ xTaskGetTickCount() };
    while (true) {
        // If there is a new drive messeage available, receive it and update the
        // lastMsgReceivedTime variable. Otherwise, don't wait for new data and
        // continue.
        if (xQueueReceive(freertos::queue::mobilityControlQueues[i], &targetRpmReceived, 0) ==
            pdTRUE) {
            lastMsgReceivedTime = get_absolute_time();
        }

        // Read the current RPM from encoders.
        feedbackMsgSent.rpm = encoder.getRpm();
        // Calculate the time since the last messeage was received.
        auto timeDiffMsg = absolute_time_diff_us(lastMsgReceivedTime, get_absolute_time()) / 1000;

        // If the motor current is over the maximum value or the timeout has been
        // reached, set the target RPM to 0 to turn the motors off.
        if (feedbackMsgSent.current >= ros::parameter::maxMotorCurrent ||
            timeDiffMsg >= ros::parameter::motorTimeoutMs) {
            targetRpmReceived = 0;
        }
        // If PID mode is open calculate the PID values.
        if (ros::parameter::motorPidMode) {
            const int32_t errorRpm = targetRpmReceived - feedbackMsgSent.rpm;

            const float proportional = errorRpm * ros::parameter::motorPidKd;
            integral += errorRpm * ros::parameter::motorPidKi;
            integral = etl::clamp(
                integral, -ros::parameter::maxMotorDutyCycle, ros::parameter::maxMotorDutyCycle);
            const float derivative = ros::parameter::motorPidKd * (errorRpm - errorRpmPrev) *
                                     ros::parameter::motorPidLoopPeriodMs / 1000.0f;

            feedbackMsgSent.dutycycle = proportional + integral + derivative;
        } else {
            // If PID mode is off, assume there is a linear relationship between the
            // motor dutycyle and RPM and use open loop control.
            feedbackMsgSent.dutycycle = targetRpmReceived *
                                        ros::parameter::maxMotorDutyCycleUpperConstraint /
                                        ros::parameter::maxMotorRpm;
        }
        // Clamp the dutycycle within the boundaries.
        feedbackMsgSent.dutycycle = etl::clamp(feedbackMsgSent.dutycycle,
            -ros::parameter::maxMotorDutyCycle,
            ros::parameter::maxMotorDutyCycle);

        // Imaginary current value to test current limiting property since we do
        // not have a current sensor yet.
        // feedbackMsgSent.current =
        //     (feedbackMsgSent.dutycycle) * (ros::parameter::maxMotorCurrent - 5.0f) / 100.0f;

        // Set the dutycyle of the motors.
        motor.setSpeedPercent(feedbackMsgSent.dutycycle);
        // Send the feedback messeage to the queue
        xQueueOverwrite(freertos::queue::motorFeedbackQueues[i], &feedbackMsgSent);
        // Delay the task by the amount set in motor_pid_loop_period_ms parameter.
        xTaskDelayUntil(&startTick, pdMS_TO_TICKS(ros::parameter::motorPidLoopPeriodMs));
    }
}

#define RCCHECK(x)                                                                                 \
    do {                                                                                           \
        if (x == RCL_RET_OK) {                                                                     \
            watchdog_update();                                                                     \
        } else {                                                                                   \
            while (true) {                                                                         \
                tight_loop_contents();                                                             \
            }                                                                                      \
        }                                                                                          \
    } while (0)

void microRosTask(void* arg) {
    // MicroRos boiler plate.
    rcl_allocator_t allocator = rcl_get_default_allocator();

    rclc_support_t support{};
    while (rclc_support_init(&support, 0, nullptr, &allocator) != RCL_RET_OK) {
        rmw_uros_ping_agent(200, 1);
    }
    sleep_ms(100);
    watchdog_enable(1500, true);
    // Set the onboard pin to high to indicate succesfull connection.
    gpio_put(pinout::led, true);

    sleep_ms(10);

    rcl_node_t node = rcl_get_zero_initialized_node();
    RCCHECK(rclc_node_init_default(&node, "pico_node", "mobility", &support));

    // Create the MicroROS motor feedback publishers
    RCCHECK(ros::createMobilityFeedbackPublisher(&node));
    sleep_ms(10);

    ros::Subscriber mobilityControlSubscriber(&node, "motor_control",
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_mobility_interfaces, msg, MobilityControl));
    sleep_ms(10);

    // Create MicroROS timer that will publish the feedback messeages
    // We use a MicroROS task for publishing instead of publishing directly in
    // FreeRTOS tasks as I had problem with random crashes when publishing from
    // different cores at the same time.
    rcl_timer_t publisherTimer = rcl_get_zero_initialized_timer();
    RCCHECK(rclc_timer_init_default(&publisherTimer, &support,
        RCL_MS_TO_NS(ros::parameter::motorFeedbackPeriodMs), ros::publisherTimerCallback));
    sleep_ms(10);

    // Create the executor responsible for all the subscribers and the timer.
    rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
    RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
    // Add the subscribers and the timer to the executor.
    MobilityControl mobilityControlMsg{};
    RCCHECK(mobilityControlSubscriber.addToExecutor(&executor, &mobilityControlMsg,
        ros::mobilityControlSubscriberCallback, nullptr, ON_NEW_DATA));
    RCCHECK(rclc_executor_add_timer(&executor, &publisherTimer));
    sleep_ms(10);

    // Create the parameter server and a seperate executor for the parameter
    // server.
    // We need the second executor since we have more handles than the maximum
    // executor handle.
    constexpr int paramCount = 11;
    ros::parameter::Server paramServer(&node, true, paramCount, false, false);
    rclc_executor_t paramServerExecutor = rclc_executor_get_zero_initialized_executor();
    RCCHECK(rclc_executor_init(&paramServerExecutor, &support.context,
        RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES, &allocator));
    RCCHECK(paramServer.addToExecutor(&paramServerExecutor));
    sleep_ms(10);

    // Create the FreeRTOS motor tasks that will run on both cores.
    // Set the watchdog timer that will reset microcontroller if it is not updated
    // within set time period.
    while (true) {
        // Update the watchdog
        watchdog_update();
        // Spin the executors to check if there are new subscriber messeages or
        // parameter server requests.
        const auto executorResult = rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1));
        const auto paramExecutorResult =
            rclc_executor_spin_some(&paramServerExecutor, RCL_MS_TO_NS(1));
        // Delay the tasks to free the core for other tasks.
        // TODO add parameter to control executor period.
        vTaskDelay(pdMS_TO_TICKS(ros::parameter::executorSpinPeriodMs));
    }
}
} // namespace task


void createMotorTasks() {
    constexpr etl::array taskFunctions{ task::motorTask<0>, task::motorTask<1>, task::motorTask<2>,
        task::motorTask<3> };
    constexpr uint32_t motorTaskStackSize = 500;
    constexpr uint32_t motorTaskPriority = configMAX_PRIORITIES - 3;
    constexpr uint32_t motorTaskCoreAffinity = 0x03;
    constexpr etl::array taskNames{ "motor_task_0", "motor_task_1", "motor_task_2",
        "motor_task_3" };
    for (int i = 0; i < 4; i++) {
        xTaskCreateAffinitySet(taskFunctions[i], taskNames[i], motorTaskStackSize, nullptr,
            motorTaskPriority, motorTaskCoreAffinity, &task::motorTaskHandles[i]);
    }
}


} // namespace freertos