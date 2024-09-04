// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rover_drive_interfaces:msg/Drive.idl
// generated code does not contain a copyright notice

#ifndef ROVER_DRIVE_INTERFACES__MSG__DETAIL__DRIVE__STRUCT_H_
#define ROVER_DRIVE_INTERFACES__MSG__DETAIL__DRIVE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/Drive in the package rover_drive_interfaces.
typedef struct rover_drive_interfaces__msg__Drive
{
  int32_t left_rpm;
  int32_t right_rpm;
} rover_drive_interfaces__msg__Drive;

// Struct for a sequence of rover_drive_interfaces__msg__Drive.
typedef struct rover_drive_interfaces__msg__Drive__Sequence
{
  rover_drive_interfaces__msg__Drive * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rover_drive_interfaces__msg__Drive__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROVER_DRIVE_INTERFACES__MSG__DETAIL__DRIVE__STRUCT_H_
