#include "node.hpp"

#include <FreeRTOS.h>
#include <task.h>


namespace ros {

rcl_publisher_t publishers[4]{};
rover_drive_interfaces__msg__MotorFeedback msgs[4]{};

rover_drive_interfaces__msg__MotorDrive msgDrive = {.target_rpm = 0,
                                                    .rotation_rads = 0.0f};

extern "C" {
// void publisherTimerCallback(rcl_timer_t *timer, int64_t last_call_time);
// void driveSubscriberCallback(const void *msgin);
// void executorTask(void *arg);

static void publisherTimerCallback(rcl_timer_t *timer, int64_t last_call_time) {

  for (int i = 0; i < 4; i++) {
    msgs[i].dutycycle = msgDrive.target_rpm / 200.0f;
    msgs[i].encoder_rpm = msgDrive.target_rpm;
    msgs[i].current = msgs[i].dutycycle * 30.0f / 100.0f;
    rcl_ret_t ret = rcl_publish(&publishers[i], &msgs[i], NULL);
  }
}
static void driveSubscriberCallback(const void *msgin) {
  if (msgin == NULL) {
    return;
  }
  auto msg =
      static_cast<const rover_drive_interfaces__msg__MotorDrive *>(msgin);

  msgDrive.target_rpm = msg->target_rpm;
  msgDrive.rotation_rads = msg->rotation_rads;
}

static void executorTask(void *arg) {
  auto node = static_cast<Node *>(arg);
  while (true) {
    vTaskSuspendAll();
    node->spinSome(RCL_MS_TO_NS(1));
    xTaskResumeAll();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
}

Node::Node() : allocator_(rcl_get_default_allocator()) {
  rclc_support_init(&support_, 0, NULL, &allocator_);

  rclc_node_init_default(&node_, "pico_node", "", &support_);

  const char *names[4]{"pico_publisher_0", "pico_publisher_1",
                       "pico_publisher_2", "pico_publisher_3"};
  for (int i = 0; i < 4; i++) {
    publishers[i] = rcl_get_zero_initialized_publisher();
    rclc_publisher_init_default(
        &publishers[i], &node_,
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorFeedback),
        names[i]);
  }

  driveSubscriber_ = rcl_get_zero_initialized_subscription();
  rclc_subscription_init_default(
      &driveSubscriber_, &node_,
      ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive),
      "pico_drive");

  publisherTimer_ = rcl_get_zero_initialized_timer();
  rclc_timer_init_default(&publisherTimer_, &support_, RCL_MS_TO_NS(50),
                          publisherTimerCallback);

   paramServerOptions_ = {
      .notify_changed_over_dds = true,
      .max_params = 10,
      .allow_undeclared_parameters = true,
      .low_mem_mode = false};

  rclc_parameter_server_init_with_option(&paramServer_, &node_,
                                         &paramServerOptions_);

  rclc_executor_init(&executor_, &support_.context,
                     RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES + 2, &allocator_);

  rclc_executor_add_timer(&executor_, &publisherTimer_);
  rclc_executor_add_subscription(&executor_, &driveSubscriber_, &msgDrive,
                                 driveSubscriberCallback, ON_NEW_DATA);
  rclc_executor_add_parameter_server(&executor_, &paramServer_, NULL);

  rclc_add_parameter(&paramServer_, "max_motor_rpm", RCLC_PARAMETER_INT);
  rclc_add_parameter(&paramServer_, "max_motor_dutycycle",
                     RCLC_PARAMETER_DOUBLE);
  rclc_add_parameter_constraint_double(&paramServer_, "max_motor_dutycycle", 0,
                                       100.0, 0);

  rclc_parameter_set_int(&paramServer_, "max_motor_rpm", 20000);
  rclc_parameter_set_double(&paramServer_, "max_motor_dutycycle", 100.0);
}

void Node::spin() {
  xTaskCreateAffinitySet(executorTask, "executor_task", 3000, this, 24, 0x01,
                         nullptr);
  vTaskStartScheduler();
}

void Node::spinSome(uint32_t timeout) {
  rclc_executor_spin_some(&executor_, RCL_MS_TO_NS(timeout));
}

} // namespace ros