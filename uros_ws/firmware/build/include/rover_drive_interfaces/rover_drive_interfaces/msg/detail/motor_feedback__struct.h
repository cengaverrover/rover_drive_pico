// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rover_drive_interfaces:msg/MotorFeedback.idl
// generated code does not contain a copyright notice

#ifndef ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOTOR_FEEDBACK__STRUCT_H_
#define ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOTOR_FEEDBACK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/MotorFeedback in the package rover_drive_interfaces.
typedef struct rover_drive_interfaces__msg__MotorFeedback
{
  float dutycycle;
  int32_t encoder_rpm;
  float current;
} rover_drive_interfaces__msg__MotorFeedback;

// Struct for a sequence of rover_drive_interfaces__msg__MotorFeedback.
typedef struct rover_drive_interfaces__msg__MotorFeedback__Sequence
{
  rover_drive_interfaces__msg__MotorFeedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rover_drive_interfaces__msg__MotorFeedback__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOTOR_FEEDBACK__STRUCT_H_
