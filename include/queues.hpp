#ifndef QUEUES_HPP
#define QUEUES_HPP

#include <FreeRTOS.h>
#include <queue.h>

#include <etl/string_view.h>

namespace freertos {

inline etl::array<QueueHandle_t, 4> publisherQueues{};
inline etl::array<QueueHandle_t, 4> driveQueues{};

void initQueues();
} // namespace freertos

#endif // QUEUES_HPP