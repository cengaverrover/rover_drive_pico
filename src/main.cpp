/*
 * main.cpp
 *
 *  Created on: 9 Feb 2023
 *      Author: jondurrant
 */

#include <pico/stdlib.h>
#include <cstdio>

extern "C" {

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include "pico_uart_transports.h"
#include <rmw_microros/rmw_microros.h>
}

#include "node.hpp"
#include "pinout.hpp"

int main() {
  rmw_uros_set_custom_transport(
      true, NULL, pico_serial_transport_open, pico_serial_transport_close,
      pico_serial_transport_write, pico_serial_transport_read);

  gpio_init(pinout::led);
  gpio_set_dir(pinout::led, GPIO_OUT);

  // Wait for agent successful ping for 2 minutes.
  const int timeout_ms = 1000;
  const uint8_t attempts = 120;
  while (rmw_uros_ping_agent(timeout_ms, attempts)) {
    tight_loop_contents();
  }

  gpio_put(pinout::led, true);

  ros::Node node{};
  node.spin();

  return 0;
}