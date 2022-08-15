// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from interfaces:msg/ImgData.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/img_data__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/msg/detail/img_data__struct.h"
#include "interfaces/msg/detail/img_data__functions.h"
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


using _ImgData__ros_msg_type = interfaces__msg__ImgData;

static bool _ImgData__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ImgData__ros_msg_type * ros_message = static_cast<const _ImgData__ros_msg_type *>(untyped_ros_message);
  // Field name: r1_x
  {
    cdr << ros_message->r1_x;
  }

  // Field name: r1_y
  {
    cdr << ros_message->r1_y;
  }

  // Field name: r1_ang
  {
    cdr << ros_message->r1_ang;
  }

  // Field name: r2_x
  {
    cdr << ros_message->r2_x;
  }

  // Field name: r2_y
  {
    cdr << ros_message->r2_y;
  }

  // Field name: r2_ang
  {
    cdr << ros_message->r2_ang;
  }

  // Field name: obj1_x
  {
    cdr << ros_message->obj1_x;
  }

  // Field name: obj1_y
  {
    cdr << ros_message->obj1_y;
  }

  // Field name: obj1_ang
  {
    cdr << ros_message->obj1_ang;
  }

  // Field name: obj2_x
  {
    cdr << ros_message->obj2_x;
  }

  // Field name: obj2_y
  {
    cdr << ros_message->obj2_y;
  }

  // Field name: obj2_ang
  {
    cdr << ros_message->obj2_ang;
  }

  // Field name: tgt_x
  {
    cdr << ros_message->tgt_x;
  }

  // Field name: tgt_y
  {
    cdr << ros_message->tgt_y;
  }

  // Field name: tgt_ang
  {
    cdr << ros_message->tgt_ang;
  }

  return true;
}

static bool _ImgData__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ImgData__ros_msg_type * ros_message = static_cast<_ImgData__ros_msg_type *>(untyped_ros_message);
  // Field name: r1_x
  {
    cdr >> ros_message->r1_x;
  }

  // Field name: r1_y
  {
    cdr >> ros_message->r1_y;
  }

  // Field name: r1_ang
  {
    cdr >> ros_message->r1_ang;
  }

  // Field name: r2_x
  {
    cdr >> ros_message->r2_x;
  }

  // Field name: r2_y
  {
    cdr >> ros_message->r2_y;
  }

  // Field name: r2_ang
  {
    cdr >> ros_message->r2_ang;
  }

  // Field name: obj1_x
  {
    cdr >> ros_message->obj1_x;
  }

  // Field name: obj1_y
  {
    cdr >> ros_message->obj1_y;
  }

  // Field name: obj1_ang
  {
    cdr >> ros_message->obj1_ang;
  }

  // Field name: obj2_x
  {
    cdr >> ros_message->obj2_x;
  }

  // Field name: obj2_y
  {
    cdr >> ros_message->obj2_y;
  }

  // Field name: obj2_ang
  {
    cdr >> ros_message->obj2_ang;
  }

  // Field name: tgt_x
  {
    cdr >> ros_message->tgt_x;
  }

  // Field name: tgt_y
  {
    cdr >> ros_message->tgt_y;
  }

  // Field name: tgt_ang
  {
    cdr >> ros_message->tgt_ang;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__msg__ImgData(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ImgData__ros_msg_type * ros_message = static_cast<const _ImgData__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name r1_x
  {
    size_t item_size = sizeof(ros_message->r1_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name r1_y
  {
    size_t item_size = sizeof(ros_message->r1_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name r1_ang
  {
    size_t item_size = sizeof(ros_message->r1_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name r2_x
  {
    size_t item_size = sizeof(ros_message->r2_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name r2_y
  {
    size_t item_size = sizeof(ros_message->r2_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name r2_ang
  {
    size_t item_size = sizeof(ros_message->r2_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name obj1_x
  {
    size_t item_size = sizeof(ros_message->obj1_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name obj1_y
  {
    size_t item_size = sizeof(ros_message->obj1_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name obj1_ang
  {
    size_t item_size = sizeof(ros_message->obj1_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name obj2_x
  {
    size_t item_size = sizeof(ros_message->obj2_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name obj2_y
  {
    size_t item_size = sizeof(ros_message->obj2_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name obj2_ang
  {
    size_t item_size = sizeof(ros_message->obj2_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name tgt_x
  {
    size_t item_size = sizeof(ros_message->tgt_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name tgt_y
  {
    size_t item_size = sizeof(ros_message->tgt_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name tgt_ang
  {
    size_t item_size = sizeof(ros_message->tgt_ang);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ImgData__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__msg__ImgData(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__msg__ImgData(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: r1_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: r1_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: r1_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: r2_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: r2_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: r2_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: obj1_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: obj1_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: obj1_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: obj2_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: obj2_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: obj2_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: tgt_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: tgt_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: tgt_ang
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _ImgData__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__msg__ImgData(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_ImgData = {
  "interfaces::msg",
  "ImgData",
  _ImgData__cdr_serialize,
  _ImgData__cdr_deserialize,
  _ImgData__get_serialized_size,
  _ImgData__max_serialized_size
};

static rosidl_message_type_support_t _ImgData__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ImgData,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, msg, ImgData)() {
  return &_ImgData__type_support;
}

#if defined(__cplusplus)
}
#endif
