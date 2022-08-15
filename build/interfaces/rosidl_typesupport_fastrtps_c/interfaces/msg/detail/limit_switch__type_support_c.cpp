// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from interfaces:msg/LimitSwitch.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/limit_switch__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/msg/detail/limit_switch__struct.h"
#include "interfaces/msg/detail/limit_switch__functions.h"
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


using _LimitSwitch__ros_msg_type = interfaces__msg__LimitSwitch;

static bool _LimitSwitch__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _LimitSwitch__ros_msg_type * ros_message = static_cast<const _LimitSwitch__ros_msg_type *>(untyped_ros_message);
  // Field name: b1_min
  {
    cdr << ros_message->b1_min;
  }

  // Field name: b1_max
  {
    cdr << ros_message->b1_max;
  }

  // Field name: b2_min
  {
    cdr << ros_message->b2_min;
  }

  // Field name: b2_max
  {
    cdr << ros_message->b2_max;
  }

  // Field name: b3_min
  {
    cdr << ros_message->b3_min;
  }

  // Field name: b3_max
  {
    cdr << ros_message->b3_max;
  }

  return true;
}

static bool _LimitSwitch__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _LimitSwitch__ros_msg_type * ros_message = static_cast<_LimitSwitch__ros_msg_type *>(untyped_ros_message);
  // Field name: b1_min
  {
    cdr >> ros_message->b1_min;
  }

  // Field name: b1_max
  {
    cdr >> ros_message->b1_max;
  }

  // Field name: b2_min
  {
    cdr >> ros_message->b2_min;
  }

  // Field name: b2_max
  {
    cdr >> ros_message->b2_max;
  }

  // Field name: b3_min
  {
    cdr >> ros_message->b3_min;
  }

  // Field name: b3_max
  {
    cdr >> ros_message->b3_max;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__msg__LimitSwitch(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _LimitSwitch__ros_msg_type * ros_message = static_cast<const _LimitSwitch__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name b1_min
  {
    size_t item_size = sizeof(ros_message->b1_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b1_max
  {
    size_t item_size = sizeof(ros_message->b1_max);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b2_min
  {
    size_t item_size = sizeof(ros_message->b2_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b2_max
  {
    size_t item_size = sizeof(ros_message->b2_max);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b3_min
  {
    size_t item_size = sizeof(ros_message->b3_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b3_max
  {
    size_t item_size = sizeof(ros_message->b3_max);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _LimitSwitch__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__msg__LimitSwitch(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__msg__LimitSwitch(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: b1_min
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b1_max
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b2_min
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b2_max
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b3_min
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b3_max
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _LimitSwitch__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__msg__LimitSwitch(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_LimitSwitch = {
  "interfaces::msg",
  "LimitSwitch",
  _LimitSwitch__cdr_serialize,
  _LimitSwitch__cdr_deserialize,
  _LimitSwitch__get_serialized_size,
  _LimitSwitch__max_serialized_size
};

static rosidl_message_type_support_t _LimitSwitch__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_LimitSwitch,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, msg, LimitSwitch)() {
  return &_LimitSwitch__type_support;
}

#if defined(__cplusplus)
}
#endif
