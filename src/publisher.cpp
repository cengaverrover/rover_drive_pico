#include "publisher.hpp"

#include <FreeRTOS.h>
#include <queue.h>

#include <BTS7960.hpp>
#include <encoder_substep.hpp>
#include <rover_drive_interfaces/msg/motor_drive.h>

#include "parameters.hpp"
#include "queues.hpp"

namespace ros {

rcl_ret_t createMotorFeedbackPublishers(rcl_node_t *node) {
  constexpr etl::array<etl::string_view, 4> publisherNames{
      "motor_feedback_front_left", "motor_feedback_back_left",
      "motor_feedback_front_right", "motor_feedback_back_right"};
  const auto msgType =
      ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorFeedback);
  rcl_ret_t ret = 0;
  for (int i = 0; i < publisherNames.size(); i++) {
    motorFeedbackPublishers[i] = rcl_get_zero_initialized_publisher();
    ret += rclc_publisher_init_default(&motorFeedbackPublishers[i], node,
                                       msgType, publisherNames[i].data());
  }
  return ret;
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
    if (xQueueReceive(freertos::publisherQueues[i], &feedbackMsgBuffer, 0) ==
        pdTRUE) {
      ret += rcl_publish(&motorFeedbackPublishers[i], &feedbackMsgBuffer, NULL);
    }
  }
}
} // namespace ros