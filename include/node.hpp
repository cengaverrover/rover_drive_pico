#ifndef NODE_HPP
#define NODE_HPP

extern "C" {

#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <rclc_parameter/rclc_parameter.h>
#include <rmw_microros/rmw_microros.h>
#include <rover_drive_interfaces/msg/motor_drive.h>
#include <rover_drive_interfaces/msg/motor_feedback.h>
#include <std_msgs/msg/int32.h>

}

#include <etl/string_view.h>

namespace ros {

class Node {
public:
  Node() = default;

  void spin();

private:
  
};
} // namespace ros

#endif // NODE_HPP