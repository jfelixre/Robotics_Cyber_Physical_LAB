// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from interfaces:msg/MotorVels.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/motor_vels__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/msg/detail/motor_vels__struct.h"
#include "interfaces/msg/detail/motor_vels__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _MotorVels__ros_msg_type = interfaces__msg__MotorVels;

static bool _MotorVels__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _MotorVels__ros_msg_type * ros_message = static_cast<const _MotorVels__ros_msg_type *>(untyped_ros_message);
  // Field name: vel_m1
  {
    cdr << ros_message->vel_m1;
  }

  // Field name: vel_m2
  {
    cdr << ros_message->vel_m2;
  }

  // Field name: vel_m3
  {
    cdr << ros_message->vel_m3;
  }

  // Field name: vel_m4
  {
    cdr << ros_message->vel_m4;
  }

  return true;
}

static bool _MotorVels__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _MotorVels__ros_msg_type * ros_message = static_cast<_MotorVels__ros_msg_type *>(untyped_ros_message);
  // Field name: vel_m1
  {
    cdr >> ros_message->vel_m1;
  }

  // Field name: vel_m2
  {
    cdr >> ros_message->vel_m2;
  }

  // Field name: vel_m3
  {
    cdr >> ros_message->vel_m3;
  }

  // Field name: vel_m4
  {
    cdr >> ros_message->vel_m4;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__msg__MotorVels(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _MotorVels__ros_msg_type * ros_message = static_cast<const _MotorVels__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name vel_m1
  {
    size_t item_size = sizeof(ros_message->vel_m1);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_m2
  {
    size_t item_size = sizeof(ros_message->vel_m2);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_m3
  {
    size_t item_size = sizeof(ros_message->vel_m3);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_m4
  {
    size_t item_size = sizeof(ros_message->vel_m4);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _MotorVels__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__msg__MotorVels(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__msg__MotorVels(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: vel_m1
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_m2
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_m3
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_m4
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _MotorVels__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__msg__MotorVels(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_MotorVels = {
  "interfaces::msg",
  "MotorVels",
  _MotorVels__cdr_serialize,
  _MotorVels__cdr_deserialize,
  _MotorVels__get_serialized_size,
  _MotorVels__max_serialized_size
};

static rosidl_message_type_support_t _MotorVels__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_MotorVels,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, msg, MotorVels)() {
  return &_MotorVels__type_support;
}

#if defined(__cplusplus)
}
#endif
