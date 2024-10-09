#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <rover_mobility_interfaces/msg/mobility_control.h>
#include <rover_mobility_interfaces/msg/mobility_feedback.h>
#include <rover_mobility_interfaces/msg/motor_feedback.h>

#include <etl/string_view.h>

using MobilityControl = rover_mobility_interfaces__msg__MobilityControl;
using MobilityFeedback = rover_mobility_interfaces__msg__MobilityFeedback;
using MotorFeedback = rover_mobility_interfaces__msg__MotorFeedback;

#endif // MESSAGES_HPP