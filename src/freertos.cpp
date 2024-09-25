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

static etl::array<StaticQueue_t, 4> driveQueueBuffers{};
static constexpr auto driveQueueItemSize = sizeof(rover_drive_interfaces__msg__MotorDrive);
static constexpr int driveQueueItemCount = 1;
static etl::array<etl::array<uint8_t, driveQueueItemSize * driveQueueItemCount>, 4>
    driveQueueStorage{};

static etl::array<StaticQueue_t, 4> publisherQueueBuffers{};
static constexpr auto publisherQueueItemSize = sizeof(rover_drive_interfaces__msg__MotorFeedback);
static constexpr int publisherQueueItemCount = 1;
static etl::array<etl::array<uint8_t, publisherQueueItemSize * publisherQueueItemCount>, 4>
    publisherQueueStorage{};

void createMsgQueues() {
    // Create all the queues.
    for (int i = 0; i < queue::publisherQueues.size(); i++) {
        queue::publisherQueues[i] = xQueueCreateStatic(publisherQueueItemCount,
            publisherQueueItemSize, publisherQueueStorage[i].data(), &publisherQueueBuffers[i]);
        
        queue::driveQueues[i] = xQueueCreateStatic(driveQueueItemCount, driveQueueItemSize,
            driveQueueStorage[i].data(), &driveQueueBuffers[i]);
    }
}

static StaticTask_t microRosTaskBuffer{};
static constexpr uint32_t microRosTaskStackSize = 4000;
static StackType_t microRosTaskStack[microRosTaskStackSize]{};
static constexpr uint32_t microRosTaskPriority = configMAX_PRIORITIES - 2;
static constexpr uint32_t microRosTaskCoreAffinity = 0x01;
static constexpr etl::string_view microRosTaskName{ "micro_ros_task" };

void createMicroRosTask() {
    task::microRosTaskHandle = xTaskCreateStaticAffinitySet(task::microRosTask,
        microRosTaskName.data(), microRosTaskStackSize, nullptr, microRosTaskPriority,
        microRosTaskStack, &microRosTaskBuffer, microRosTaskCoreAffinity);
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
   implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
   used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
       function then they must be declared static - otherwise they will be allocated on
       the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
       state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
       Note that, as the array is necessarily of type StackType_t,
       configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
   application must provide an implementation of vApplicationGetTimerTaskMemory()
   to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
       function then they must be declared static - otherwise they will be allocated on
       the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
       task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
       Note that, as the array is necessarily of type StackType_t,
      configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


} // namespace freertos
