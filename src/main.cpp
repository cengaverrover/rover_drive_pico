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

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rclc_parameter/rclc_parameter.h>
#include <std_msgs/msg/int32.h>
#include <rmw_microros/rmw_microros.h>
#include <rover_drive_interfaces/msg/motor_settings.h>
#include <std_msgs/msg/int32.h>

#include "pico_uart_transports.h"
}

const uint LED_PIN = 25;

rcl_publisher_t publisher1;
std_msgs__msg__Int32 msg1;

rcl_publisher_t publisher2;
rover_drive_interfaces__msg__MotorSettings msg2 = { .max_rpm = 20000, .max_dutycycle = 100.0f };


void timer_callback1(rcl_timer_t* timer, int64_t last_call_time) {
    rcl_ret_t ret = rcl_publish(&publisher1, &msg1, NULL);
    msg1.data++;
}

void timer_callback2(rcl_timer_t* timer, int64_t last_call_time) {
    rcl_ret_t ret = rcl_publish(&publisher2, &msg2, NULL);
}

int main() {
    rmw_uros_set_custom_transport(
        true,
        NULL,
        pico_serial_transport_open,
        pico_serial_transport_close,
        pico_serial_transport_write,
        pico_serial_transport_read
    );

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    rcl_timer_t timer1;
    rcl_timer_t timer2;
    rcl_node_t node;
    rcl_allocator_t allocator;
    rclc_support_t support;
    rclc_executor_t executor;

    allocator = rcl_get_default_allocator();

    // Wait for agent successful ping for 2 minutes.
    const int timeout_ms = 1000;
    const uint8_t attempts = 120;

    rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

    if (ret != RCL_RET_OK)
    {
        // Unreachable agent, exiting program.
        return ret;
    }

    rclc_support_init(&support, 0, NULL, &allocator);

    rclc_node_init_default(&node, "pico_node", "", &support);
    rclc_publisher_init_default(
        &publisher1, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "pico_publisher");

    rclc_publisher_init_default(&publisher2, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorSettings), "motor_settings_publisher");

    rclc_timer_init_default(&timer1, &support, RCL_MS_TO_NS(1000),
        timer_callback1);

    rclc_timer_init_default(&timer2, &support, RCL_MS_TO_NS(2000),
                            timer_callback2);

    rclc_parameter_server_t param_server;
    rclc_parameter_options_t param_server_options = {
        .notify_changed_over_dds = true,
        .max_params = 10,
        .allow_undeclared_parameters = true,
        .low_mem_mode = false};

    rclc_parameter_server_init_with_option(&param_server, &node, &param_server_options);

    rclc_executor_init(&executor, &support.context, RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES + 2, &allocator);

    rclc_executor_add_timer(&executor, &timer1);
    rclc_executor_add_timer(&executor, &timer2);
    rclc_executor_add_parameter_server(&executor, &param_server, NULL);

    rclc_add_parameter(&param_server, "max_motor_rpm", RCLC_PARAMETER_INT);
    rclc_add_parameter(&param_server, "max_motor_dutycycle",
                       RCLC_PARAMETER_DOUBLE);
    rclc_add_parameter_constraint_double(&param_server, "max_motor_dutycycle", 0, 100.0, 0);
    
    rclc_parameter_set_int(&param_server, "max_motor_rpm", 20000);
    rclc_parameter_set_double(&param_server, "max_motor_dutycycle", 100.0);
    
    gpio_put(LED_PIN, 1);

    msg1.data = 0;
    while (true)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }
    return 0;
}