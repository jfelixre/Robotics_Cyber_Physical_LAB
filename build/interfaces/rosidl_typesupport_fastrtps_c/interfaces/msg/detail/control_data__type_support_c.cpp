// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/control_data__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/msg/detail/control_data__struct.h"
#include "interfaces/msg/detail/control_data__functions.h"
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


using _ControlData__ros_msg_type = interfaces__msg__ControlData;

static bool _ControlData__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ControlData__ros_msg_type * ros_message = static_cast<const _ControlData__ros_msg_type *>(untyped_ros_message);
  // Field name: time
  {
    cdr << ros_message->time;
  }

  // Field name: fase
  {
    cdr << ros_message->fase;
  }

  // Field name: x_robot
  {
    cdr << ros_message->x_robot;
  }

  // Field name: y_robot
  {
    cdr << ros_message->y_robot;
  }

  // Field name: ang_robot
  {
    cdr << ros_message->ang_robot;
  }

  // Field name: x_obj
  {
    cdr << ros_message->x_obj;
  }

  // Field name: y_obj
  {
    cdr << ros_message->y_obj;
  }

  // Field name: ang_obj
  {
    cdr << ros_message->ang_obj;
  }

  // Field name: x_tgt
  {
    cdr << ros_message->x_tgt;
  }

  // Field name: y_tgt
  {
    cdr << ros_message->y_tgt;
  }

  // Field name: ang_tgt
  {
    cdr << ros_message->ang_tgt;
  }

  // Field name: x_goal
  {
    cdr << ros_message->x_goal;
  }

  // Field name: y_goal
  {
    cdr << ros_message->y_goal;
  }

  // Field name: ang_goal
  {
    cdr << ros_message->ang_goal;
  }

  // Field name: x_error
  {
    cdr << ros_message->x_error;
  }

  // Field name: y_error
  {
    cdr << ros_message->y_error;
  }

  // Field name: ang_error
  {
    cdr << ros_message->ang_error;
  }

  // Field name: vel_x_world
  {
    cdr << ros_message->vel_x_world;
  }

  // Field name: vel_y_world
  {
    cdr << ros_message->vel_y_world;
  }

  // Field name: vel_ang_world
  {
    cdr << ros_message->vel_ang_world;
  }

  // Field name: vel_x_robot
  {
    cdr << ros_message->vel_x_robot;
  }

  // Field name: vel_y_robot
  {
    cdr << ros_message->vel_y_robot;
  }

  // Field name: vel_ang_robot
  {
    cdr << ros_message->vel_ang_robot;
  }

  return true;
}

static bool _ControlData__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ControlData__ros_msg_type * ros_message = static_cast<_ControlData__ros_msg_type *>(untyped_ros_message);
  // Field name: time
  {
    cdr >> ros_message->time;
  }

  // Field name: fase
  {
    cdr >> ros_message->fase;
  }

  // Field name: x_robot
  {
    cdr >> ros_message->x_robot;
  }

  // Field name: y_robot
  {
    cdr >> ros_message->y_robot;
  }

  // Field name: ang_robot
  {
    cdr >> ros_message->ang_robot;
  }

  // Field name: x_obj
  {
    cdr >> ros_message->x_obj;
  }

  // Field name: y_obj
  {
    cdr >> ros_message->y_obj;
  }

  // Field name: ang_obj
  {
    cdr >> ros_message->ang_obj;
  }

  // Field name: x_tgt
  {
    cdr >> ros_message->x_tgt;
  }

  // Field name: y_tgt
  {
    cdr >> ros_message->y_tgt;
  }

  // Field name: ang_tgt
  {
    cdr >> ros_message->ang_tgt;
  }

  // Field name: x_goal
  {
    cdr >> ros_message->x_goal;
  }

  // Field name: y_goal
  {
    cdr >> ros_message->y_goal;
  }

  // Field name: ang_goal
  {
    cdr >> ros_message->ang_goal;
  }

  // Field name: x_error
  {
    cdr >> ros_message->x_error;
  }

  // Field name: y_error
  {
    cdr >> ros_message->y_error;
  }

  // Field name: ang_error
  {
    cdr >> ros_message->ang_error;
  }

  // Field name: vel_x_world
  {
    cdr >> ros_message->vel_x_world;
  }

  // Field name: vel_y_world
  {
    cdr >> ros_message->vel_y_world;
  }

  // Field name: vel_ang_world
  {
    cdr >> ros_message->vel_ang_world;
  }

  // Field name: vel_x_robot
  {
    cdr >> ros_message->vel_x_robot;
  }

  // Field name: vel_y_robot
  {
    cdr >> ros_message->vel_y_robot;
  }

  // Field name: vel_ang_robot
  {
    cdr >> ros_message->vel_ang_robot;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__msg__ControlData(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ControlData__ros_msg_type * ros_message = static_cast<const _ControlData__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name time
  {
    size_t item_size = sizeof(ros_message->time);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name fase
  {
    size_t item_size = sizeof(ros_message->fase);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name x_robot
  {
    size_t item_size = sizeof(ros_message->x_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y_robot
  {
    size_t item_size = sizeof(ros_message->y_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ang_robot
  {
    size_t item_size = sizeof(ros_message->ang_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name x_obj
  {
    size_t item_size = sizeof(ros_message->x_obj);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y_obj
  {
    size_t item_size = sizeof(ros_message->y_obj);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ang_obj
  {
    size_t item_size = sizeof(ros_message->ang_obj);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name x_tgt
  {
    size_t item_size = sizeof(ros_message->x_tgt);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y_tgt
  {
    size_t item_size = sizeof(ros_message->y_tgt);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ang_tgt
  {
    size_t item_size = sizeof(ros_message->ang_tgt);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name x_goal
  {
    size_t item_size = sizeof(ros_message->x_goal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y_goal
  {
    size_t item_size = sizeof(ros_message->y_goal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ang_goal
  {
    size_t item_size = sizeof(ros_message->ang_goal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name x_error
  {
    size_t item_size = sizeof(ros_message->x_error);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y_error
  {
    size_t item_size = sizeof(ros_message->y_error);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ang_error
  {
    size_t item_size = sizeof(ros_message->ang_error);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_x_world
  {
    size_t item_size = sizeof(ros_message->vel_x_world);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_y_world
  {
    size_t item_size = sizeof(ros_message->vel_y_world);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_ang_world
  {
    size_t item_size = sizeof(ros_message->vel_ang_world);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_x_robot
  {
    size_t item_size = sizeof(ros_message->vel_x_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_y_robot
  {
    size_t item_size = sizeof(ros_message->vel_y_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name vel_ang_robot
  {
    size_t item_size = sizeof(ros_message->vel_ang_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ControlData__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__msg__ControlData(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__msg__ControlData(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: time
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: fase
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: x_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ang_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: x_obj
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y_obj
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ang_obj
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: x_tgt
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y_tgt
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ang_tgt
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: x_goal
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y_goal
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ang_goal
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: x_error
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y_error
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ang_error
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_x_world
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_y_world
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_ang_world
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_x_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_y_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: vel_ang_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _ControlData__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__msg__ControlData(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_ControlData = {
  "interfaces::msg",
  "ControlData",
  _ControlData__cdr_serialize,
  _ControlData__cdr_deserialize,
  _ControlData__get_serialized_size,
  _ControlData__max_serialized_size
};

static rosidl_message_type_support_t _ControlData__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ControlData,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, msg, ControlData)() {
  return &_ControlData__type_support;
}

#if defined(__cplusplus)
}
#endif
