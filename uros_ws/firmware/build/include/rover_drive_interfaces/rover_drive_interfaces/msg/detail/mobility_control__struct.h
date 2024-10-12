// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rover_drive_interfaces:msg/MobilityControl.idl
// generated code does not contain a copyright notice

#ifndef ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOBILITY_CONTROL__STRUCT_H_
#define ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOBILITY_CONTROL__STRUCT_H_

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
  rover_drive_interfaces__msg__MobilityControl__FRONT_LEFT = 0
};

/// Constant 'BACK_LEFT'.
enum
{
  rover_drive_interfaces__msg__MobilityControl__BACK_LEFT = 1
};

/// Constant 'FRONT_RIGHT'.
enum
{
  rover_drive_interfaces__msg__MobilityControl__FRONT_RIGHT = 2
};

/// Constant 'BACK_RIGHT'.
enum
{
  rover_drive_interfaces__msg__MobilityControl__BACK_RIGHT = 3
};

/// Struct defined in msg/MobilityControl in the package rover_drive_interfaces.
typedef struct rover_drive_interfaces__msg__MobilityControl
{
  int32_t target_rpm[4];
} rover_drive_interfaces__msg__MobilityControl;

// Struct for a sequence of rover_drive_interfaces__msg__MobilityControl.
typedef struct rover_drive_interfaces__msg__MobilityControl__Sequence
{
  rover_drive_interfaces__msg__MobilityControl * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rover_drive_interfaces__msg__MobilityControl__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOBILITY_CONTROL__STRUCT_H_
