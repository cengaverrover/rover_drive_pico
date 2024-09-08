#ifndef NODE_HPP
#define NODE_HPP

extern "C" {

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcl/error_handling.h>
#include <rclc/executor.h>
#include <rclc_parameter/rclc_parameter.h>
#include <rmw_microros/rmw_microros.h>
#include <rover_drive_interfaces/msg/motor_drive.h>
#include <rover_drive_interfaces/msg/motor_feedback.h>
#include <std_msgs/msg/int32.h>
}

namespace ros {

class Node {
public:
  Node();

  void spin();

  void spinSome(uint32_t timeout);

private:
  rcl_allocator_t allocator_{rcl_get_default_allocator()};
  rclc_support_t support_{};
  rcl_node_t node_{rcl_get_zero_initialized_node()};
  rcl_subscription_t driveSubscriber_{rcl_get_zero_initialized_subscription()};
  rcl_timer_t publisherTimer_{rcl_get_zero_initialized_timer()};
  rclc_parameter_server_t paramServer_{};
  rclc_parameter_options_t paramServerOptions_{};
  rclc_executor_t executor_{rclc_executor_get_zero_initialized_executor()};
};
} // namespace ros

#endif // NODE_HPP