// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rover_drive_interfaces:msg/MobilityFeedback.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rover_drive_interfaces/msg/detail/mobility_feedback__rosidl_typesupport_introspection_c.h"
#include "rover_drive_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rover_drive_interfaces/msg/detail/mobility_feedback__functions.h"
#include "rover_drive_interfaces/msg/detail/mobility_feedback__struct.h"


// Include directives for member types
// Member `motor_feedbacks`
#include "rover_drive_interfaces/msg/motor_feedback.h"
// Member `motor_feedbacks`
#include "rover_drive_interfaces/msg/detail/motor_feedback__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rover_drive_interfaces__msg__MobilityFeedback__init(message_memory);
}

void rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_fini_function(void * message_memory)
{
  rover_drive_interfaces__msg__MobilityFeedback__fini(message_memory);
}

size_t rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__size_function__MobilityFeedback__motor_feedbacks(
  const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__get_const_function__MobilityFeedback__motor_feedbacks(
  const void * untyped_member, size_t index)
{
  const rover_drive_interfaces__msg__MotorFeedback * member =
    (const rover_drive_interfaces__msg__MotorFeedback *)(untyped_member);
  return &member[index];
}

void * rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__get_function__MobilityFeedback__motor_feedbacks(
  void * untyped_member, size_t index)
{
  rover_drive_interfaces__msg__MotorFeedback * member =
    (rover_drive_interfaces__msg__MotorFeedback *)(untyped_member);
  return &member[index];
}

void rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__fetch_function__MobilityFeedback__motor_feedbacks(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const rover_drive_interfaces__msg__MotorFeedback * item =
    ((const rover_drive_interfaces__msg__MotorFeedback *)
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__get_const_function__MobilityFeedback__motor_feedbacks(untyped_member, index));
  rover_drive_interfaces__msg__MotorFeedback * value =
    (rover_drive_interfaces__msg__MotorFeedback *)(untyped_value);
  *value = *item;
}

void rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__assign_function__MobilityFeedback__motor_feedbacks(
  void * untyped_member, size_t index, const void * untyped_value)
{
  rover_drive_interfaces__msg__MotorFeedback * item =
    ((rover_drive_interfaces__msg__MotorFeedback *)
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__get_function__MobilityFeedback__motor_feedbacks(untyped_member, index));
  const rover_drive_interfaces__msg__MotorFeedback * value =
    (const rover_drive_interfaces__msg__MotorFeedback *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_member_array[1] = {
  {
    "motor_feedbacks",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(rover_drive_interfaces__msg__MobilityFeedback, motor_feedbacks),  // bytes offset in struct
    NULL,  // default value
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__size_function__MobilityFeedback__motor_feedbacks,  // size() function pointer
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__get_const_function__MobilityFeedback__motor_feedbacks,  // get_const(index) function pointer
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__get_function__MobilityFeedback__motor_feedbacks,  // get(index) function pointer
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__fetch_function__MobilityFeedback__motor_feedbacks,  // fetch(index, &value) function pointer
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__assign_function__MobilityFeedback__motor_feedbacks,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_members = {
  "rover_drive_interfaces__msg",  // message namespace
  "MobilityFeedback",  // message name
  1,  // number of fields
  sizeof(rover_drive_interfaces__msg__MobilityFeedback),
  rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_member_array,  // message members
  rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_init_function,  // function to initialize message memory (memory has to be allocated)
  rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_type_support_handle = {
  0,
  &rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rover_drive_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rover_drive_interfaces, msg, MobilityFeedback)() {
  rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rover_drive_interfaces, msg, MotorFeedback)();
  if (!rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_type_support_handle.typesupport_identifier) {
    rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rover_drive_interfaces__msg__MobilityFeedback__rosidl_typesupport_introspection_c__MobilityFeedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
