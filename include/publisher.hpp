#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <etl/string_view.h>
#include <rcl/publisher.h>
#include <rcl/timer.h>
#include <rover_drive_interfaces/msg/motor_feedback.h>

namespace ros {

inline etl::array<rcl_publisher_t, 4> motorFeedbackPublishers{};
inline etl::array<rover_drive_interfaces__msg__MotorFeedback, 4>
    motorFeedbackMsgs{};

extern "C" void publisherTimerCallback(rcl_timer_t *timer,
                                       int64_t last_call_time);

rcl_ret_t createMotorFeedbackPublishers(rcl_node_t *node);

} // namespace ros

#endif // PUBLISHER_HPP