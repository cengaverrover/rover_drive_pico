#include "subscriber.hpp"
#include <rclc/subscription.h>

#include <FreeRTOS.h>
#include <queue.h>

namespace ros {

void driveSubscriberCallback(const void *msgin, void *context) {
  QueueHandle_t driveQueue = static_cast<QueueHandle_t>(context);
  if (msgin != NULL) {
    auto msg =
        static_cast<const rover_drive_interfaces__msg__MotorDrive *>(msgin);
    xQueueOverwrite(driveQueue, msg);
  }
}

Subscriber::Subscriber(rcl_node_t *node, etl::string_view name,
                       const rosidl_message_type_support_t *type)
    : subscriber_(rcl_get_zero_initialized_subscription()) {
  rclc_subscription_init_default(&subscriber_, node, type, name.data());
}

rcl_ret_t
Subscriber::addToExecutor(rclc_executor_t *executor, void *msg,
                          rclc_subscription_callback_with_context_t callback,
                          void *context,
                          rclc_executor_handle_invocation_t event) {
  return rclc_executor_add_subscription_with_context(
      executor, &subscriber_, msg, callback, context, event);
}

} // namespace ros