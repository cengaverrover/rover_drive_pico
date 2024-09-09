#ifndef TASKS_HPP
#define TASKS_HPP

#include "FreeRTOSConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <pico/types.h>

#include <etl/array.h>

namespace freertos {

void createMotorTasks(uint32_t uxStackDepth = 500,
                      uint32_t uxPriority = configMAX_PRIORITIES / 2,
                      uint32_t uxCoreAffinityMask = 0x03);

inline TaskHandle_t microRosTaskHandle= nullptr;
extern "C" void microRosTask(void *arg);

inline etl::array<TaskHandle_t, 4> motorTaskHandles{};
extern "C" void motorTask(void *arg);

} // namespace freertos

#endif // TASKS_HPP