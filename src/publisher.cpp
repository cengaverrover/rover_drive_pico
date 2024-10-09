/**
 * @file publisher.cpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Source file of MicroROS publisher related functions and callbacks.
 * @version 0.1
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "publisher.hpp"

#include <rclc/publisher.h>
#include "parameters.hpp"
#include "queues.hpp"
#include "messages.hpp"
namespace ros {

Publisher::Publisher(
    rcl_node_t* node, etl::string_view name, const rosidl_message_type_support_t* typeSupport) {
    if (node != nullptr) {
        rclc_publisher_init_default(&publisher_, node, typeSupport, name.data());
    }
}

rcl_ret_t Publisher::init(
    rcl_node_t* node, etl::string_view name, const rosidl_message_type_support_t* typeSupport) {
    return rclc_publisher_init_default(&publisher_, node, typeSupport, name.data());
}


rcl_ret_t Publisher::publish(const void* msg, rmw_publisher_allocation_t* allocation) {
    return rcl_publish(&publisher_, msg, allocation);
}

static Publisher motorFeedbackPublisher{};

rcl_ret_t createMobilityFeedbackPublisher(rcl_node_t* node) {
    constexpr auto publisherName{ "motor_feedbacks" };
    const auto msgType =
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_mobility_interfaces, msg, MobilityFeedback);
    rcl_ret_t ret = motorFeedbackPublisher.init(node, publisherName, msgType);
    return ret;
}

void publisherTimerCallback(rcl_timer_t* timer, int64_t last_call_time) {
    // If timer period was changed through parameters, set the new period to the
    // timer.
    int64_t lastPeriod{};
    rcl_ret_t ret = rcl_timer_get_period(timer, &lastPeriod);
    if (lastPeriod != RCL_MS_TO_NS(parameter::motorFeedbackPeriodMs)) {
        ret += rcl_timer_exchange_period(
            timer, RCL_MS_TO_NS(parameter::motorFeedbackPeriodMs), &lastPeriod);
    }
    // Receive all the feedback messeages from the queues and publish them.
    MobilityFeedback feedbackMsgBuffer{};
    for (int i = 0; i < freertos::queue::motorFeedbackQueues.size(); i++) {
        if (xQueueReceive(freertos::queue::motorFeedbackQueues[i],
                &(feedbackMsgBuffer.motor_feedbacks[i]),
                0) == pdTRUE) {}
    }
    ret += motorFeedbackPublisher.publish(&feedbackMsgBuffer);
}
} // namespace ros