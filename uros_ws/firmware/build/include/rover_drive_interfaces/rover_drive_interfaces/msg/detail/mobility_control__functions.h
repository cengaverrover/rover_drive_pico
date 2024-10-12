// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from rover_drive_interfaces:msg/MobilityControl.idl
// generated code does not contain a copyright notice

#ifndef ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOBILITY_CONTROL__FUNCTIONS_H_
#define ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOBILITY_CONTROL__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "rover_drive_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "rover_drive_interfaces/msg/detail/mobility_control__struct.h"

/// Initialize msg/MobilityControl message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * rover_drive_interfaces__msg__MobilityControl
 * )) before or use
 * rover_drive_interfaces__msg__MobilityControl__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
bool
rover_drive_interfaces__msg__MobilityControl__init(rover_drive_interfaces__msg__MobilityControl * msg);

/// Finalize msg/MobilityControl message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
void
rover_drive_interfaces__msg__MobilityControl__fini(rover_drive_interfaces__msg__MobilityControl * msg);

/// Create msg/MobilityControl message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * rover_drive_interfaces__msg__MobilityControl__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
rover_drive_interfaces__msg__MobilityControl *
rover_drive_interfaces__msg__MobilityControl__create();

/// Destroy msg/MobilityControl message.
/**
 * It calls
 * rover_drive_interfaces__msg__MobilityControl__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
void
rover_drive_interfaces__msg__MobilityControl__destroy(rover_drive_interfaces__msg__MobilityControl * msg);

/// Check for msg/MobilityControl message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
bool
rover_drive_interfaces__msg__MobilityControl__are_equal(const rover_drive_interfaces__msg__MobilityControl * lhs, const rover_drive_interfaces__msg__MobilityControl * rhs);

/// Copy a msg/MobilityControl message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
bool
rover_drive_interfaces__msg__MobilityControl__copy(
  const rover_drive_interfaces__msg__MobilityControl * input,
  rover_drive_interfaces__msg__MobilityControl * output);

/// Initialize array of msg/MobilityControl messages.
/**
 * It allocates the memory for the number of elements and calls
 * rover_drive_interfaces__msg__MobilityControl__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
bool
rover_drive_interfaces__msg__MobilityControl__Sequence__init(rover_drive_interfaces__msg__MobilityControl__Sequence * array, size_t size);

/// Finalize array of msg/MobilityControl messages.
/**
 * It calls
 * rover_drive_interfaces__msg__MobilityControl__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
void
rover_drive_interfaces__msg__MobilityControl__Sequence__fini(rover_drive_interfaces__msg__MobilityControl__Sequence * array);

/// Create array of msg/MobilityControl messages.
/**
 * It allocates the memory for the array and calls
 * rover_drive_interfaces__msg__MobilityControl__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
rover_drive_interfaces__msg__MobilityControl__Sequence *
rover_drive_interfaces__msg__MobilityControl__Sequence__create(size_t size);

/// Destroy array of msg/MobilityControl messages.
/**
 * It calls
 * rover_drive_interfaces__msg__MobilityControl__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
void
rover_drive_interfaces__msg__MobilityControl__Sequence__destroy(rover_drive_interfaces__msg__MobilityControl__Sequence * array);

/// Check for msg/MobilityControl message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
bool
rover_drive_interfaces__msg__MobilityControl__Sequence__are_equal(const rover_drive_interfaces__msg__MobilityControl__Sequence * lhs, const rover_drive_interfaces__msg__MobilityControl__Sequence * rhs);

/// Copy an array of msg/MobilityControl messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_rover_drive_interfaces
bool
rover_drive_interfaces__msg__MobilityControl__Sequence__copy(
  const rover_drive_interfaces__msg__MobilityControl__Sequence * input,
  rover_drive_interfaces__msg__MobilityControl__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // ROVER_DRIVE_INTERFACES__MSG__DETAIL__MOBILITY_CONTROL__FUNCTIONS_H_
