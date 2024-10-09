/**
 * @file queues.hpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Header file for FreeRTOS queues used to transfer messeages across
 * tasks.
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef QUEUES_HPP
#define QUEUES_HPP

#include <FreeRTOS.h>
#include <queue.h>

#include <etl/string_view.h>

namespace freertos {

namespace queue {

inline etl::array<QueueHandle_t, 4> motorFeedbackQueues{};
inline etl::array<QueueHandle_t, 4> mobilityControlQueues{};

} // namespace queue
} // namespace freertos

#endif // QUEUES_HPP