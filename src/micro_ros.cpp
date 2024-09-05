extern "C" {

#include "FreeRTOS.h"
#include "task.h"

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

#include "micro_ros.hpp"

static rcl_publisher_t publisher_left{};
static rcl_publisher_t publisher_right{};

static rover_drive_interfaces__msg__MotorFeedback msg_left{
    .dutycycle = 0.0f, .encoder_rpm = 0, .current = 0};
static rover_drive_interfaces__msg__MotorFeedback msg_right{
    .dutycycle = 0.0f, .encoder_rpm = 0, .current = 0};

static rover_drive_interfaces__msg__MotorDrive msg_drive = {.target_rpm = 0, .rotation_rads = 0.0f};

namespace timers {

void timer_callback1(rcl_timer_t *timer, int64_t last_call_time) {
  msg_left.dutycycle = msg_drive.target_rpm / 200.0f;
  msg_left.encoder_rpm = msg_drive.target_rpm;
  msg_left.current = msg_left.dutycycle * 30.0f / 100.0f ;
  rcl_ret_t ret = rcl_publish(&publisher_left, &msg_left, NULL);
}

} // namespace timers

namespace callbacks {

void subscriber_callback(const void *msgin) {
  if (msgin == NULL) {
    return;
  }
  auto msg = static_cast<const rover_drive_interfaces__msg__MotorDrive *>(msgin);

  msg_drive.target_rpm = msg->target_rpm;
  msg_drive.rotation_rads = msg->rotation_rads;
}
} // namespace callbacks

namespace tasks {

TaskHandle_t micro_ros_handle = nullptr;
void micro_ros(void *args) {

  rcl_allocator_t allocator = rcl_get_default_allocator();

  rclc_support_t support{};
  rclc_support_init(&support, 0, NULL, &allocator);

  rcl_node_t node = rcl_get_zero_initialized_node();
  rclc_node_init_default(&node, "pico_node", "", &support);

  publisher_left = rcl_get_zero_initialized_publisher();
  rclc_publisher_init_default(&publisher_left, &node,
                              ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorFeedback),
                              "pico_publisher");

  rcl_subscription_t subscriber = rcl_get_zero_initialized_subscription();
  rclc_subscription_init_default(
      &subscriber, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive),
      "pico_drive");

  rcl_timer_t timer1 = rcl_get_zero_initialized_timer();
  rclc_timer_init_default(&timer1, &support, RCL_MS_TO_NS(50),
                          timers::timer_callback1);

  rclc_parameter_server_t param_server;
  rclc_parameter_options_t param_server_options = {
      .notify_changed_over_dds = true,
      .max_params = 10,
      .allow_undeclared_parameters = true,
      .low_mem_mode = false};

  rclc_parameter_server_init_with_option(&param_server, &node,
                                         &param_server_options);

  rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&executor, &support.context,
                     RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES + 2, &allocator);

  rclc_executor_add_timer(&executor, &timer1);
  rclc_executor_add_subscription(&executor, &subscriber, &msg_drive,
                                 callbacks::subscriber_callback, ON_NEW_DATA);
  rclc_executor_add_parameter_server(&executor, &param_server, NULL);

  rclc_add_parameter(&param_server, "max_motor_rpm", RCLC_PARAMETER_INT);
  rclc_add_parameter(&param_server, "max_motor_dutycycle",
                     RCLC_PARAMETER_DOUBLE);
  rclc_add_parameter_constraint_double(&param_server, "max_motor_dutycycle", 0,
                                       100.0, 0);

  rclc_parameter_set_int(&param_server, "max_motor_rpm", 20000);
  rclc_parameter_set_double(&param_server, "max_motor_dutycycle", 100.0);

  while (true) {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(0));
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  rcl_ret_t ret = 0;
  ret += rcl_timer_fini(&timer1);
  ret += rcl_subscription_fini(&subscriber, &node);
  ret += rclc_executor_fini(&executor);
  vTaskDelete(NULL);
}

} // namespace tasks
