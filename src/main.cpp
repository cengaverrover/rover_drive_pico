/**
 * @file main.cpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Starting point of program.
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "rcl/allocator.h"
#include <pico/stdlib.h>
#include <hardware/watchdog.h>

extern "C" {

#include <FreeRTOS.h>
#include <task.h>

#include "pico_usb_transports.h"
#include <rmw_microros/rmw_microros.h>
#include "freertos_allocators.h"
}

#include "pinout.hpp"
#include "freertos.hpp"
#include <cstdio>

int main() {
    stdio_init_all();
    sleep_ms(2000);
    stdio_filter_driver(&stdio_uart);

    rcl_allocator_t freertosAllocator = rcutils_get_zero_initialized_allocator();
    freertosAllocator.allocate = __freertos_allocate;
    freertosAllocator.deallocate = __freertos_deallocate;
    freertosAllocator.reallocate = __freertos_reallocate;
    freertosAllocator.zero_allocate = __freertos_zero_allocate;

    if (!rcutils_set_default_allocator(&freertosAllocator)) {
        printf("Error on default allocators (line %d)\n", __LINE__);
    }

    // Set the custom transport functions that MicroROS is going to use.
    rmw_uros_set_custom_transport(true, NULL, pico_usb_transport_open, pico_usb_transport_close,
        pico_usb_transport_write, pico_usb_transport_read);

    // Setup the onboard led.
    gpio_init(pinout::led);
    gpio_set_dir(pinout::led, GPIO_OUT);

    // Setup all the FreeRTOS queues that will transfer data across tasks in a
    // thread safe manner.
    freertos::createMsgQueues();

    // Create and start the main task off the program.
    freertos::createMicroRosTask();
    freertos::createMotorTasks();

    vTaskStartScheduler();

    // Code will never reach here.
    while (true) {}
    return 0;
}