// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from interfaces:msg/MotorVelsWArm.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/motor_vels_w_arm__rosidl_typesupport_fastrtps_cpp.hpp"
#include "interfaces/msg/detail/motor_vels_w_arm__struct.hpp"

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
  const interfaces::msg::MotorVelsWArm & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: vel_m1
  cdr << ros_message.vel_m1;
  // Member: vel_m2
  cdr << ros_message.vel_m2;
  // Member: vel_m3
  cdr << ros_message.vel_m3;
  // Member: vel_m4
  cdr << ros_message.vel_m4;
  // Member: vel_b1
  cdr << ros_message.vel_b1;
  // Member: vel_b2
  cdr << ros_message.vel_b2;
  // Member: vel_b3
  cdr << ros_message.vel_b3;
  // Member: vel_g1
  cdr << ros_message.vel_g1;
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  interfaces::msg::MotorVelsWArm & ros_message)
{
  // Member: vel_m1
  cdr >> ros_message.vel_m1;

  // Member: vel_m2
  cdr >> ros_message.vel_m2;

  // Member: vel_m3
  cdr >> ros_message.vel_m3;

  // Member: vel_m4
  cdr >> ros_message.vel_m4;

  // Member: vel_b1
  cdr >> ros_message.vel_b1;

  // Member: vel_b2
  cdr >> ros_message.vel_b2;

  // Member: vel_b3
  cdr >> ros_message.vel_b3;

  // Member: vel_g1
  cdr >> ros_message.vel_g1;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
get_serialized_size(
  const interfaces::msg::MotorVelsWArm & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: vel_m1
  {
    size_t item_size = sizeof(ros_message.vel_m1);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_m2
  {
    size_t item_size = sizeof(ros_message.vel_m2);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_m3
  {
    size_t item_size = sizeof(ros_message.vel_m3);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_m4
  {
    size_t item_size = sizeof(ros_message.vel_m4);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_b1
  {
    size_t item_size = sizeof(ros_message.vel_b1);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_b2
  {
    size_t item_size = sizeof(ros_message.vel_b2);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_b3
  {
    size_t item_size = sizeof(ros_message.vel_b3);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_g1
  {
    size_t item_size = sizeof(ros_message.vel_g1);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
max_serialized_size_MotorVelsWArm(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: vel_m1
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_m2
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_m3
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_m4
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_b1
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_b2
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_b3
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_g1
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static bool _MotorVelsWArm__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const interfaces::msg::MotorVelsWArm *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _MotorVelsWArm__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<interfaces::msg::MotorVelsWArm *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _MotorVelsWArm__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const interfaces::msg::MotorVelsWArm *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _MotorVelsWArm__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_MotorVelsWArm(full_bounded, 0);
}

static message_type_support_callbacks_t _MotorVelsWArm__callbacks = {
  "interfaces::msg",
  "MotorVelsWArm",
  _MotorVelsWArm__cdr_serialize,
  _MotorVelsWArm__cdr_deserialize,
  _MotorVelsWArm__get_serialized_size,
  _MotorVelsWArm__max_serialized_size
};

static rosidl_message_type_support_t _MotorVelsWArm__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_MotorVelsWArm__callbacks,
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
get_message_type_support_handle<interfaces::msg::MotorVelsWArm>()
{
  return &interfaces::msg::typesupport_fastrtps_cpp::_MotorVelsWArm__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, interfaces, msg, MotorVelsWArm)() {
  return &interfaces::msg::typesupport_fastrtps_cpp::_MotorVelsWArm__handle;
}

#ifdef __cplusplus
}
#endif
