#ifndef MICRO_ROS_HPP
#define MICRO_ROS_HPP

#include <FreeRTOS.h>
#include "task.h"

// Existing code...

namespace tasks {

extern TaskHandle_t micro_ros_handle; 
void micro_ros(void *args);

}

#endif // MICRO_ROS_HPP