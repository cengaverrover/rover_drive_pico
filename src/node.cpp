#include "node.hpp"

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <etl/array.h>

#include "BTS7960.hpp"
#include "encoder_substep.hpp"
#include "parameters.hpp"
#include "pinout.hpp"

namespace ros {

static etl::array<rcl_publisher_t, pinout::motorPwmL.size()> publishers{};
static etl::array<rover_drive_interfaces__msg__MotorFeedback, publishers.size()>
    msgs{};

static etl::array<rover_drive_interfaces__msg__MotorDrive, publishers.size()>
    driveMsgs{};

static etl::array<QueueHandle_t, publishers.size()> publisherQueues{};
static etl::array<QueueHandle_t, publishers.size()> driveQueues{};

template <size_t i> void publisherTask(void *arg) {

  // Create the motor and encoder classes.
  motor::BTS7960 motor(pinout::motorPwmL[i], pinout::motorPwmR[i]);
  encoder::EncoderSubstep encoder(pinout::encoderPio, pinout::encoderPioSm[i],
                                  pinout::encoderA[i]);

  // Create the ros messeages.
  rover_drive_interfaces__msg__MotorDrive driveMsgReceived{};
  rover_drive_interfaces__msg__MotorFeedback feedbackMsgSent{};

  // Get the starting time of the task.
  TickType_t startTick{xTaskGetTickCount()};
  while (true) {
    // If there is a new drive messeage available, receive it.
    if (xQueuePeek(driveQueues[i], &driveMsgReceived, 0) == pdTRUE) {
      feedbackMsgSent.dutycycle = driveMsgReceived.target_rpm / 200.0f;
      // feedbackMsgSent.encoder_rpm = driveMsgReceived.target_rpm;
      feedbackMsgSent.current = feedbackMsgSent.dutycycle * 30.0f / 100.0f;
    }

    if (feedbackMsgSent.current >= parameter::maxMotorCurrent) {
      feedbackMsgSent.dutycycle = 0;
    }

    feedbackMsgSent.encoder_rpm = encoder.getRpm();
    const float targetRpm =
        etl::clamp(driveMsgReceived.target_rpm, static_cast<int32_t>(0),
                   parameter::maxMotorRpm);

    feedbackMsgSent.dutycycle = etl::clamp(
        feedbackMsgSent.dutycycle, parameter::maxMotorDutyCycleLowerConstraint,
        parameter::maxMotorDutyCycle);
    motor.setSpeed(feedbackMsgSent.dutycycle);

    xQueueOverwrite(publisherQueues[i], &feedbackMsgSent);
    xTaskDelayUntil(&startTick, pdMS_TO_TICKS(parameter::motorPidLoopPeriodMs));
  }
}

template <size_t i> static void driveSubscriberCallback(const void *msgin) {
  if (msgin != NULL) {
    auto msg =
        static_cast<const rover_drive_interfaces__msg__MotorDrive *>(msgin);
    xQueueOverwrite(driveQueues[i], msg);
  }
}
extern "C" {

static void publisherTimerCallback(rcl_timer_t *timer, int64_t last_call_time) {
  // If timer period was changed through parameters, set the new period to the
  // timer.
  int64_t lastPeriod{};
  rcl_ret_t ret = rcl_timer_get_period(timer, &lastPeriod);
  if (lastPeriod != RCL_MS_TO_NS(parameter::motorFeedbackPeriodMs)) {
    ret += rcl_timer_exchange_period(
        timer, RCL_MS_TO_NS(parameter::motorFeedbackPeriodMs), &lastPeriod);
  }
  // Receive all the feedback messeages from the queues and publish them.
  rover_drive_interfaces__msg__MotorFeedback feedbackMsgBuffer{};
  for (int i = 0; i < publishers.size(); i++) {
    if (xQueueReceive(publisherQueues[i], &feedbackMsgBuffer, 0) == pdTRUE) {
      ret += rcl_publish(&publishers[i], &feedbackMsgBuffer, NULL);
    }
  }
}

static void microRosTask(void *arg) {
  vTaskSuspendAll();

  rcl_allocator_t allocator = rcl_get_default_allocator();

  rclc_support_t support{};
  rclc_support_init(&support, 0, NULL, &allocator);

  rcl_node_t node = rcl_get_zero_initialized_node();
  rclc_node_init_default(&node, "pico_node", "drive", &support);

  constexpr etl::array<etl::string_view, 4> publisherNames{
      "pico_publisher_0", "pico_publisher_1", "pico_publisher_2",
      "pico_publisher_3"};
  constexpr etl::array<etl::string_view, 4> subscriberNames{
      "pico_subscriber_0", "pico_subscriber_1", "pico_subscriber_2",
      "pico_subscriber_3"};

  etl::array<rcl_subscription_t, 4> driveSubscribers{};
  for (int i = 0; i < 4; i++) {
    publishers[i] = rcl_get_zero_initialized_publisher();
    rclc_publisher_init_default(
        &publishers[i], &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorFeedback),
        publisherNames[i].data());

    driveSubscribers[i] = rcl_get_zero_initialized_subscription();
    rclc_subscription_init_default(
        &driveSubscribers[i], &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive),
        subscriberNames[i].data());

    publisherQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorFeedback));

    driveQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorDrive));
  }

  rcl_timer_t publisherTimer = rcl_get_zero_initialized_timer();
  publisherTimer = rcl_get_zero_initialized_timer();
  rclc_timer_init_default(&publisherTimer, &support, RCL_MS_TO_NS(100),
                          publisherTimerCallback);

  rclc_parameter_options_t paramServerOptions{};
  paramServerOptions = {.notify_changed_over_dds = true,
                        .max_params = 10,
                        .allow_undeclared_parameters = true,
                        .low_mem_mode = false};

  parameter::Server paramServer(&node, &paramServerOptions);

  rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&executor, &support.context, 5, &allocator);

  rclc_executor_add_subscription(&executor, &driveSubscribers[0], &driveMsgs[0],
                                 driveSubscriberCallback<0>, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &driveSubscribers[1], &driveMsgs[1],
                                 driveSubscriberCallback<1>, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &driveSubscribers[2], &driveMsgs[2],
                                 driveSubscriberCallback<2>, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &driveSubscribers[3], &driveMsgs[3],
                                 driveSubscriberCallback<3>, ON_NEW_DATA);
  rclc_executor_add_timer(&executor, &publisherTimer);

  rclc_executor_t paramServerExecutor =
      rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&paramServerExecutor, &support.context, RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES, &allocator);
  paramServer.addToExecutor(&paramServerExecutor);
  paramServer.initParameters();

  xTaskCreateAffinitySet(publisherTask<0>, "publisher_task_0", 500, nullptr, 3, 0x03,
                         nullptr);
  xTaskCreateAffinitySet(publisherTask<1>, "publisher_task_1", 500, nullptr, 3, 0x03,
                         nullptr);
  xTaskCreateAffinitySet(publisherTask<2>, "publisher_task_2", 500, nullptr, 3, 0x03,
                         nullptr);
  xTaskCreateAffinitySet(publisherTask<3>, "publisher_task_3", 500, nullptr, 3, 0x03,
                         nullptr);

  xTaskResumeAll();
  while (true) {
    // vTaskSuspendAll();
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1));
    rclc_executor_spin_some(&paramServerExecutor, RCL_MS_TO_NS(1));
    // xTaskResumeAll();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
}

void Node::spin() {
  xTaskCreateAffinitySet(microRosTask, "executor_task", 4000, nullptr, 4, 0x01,
                         nullptr);
  vTaskStartScheduler();
}

} // namespace ros