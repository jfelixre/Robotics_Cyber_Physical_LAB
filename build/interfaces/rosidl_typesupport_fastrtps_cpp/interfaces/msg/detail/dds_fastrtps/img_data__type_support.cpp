// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from interfaces:msg/ImgData.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/img_data__rosidl_typesupport_fastrtps_cpp.hpp"
#include "interfaces/msg/detail/img_data__struct.hpp"

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
  const interfaces::msg::ImgData & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: r1_x
  cdr << ros_message.r1_x;
  // Member: r1_y
  cdr << ros_message.r1_y;
  // Member: r1_ang
  cdr << ros_message.r1_ang;
  // Member: r2_x
  cdr << ros_message.r2_x;
  // Member: r2_y
  cdr << ros_message.r2_y;
  // Member: r2_ang
  cdr << ros_message.r2_ang;
  // Member: obj1_x
  cdr << ros_message.obj1_x;
  // Member: obj1_y
  cdr << ros_message.obj1_y;
  // Member: obj1_ang
  cdr << ros_message.obj1_ang;
  // Member: obj2_x
  cdr << ros_message.obj2_x;
  // Member: obj2_y
  cdr << ros_message.obj2_y;
  // Member: obj2_ang
  cdr << ros_message.obj2_ang;
  // Member: tgt_x
  cdr << ros_message.tgt_x;
  // Member: tgt_y
  cdr << ros_message.tgt_y;
  // Member: tgt_ang
  cdr << ros_message.tgt_ang;
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  interfaces::msg::ImgData & ros_message)
{
  // Member: r1_x
  cdr >> ros_message.r1_x;

  // Member: r1_y
  cdr >> ros_message.r1_y;

  // Member: r1_ang
  cdr >> ros_message.r1_ang;

  // Member: r2_x
  cdr >> ros_message.r2_x;

  // Member: r2_y
  cdr >> ros_message.r2_y;

  // Member: r2_ang
  cdr >> ros_message.r2_ang;

  // Member: obj1_x
  cdr >> ros_message.obj1_x;

  // Member: obj1_y
  cdr >> ros_message.obj1_y;

  // Member: obj1_ang
  cdr >> ros_message.obj1_ang;

  // Member: obj2_x
  cdr >> ros_message.obj2_x;

  // Member: obj2_y
  cdr >> ros_message.obj2_y;

  // Member: obj2_ang
  cdr >> ros_message.obj2_ang;

  // Member: tgt_x
  cdr >> ros_message.tgt_x;

  // Member: tgt_y
  cdr >> ros_message.tgt_y;

  // Member: tgt_ang
  cdr >> ros_message.tgt_ang;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
get_serialized_size(
  const interfaces::msg::ImgData & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: r1_x
  {
    size_t item_size = sizeof(ros_message.r1_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: r1_y
  {
    size_t item_size = sizeof(ros_message.r1_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: r1_ang
  {
    size_t item_size = sizeof(ros_message.r1_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: r2_x
  {
    size_t item_size = sizeof(ros_message.r2_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: r2_y
  {
    size_t item_size = sizeof(ros_message.r2_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: r2_ang
  {
    size_t item_size = sizeof(ros_message.r2_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: obj1_x
  {
    size_t item_size = sizeof(ros_message.obj1_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: obj1_y
  {
    size_t item_size = sizeof(ros_message.obj1_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: obj1_ang
  {
    size_t item_size = sizeof(ros_message.obj1_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: obj2_x
  {
    size_t item_size = sizeof(ros_message.obj2_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: obj2_y
  {
    size_t item_size = sizeof(ros_message.obj2_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: obj2_ang
  {
    size_t item_size = sizeof(ros_message.obj2_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: tgt_x
  {
    size_t item_size = sizeof(ros_message.tgt_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: tgt_y
  {
    size_t item_size = sizeof(ros_message.tgt_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: tgt_ang
  {
    size_t item_size = sizeof(ros_message.tgt_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
max_serialized_size_ImgData(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: r1_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: r1_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: r1_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: r2_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: r2_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: r2_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: obj1_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: obj1_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: obj1_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: obj2_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: obj2_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: obj2_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: tgt_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: tgt_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: tgt_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static bool _ImgData__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const interfaces::msg::ImgData *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _ImgData__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<interfaces::msg::ImgData *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _ImgData__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const interfaces::msg::ImgData *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _ImgData__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_ImgData(full_bounded, 0);
}

static message_type_support_callbacks_t _ImgData__callbacks = {
  "interfaces::msg",
  "ImgData",
  _ImgData__cdr_serialize,
  _ImgData__cdr_deserialize,
  _ImgData__get_serialized_size,
  _ImgData__max_serialized_size
};

static rosidl_message_type_support_t _ImgData__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_ImgData__callbacks,
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
get_message_type_support_handle<interfaces::msg::ImgData>()
{
  return &interfaces::msg::typesupport_fastrtps_cpp::_ImgData__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, interfaces, msg, ImgData)() {
  return &interfaces::msg::typesupport_fastrtps_cpp::_ImgData__handle;
}

#ifdef __cplusplus
}
#endif
