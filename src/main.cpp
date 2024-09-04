/*
 * main.cpp
 *
 *  Created on: 9 Feb 2023
 *      Author: jondurrant
 */


#include <stdio.h>
#include "pico/stdlib.h"

extern "C" {

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <rmw_microros/rmw_microros.h>
#include "pico_uart_transports.h"
}

#include "pinout.hpp"
#include "micro_ros.hpp"

int main() {
    rmw_uros_set_custom_transport(
        true,
        NULL,
        pico_serial_transport_open,
        pico_serial_transport_close,
        pico_serial_transport_write,
        pico_serial_transport_read
    );

    gpio_init(pinout::led);
    gpio_set_dir(pinout::led, GPIO_OUT);

    // Wait for agent successful ping for 2 minutes.
    const int timeout_ms = 1000;
    const uint8_t attempts = 120;
    while (rmw_uros_ping_agent(timeout_ms, attempts)) {
      tight_loop_contents();
    }

    gpio_put(pinout::led, true);

    xTaskCreate(tasks::micro_ros, "micro_ros_task", 3000, nullptr, 24,
                &tasks::micro_ros_handle);

    vTaskStartScheduler();
    
    return 0;
}