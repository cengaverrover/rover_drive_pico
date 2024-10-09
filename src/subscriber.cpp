/**
 * @file subscriber.cpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Source file of MicroRos subscriber related functions and callbacks.
 * @version 0.1
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <rclc/subscription.h>

#include <FreeRTOS.h>
#include <queue.h>

#include "subscriber.hpp"
#include "messages.hpp"
#include "queues.hpp"
namespace ros {

void mobilityControlSubscriberCallback(const void* msgin, void* context) {
    if (msgin != nullptr) {
        auto msg = static_cast<const MobilityControl*>(msgin);
        for (int i = 0; i < freertos::queue::mobilityControlQueues.size(); i++) {
            xQueueOverwrite(freertos::queue::mobilityControlQueues[i], &(msg->target_rpm[i]));
        }
    }
}

Subscriber::Subscriber(
    rcl_node_t* node, etl::string_view name, const rosidl_message_type_support_t* type)
    : subscriber_(rcl_get_zero_initialized_subscription()) {
    rclc_subscription_init_default(&subscriber_, node, type, name.data());
}

rcl_ret_t Subscriber::addToExecutor(rclc_executor_t* executor, void* msg,
    rclc_subscription_callback_with_context_t callback, void* context,
    rclc_executor_handle_invocation_t event) {
    return rclc_executor_add_subscription_with_context(
        executor, &subscriber_, msg, callback, context, event);
}


} // namespace ros