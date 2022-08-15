// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from interfaces:msg/LimitSwitch.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/limit_switch__rosidl_typesupport_fastrtps_cpp.hpp"
#include "interfaces/msg/detail/limit_switch__struct.hpp"

#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

namespace interfaces
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
cdr_serialize(
  const interfaces::msg::LimitSwitch & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: b1_min
  cdr << ros_message.b1_min;
  // Member: b1_max
  cdr << ros_message.b1_max;
  // Member: b2_min
  cdr << ros_message.b2_min;
  // Member: b2_max
  cdr << ros_message.b2_max;
  // Member: b3_min
  cdr << ros_message.b3_min;
  // Member: b3_max
  cdr << ros_message.b3_max;
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  interfaces::msg::LimitSwitch & ros_message)
{
  // Member: b1_min
  cdr >> ros_message.b1_min;

  // Member: b1_max
  cdr >> ros_message.b1_max;

  // Member: b2_min
  cdr >> ros_message.b2_min;

  // Member: b2_max
  cdr >> ros_message.b2_max;

  // Member: b3_min
  cdr >> ros_message.b3_min;

  // Member: b3_max
  cdr >> ros_message.b3_max;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
get_serialized_size(
  const interfaces::msg::LimitSwitch & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: b1_min
  {
    size_t item_size = sizeof(ros_message.b1_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: b1_max
  {
    size_t item_size = sizeof(ros_message.b1_max);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: b2_min
  {
    size_t item_size = sizeof(ros_message.b2_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: b2_max
  {
    size_t item_size = sizeof(ros_message.b2_max);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: b3_min
  {
    size_t item_size = sizeof(ros_message.b3_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: b3_max
  {
    size_t item_size = sizeof(ros_message.b3_max);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
max_serialized_size_LimitSwitch(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: b1_min
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: b1_max
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: b2_min
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: b2_max
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: b3_min
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: b3_max
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static bool _LimitSwitch__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const interfaces::msg::LimitSwitch *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _LimitSwitch__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<interfaces::msg::LimitSwitch *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _LimitSwitch__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const interfaces::msg::LimitSwitch *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _LimitSwitch__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_LimitSwitch(full_bounded, 0);
}

static message_type_support_callbacks_t _LimitSwitch__callbacks = {
  "interfaces::msg",
  "LimitSwitch",
  _LimitSwitch__cdr_serialize,
  _LimitSwitch__cdr_deserialize,
  _LimitSwitch__get_serialized_size,
  _LimitSwitch__max_serialized_size
};

static rosidl_message_type_support_t _LimitSwitch__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_LimitSwitch__callbacks,
  get_message_typesupport_handle_function,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace interfaces

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_interfaces
const rosidl_message_type_support_t *
get_message_type_support_handle<interfaces::msg::LimitSwitch>()
{
  return &interfaces::msg::typesupport_fastrtps_cpp::_LimitSwitch__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, interfaces, msg, LimitSwitch)() {
  return &interfaces::msg::typesupport_fastrtps_cpp::_LimitSwitch__handle;
}

#ifdef __cplusplus
}
#endif
