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

#include <rover_drive_interfaces/msg/motor_drive.h>
#include <rover_drive_interfaces/msg/motor_feedback.h>

#include <etl/string_view.h>

namespace freertos {


void createMsgQueues() {
    constexpr auto driveQueueItemSize = sizeof(rover_drive_interfaces__msg__MotorDrive);
    constexpr int driveQueueItemCount = 1;

    constexpr auto publisherQueueItemSize = sizeof(rover_drive_interfaces__msg__MotorFeedback);
    constexpr int publisherQueueItemCount = 1;
    // Create all the queues.
    for (int i = 0; i < queue::publisherQueues.size(); i++) {
        queue::publisherQueues[i] = xQueueCreate(publisherQueueItemCount, publisherQueueItemSize);

        queue::driveQueues[i] = xQueueCreate(driveQueueItemCount, driveQueueItemSize);
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
