/**
 * @file freertos.cpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Source file of Freertos related functions and tasks.
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "freertos.hpp"
#include "queues.hpp"
#include "tasks.hpp"
#include "messages.hpp"

#include <etl/string_view.h>


namespace freertos {


void createMsgQueues() {
    constexpr auto mobilityControlQueueSize = sizeof(int32_t);
    constexpr int mobilityControlQueueItemCount = 1;

    constexpr auto mobilityFeedbackQueueItemSize = sizeof(MotorFeedback);
    constexpr int mobilityFeedbackQueueItemCount = 1;
    // Create all the queues.
    for (int i = 0; i < queue::motorFeedbackQueues.size(); i++) {
        queue::motorFeedbackQueues[i] =
            xQueueCreate(mobilityFeedbackQueueItemCount, mobilityFeedbackQueueItemSize);

        queue::mobilityControlQueues[i] =
            xQueueCreate(mobilityControlQueueItemCount, mobilityControlQueueSize);
    }
}


void createMicroRosTask() {
    constexpr uint32_t microRosTaskStackSize = 4000;
    constexpr uint32_t microRosTaskPriority = configMAX_PRIORITIES - 2;
    constexpr uint32_t microRosTaskCoreAffinity = 0x01;
    constexpr etl::string_view microRosTaskName{ "micro_ros_task" };

    xTaskCreateAffinitySet(task::microRosTask, microRosTaskName.data(), microRosTaskStackSize,
        nullptr, microRosTaskPriority, microRosTaskCoreAffinity, &task::microRosTaskHandle);
}


} // namespace freertos
