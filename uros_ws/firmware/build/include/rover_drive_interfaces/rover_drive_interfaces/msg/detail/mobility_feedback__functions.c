// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rover_drive_interfaces:msg/MobilityFeedback.idl
// generated code does not contain a copyright notice
#include "rover_drive_interfaces/msg/detail/mobility_feedback__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `motor_feedbacks`
#include "rover_drive_interfaces/msg/detail/motor_feedback__functions.h"

bool
rover_drive_interfaces__msg__MobilityFeedback__init(rover_drive_interfaces__msg__MobilityFeedback * msg)
{
  if (!msg) {
    return false;
  }
  // motor_feedbacks
  for (size_t i = 0; i < 4; ++i) {
    if (!rover_drive_interfaces__msg__MotorFeedback__init(&msg->motor_feedbacks[i])) {
      rover_drive_interfaces__msg__MobilityFeedback__fini(msg);
      return false;
    }
  }
  return true;
}

void
rover_drive_interfaces__msg__MobilityFeedback__fini(rover_drive_interfaces__msg__MobilityFeedback * msg)
{
  if (!msg) {
    return;
  }
  // motor_feedbacks
  for (size_t i = 0; i < 4; ++i) {
    rover_drive_interfaces__msg__MotorFeedback__fini(&msg->motor_feedbacks[i]);
  }
}

bool
rover_drive_interfaces__msg__MobilityFeedback__are_equal(const rover_drive_interfaces__msg__MobilityFeedback * lhs, const rover_drive_interfaces__msg__MobilityFeedback * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // motor_feedbacks
  for (size_t i = 0; i < 4; ++i) {
    if (!rover_drive_interfaces__msg__MotorFeedback__are_equal(
        &(lhs->motor_feedbacks[i]), &(rhs->motor_feedbacks[i])))
    {
      return false;
    }
  }
  return true;
}

bool
rover_drive_interfaces__msg__MobilityFeedback__copy(
  const rover_drive_interfaces__msg__MobilityFeedback * input,
  rover_drive_interfaces__msg__MobilityFeedback * output)
{
  if (!input || !output) {
    return false;
  }
  // motor_feedbacks
  for (size_t i = 0; i < 4; ++i) {
    if (!rover_drive_interfaces__msg__MotorFeedback__copy(
        &(input->motor_feedbacks[i]), &(output->motor_feedbacks[i])))
    {
      return false;
    }
  }
  return true;
}

rover_drive_interfaces__msg__MobilityFeedback *
rover_drive_interfaces__msg__MobilityFeedback__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rover_drive_interfaces__msg__MobilityFeedback * msg = (rover_drive_interfaces__msg__MobilityFeedback *)allocator.allocate(sizeof(rover_drive_interfaces__msg__MobilityFeedback), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rover_drive_interfaces__msg__MobilityFeedback));
  bool success = rover_drive_interfaces__msg__MobilityFeedback__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rover_drive_interfaces__msg__MobilityFeedback__destroy(rover_drive_interfaces__msg__MobilityFeedback * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rover_drive_interfaces__msg__MobilityFeedback__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rover_drive_interfaces__msg__MobilityFeedback__Sequence__init(rover_drive_interfaces__msg__MobilityFeedback__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rover_drive_interfaces__msg__MobilityFeedback * data = NULL;

  if (size) {
    data = (rover_drive_interfaces__msg__MobilityFeedback *)allocator.zero_allocate(size, sizeof(rover_drive_interfaces__msg__MobilityFeedback), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rover_drive_interfaces__msg__MobilityFeedback__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rover_drive_interfaces__msg__MobilityFeedback__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
rover_drive_interfaces__msg__MobilityFeedback__Sequence__fini(rover_drive_interfaces__msg__MobilityFeedback__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      rover_drive_interfaces__msg__MobilityFeedback__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

rover_drive_interfaces__msg__MobilityFeedback__Sequence *
rover_drive_interfaces__msg__MobilityFeedback__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rover_drive_interfaces__msg__MobilityFeedback__Sequence * array = (rover_drive_interfaces__msg__MobilityFeedback__Sequence *)allocator.allocate(sizeof(rover_drive_interfaces__msg__MobilityFeedback__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rover_drive_interfaces__msg__MobilityFeedback__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rover_drive_interfaces__msg__MobilityFeedback__Sequence__destroy(rover_drive_interfaces__msg__MobilityFeedback__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rover_drive_interfaces__msg__MobilityFeedback__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rover_drive_interfaces__msg__MobilityFeedback__Sequence__are_equal(const rover_drive_interfaces__msg__MobilityFeedback__Sequence * lhs, const rover_drive_interfaces__msg__MobilityFeedback__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rover_drive_interfaces__msg__MobilityFeedback__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rover_drive_interfaces__msg__MobilityFeedback__Sequence__copy(
  const rover_drive_interfaces__msg__MobilityFeedback__Sequence * input,
  rover_drive_interfaces__msg__MobilityFeedback__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rover_drive_interfaces__msg__MobilityFeedback);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rover_drive_interfaces__msg__MobilityFeedback * data =
      (rover_drive_interfaces__msg__MobilityFeedback *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rover_drive_interfaces__msg__MobilityFeedback__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rover_drive_interfaces__msg__MobilityFeedback__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rover_drive_interfaces__msg__MobilityFeedback__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
