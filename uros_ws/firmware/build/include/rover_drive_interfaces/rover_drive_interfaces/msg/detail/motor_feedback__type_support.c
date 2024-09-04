// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rover_drive_interfaces:msg/MotorFeedback.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rover_drive_interfaces/msg/detail/motor_feedback__rosidl_typesupport_introspection_c.h"
#include "rover_drive_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rover_drive_interfaces/msg/detail/motor_feedback__functions.h"
#include "rover_drive_interfaces/msg/detail/motor_feedback__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rover_drive_interfaces__msg__MotorFeedback__init(message_memory);
}

void rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_fini_function(void * message_memory)
{
  rover_drive_interfaces__msg__MotorFeedback__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_member_array[3] = {
  {
    "dutycycle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rover_drive_interfaces__msg__MotorFeedback, dutycycle),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "encoder_rpm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rover_drive_interfaces__msg__MotorFeedback, encoder_rpm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rover_drive_interfaces__msg__MotorFeedback, current),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_members = {
  "rover_drive_interfaces__msg",  // message namespace
  "MotorFeedback",  // message name
  3,  // number of fields
  sizeof(rover_drive_interfaces__msg__MotorFeedback),
  rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_member_array,  // message members
  rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_init_function,  // function to initialize message memory (memory has to be allocated)
  rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_type_support_handle = {
  0,
  &rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rover_drive_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rover_drive_interfaces, msg, MotorFeedback)() {
  if (!rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_type_support_handle.typesupport_identifier) {
    rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rover_drive_interfaces__msg__MotorFeedback__rosidl_typesupport_introspection_c__MotorFeedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
