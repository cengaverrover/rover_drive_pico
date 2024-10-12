// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rover_mobility_interfaces:msg/MobilityFeedback.idl
// generated code does not contain a copyright notice

#ifndef ROVER_MOBILITY_INTERFACES__MSG__DETAIL__MOBILITY_FEEDBACK__STRUCT_H_
#define ROVER_MOBILITY_INTERFACES__MSG__DETAIL__MOBILITY_FEEDBACK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'FRONT_LEFT'.
enum
{
  rover_mobility_interfaces__msg__MobilityFeedback__FRONT_LEFT = 0
};

/// Constant 'BACK_LEFT'.
enum
{
  rover_mobility_interfaces__msg__MobilityFeedback__BACK_LEFT = 1
};

/// Constant 'FRONT_RIGHT'.
enum
{
  rover_mobility_interfaces__msg__MobilityFeedback__FRONT_RIGHT = 2
};

/// Constant 'BACK_RIGHT'.
enum
{
  rover_mobility_interfaces__msg__MobilityFeedback__BACK_RIGHT = 3
};

// Include directives for member types
// Member 'motor_feedbacks'
#include "rover_mobility_interfaces/msg/detail/motor_feedback__struct.h"

/// Struct defined in msg/MobilityFeedback in the package rover_mobility_interfaces.
typedef struct rover_mobility_interfaces__msg__MobilityFeedback
{
  rover_mobility_interfaces__msg__MotorFeedback motor_feedbacks[4];
} rover_mobility_interfaces__msg__MobilityFeedback;

// Struct for a sequence of rover_mobility_interfaces__msg__MobilityFeedback.
typedef struct rover_mobility_interfaces__msg__MobilityFeedback__Sequence
{
  rover_mobility_interfaces__msg__MobilityFeedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rover_mobility_interfaces__msg__MobilityFeedback__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROVER_MOBILITY_INTERFACES__MSG__DETAIL__MOBILITY_FEEDBACK__STRUCT_H_
