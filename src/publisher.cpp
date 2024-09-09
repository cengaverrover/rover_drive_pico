#include "publisher.hpp"

#include <FreeRTOS.h>
#include <queue.h>

#include <BTS7960.hpp>
#include <encoder_substep.hpp>
#include <rover_drive_interfaces/msg/motor_drive.h>

#include "parameters.hpp"
#include "pinout.hpp"
#include "queues.hpp"

namespace ros {

rcl_ret_t initPublishers(rcl_node_t *node) {
  constexpr etl::array<etl::string_view, 4> publisherNames{
      "pico_publisher_0", "pico_publisher_1", "pico_publisher_2",
      "pico_publisher_3"};
  auto msgType =
      ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorFeedback);
  rcl_ret_t ret = 0;
  for (int i = 0; i < publisherNames.size(); i++) {
    motorFeedbackPublishers[i] = rcl_get_zero_initialized_publisher();
    ret += rclc_publisher_init_default(&motorFeedbackPublishers[i], node,
                                       msgType, publisherNames[i].data());
  }
  return ret;
}

void createPublisherTasks(const uint32_t uxStackDepth, uint32_t uxPriority,
                          uint32_t uxCoreAffinityMask) {
  void *indexes[4]{(void *)0, (void *)1, (void *)2, (void *)3};
  constexpr etl::array<etl::string_view, 4> taskNames{
      "publisher_task_0", "publisher_task_1", "publisher_task_2",
      "publisher_task_3"};
  for (int i = 0; i < motorFeedbackPublishers.size(); i++) {
    xTaskCreateAffinitySet(publisherTask, taskNames[i].data(), uxStackDepth,
                           indexes[i], uxPriority, uxCoreAffinityMask, nullptr);
  }
}

void publisherTimerCallback(rcl_timer_t *timer, int64_t last_call_time) {
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
  for (int i = 0; i < motorFeedbackPublishers.size(); i++) {
    if (xQueueReceive(publisherQueues[i], &feedbackMsgBuffer, 0) == pdTRUE) {
      ret += rcl_publish(&motorFeedbackPublishers[i], &feedbackMsgBuffer, NULL);
    }
  }
}

void publisherTask(void *arg) {
  const size_t i = (size_t)(arg);
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
} // namespace ros