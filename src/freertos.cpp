/**
 * @file freertos.cpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Source file of Freertos related functions and tasks.
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "queues.hpp"
#include <rover_drive_interfaces/msg/motor_drive.h>
#include <rover_drive_interfaces/msg/motor_feedback.h>

#include "BTS7960.hpp"
#include "encoder_substep.hpp"
#include "parameters.hpp"
#include "pinout.hpp"
#include "publisher.hpp"
#include "subscriber.hpp"
#include "tasks.hpp"

namespace freertos {
void createMsgQueues() {
  for (int i = 0; i < publisherQueues.size(); i++) {
    publisherQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorFeedback));
    driveQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorDrive));
  }
}

void createMotorTasks(uint32_t uxStackDepth, uint32_t uxPriority,
                      uint32_t uxCoreAffinityMask) {
  constexpr etl::array<etl::string_view, 4> taskNames{
      "publisher_task_0", "publisher_task_1", "publisher_task_2",
      "publisher_task_3"};
  void *indexes[4]{(void *)0, (void *)1, (void *)2, (void *)3};
  for (int i = 0; i < pinout::motorPwmL.size(); i++) {
    xTaskCreateAffinitySet(motorTask, taskNames[i].data(), uxStackDepth,
                           indexes[i], uxPriority, uxCoreAffinityMask,
                           &motorTaskHandles[i]);
  }
}

void motorTask(void *arg) {
  const size_t i = (size_t)(arg);
  // Create the motor and encoder classes.
  motor::BTS7960 motor(pinout::motorPwmL[i], pinout::motorPwmR[i]);
  encoder::EncoderSubstep encoder(pinout::encoderPio, pinout::encoderPioSm[i],
                                  pinout::encoderA[i]);

  // Create the ros messeages.
  rover_drive_interfaces__msg__MotorDrive driveMsgReceived{};
  rover_drive_interfaces__msg__MotorFeedback feedbackMsgSent{};

  float integral = 0.0f;
  float errorRpmPrev = 0.0f;

  absolute_time_t lastMsgReceivedTime = get_absolute_time();
  // Get the starting time of the task.
  TickType_t startTick{xTaskGetTickCount()};
  while (true) {
    // If there is a new drive messeage available, receive it.
    if (xQueueReceive(freertos::driveQueues[i], &driveMsgReceived, 0) ==
        pdTRUE) {
      feedbackMsgSent.dutycycle = driveMsgReceived.target_rpm / 200.0f;
      // feedbackMsgSent.encoder_rpm = driveMsgReceived.target_rpm;
      feedbackMsgSent.current = feedbackMsgSent.dutycycle * 30.0f / 100.0f;
      lastMsgReceivedTime = get_absolute_time();
    }

    feedbackMsgSent.encoder_rpm = encoder.getRpm();

    auto timeDiffMsg =
        absolute_time_diff_us(lastMsgReceivedTime, get_absolute_time()) / 1000;

    if (feedbackMsgSent.current >= ros::parameter::maxMotorCurrent ||
        timeDiffMsg >= ros::parameter::motorTimeoutMs) {
      feedbackMsgSent.dutycycle = 0;
    } else {
      if (ros::parameter::motorPidMode) {
        const int32_t errorRpm =
            driveMsgReceived.target_rpm - feedbackMsgSent.encoder_rpm;

        const float proportional = errorRpm * ros::parameter::motorPidKd;
        integral += errorRpm * ros::parameter::motorPidKi;
        integral = etl::clamp(integral, -ros::parameter::maxMotorDutyCycle,
                              ros::parameter::maxMotorDutyCycle);
        const float derivative = ros::parameter::motorPidKd *
                                 (errorRpm - errorRpmPrev) *
                                 ros::parameter::motorPidLoopPeriodMs / 1000.0f;

        feedbackMsgSent.dutycycle = proportional + integral + derivative;
      } else {
        feedbackMsgSent.dutycycle =
            driveMsgReceived.target_rpm *
            ros::parameter::maxMotorDutyCycleUpperConstraint /
            ros::parameter::maxMotorRpm;
      }
      feedbackMsgSent.dutycycle = etl::clamp(feedbackMsgSent.dutycycle,
                                             -ros::parameter::maxMotorDutyCycle,
                                             ros::parameter::maxMotorDutyCycle);
    }

    motor.setSpeed(feedbackMsgSent.dutycycle);

    xQueueOverwrite(freertos::publisherQueues[i], &feedbackMsgSent);
    xTaskDelayUntil(&startTick,
                    pdMS_TO_TICKS(ros::parameter::motorPidLoopPeriodMs));
  }
}

void microRosTask(void *arg) {
  vTaskSuspendAll();

  rcl_allocator_t allocator = rcl_get_default_allocator();

  rclc_support_t support{};
  rclc_support_init(&support, 0, NULL, &allocator);

  rcl_node_t node = rcl_get_zero_initialized_node();
  rclc_node_init_default(&node, "pico_node", "drive", &support);

  ros::createMotorFeedbackPublishers(&node);

  constexpr etl::array<etl::string_view, 4> subscriberNames{
      "pico_subscriber_0", "pico_subscriber_1", "pico_subscriber_2",
      "pico_subscriber_3"};
  auto subscriberMsgType =
      ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive);
  etl::array<ros::Subscriber, 4> driveSubscribers{
      ros::Subscriber(&node, subscriberNames[0], subscriberMsgType),
      ros::Subscriber(&node, subscriberNames[1], subscriberMsgType),
      ros::Subscriber(&node, subscriberNames[2], subscriberMsgType),
      ros::Subscriber(&node, subscriberNames[3], subscriberMsgType)};

  rcl_timer_t publisherTimer = rcl_get_zero_initialized_timer();
  publisherTimer = rcl_get_zero_initialized_timer();
  rclc_timer_init_default(&publisherTimer, &support, RCL_MS_TO_NS(100),
                          ros::publisherTimerCallback);

  rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&executor, &support.context, 5, &allocator);

  for (int i = 0; i < driveSubscribers.size(); i++) {
    driveSubscribers[i].addToExecutor(&executor, &ros::driveMsgs[i],
                                      ros::driveSubscriberCallback,
                                      driveQueues[i], ON_NEW_DATA);
  }
  rclc_executor_add_timer(&executor, &publisherTimer);

  ros::parameter::Server paramServer(&node, true, 10, true, false);
  rclc_executor_t paramServerExecutor =
      rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&paramServerExecutor, &support.context,
                     RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES, &allocator);
  paramServer.addToExecutor(&paramServerExecutor);
  paramServer.initParameters();

  freertos::createMotorTasks(500, 3, 0x03);

  xTaskResumeAll();
  while (true) {
    // vTaskSuspendAll();
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1));
    rclc_executor_spin_some(&paramServerExecutor, RCL_MS_TO_NS(1));
    // xTaskResumeAll();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
} // namespace freertos
