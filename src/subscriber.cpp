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

#include "subscriber.hpp"
#include <rclc/subscription.h>

#include <FreeRTOS.h>
#include <queue.h>

namespace ros {

void driveSubscriberCallback(const void* msgin, void* context) {
    if (msgin != nullptr || context != nullptr) {
        QueueHandle_t driveQueue = static_cast<QueueHandle_t>(context);
        auto msg = static_cast<const rover_drive_interfaces__msg__MotorDrive*>(msgin);
        xQueueOverwrite(driveQueue, msg);
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


etl::array<Subscriber, 4> createSubscribers(rcl_node_t* node) {
    constexpr etl::array<etl::string_view, 4> subscriberNames{ "motor_drive_front_left",
        "motor_drive_back_left", "motor_drive_front_right", "motor_drive_back_right" };
    const auto subscriberMsgType = ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive);
    
    return { ros::Subscriber(node, subscriberNames[0], subscriberMsgType),
        ros::Subscriber(node, subscriberNames[1], subscriberMsgType),
        ros::Subscriber(node, subscriberNames[2], subscriberMsgType),
        ros::Subscriber(node, subscriberNames[3], subscriberMsgType) };
}

} // namespace ros