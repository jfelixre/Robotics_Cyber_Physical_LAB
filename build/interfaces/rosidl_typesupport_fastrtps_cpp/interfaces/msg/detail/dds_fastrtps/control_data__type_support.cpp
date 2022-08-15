// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/control_data__rosidl_typesupport_fastrtps_cpp.hpp"
#include "interfaces/msg/detail/control_data__struct.hpp"

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
  const interfaces::msg::ControlData & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: time
  cdr << ros_message.time;
  // Member: fase
  cdr << ros_message.fase;
  // Member: x_robot
  cdr << ros_message.x_robot;
  // Member: y_robot
  cdr << ros_message.y_robot;
  // Member: ang_robot
  cdr << ros_message.ang_robot;
  // Member: x_obj
  cdr << ros_message.x_obj;
  // Member: y_obj
  cdr << ros_message.y_obj;
  // Member: ang_obj
  cdr << ros_message.ang_obj;
  // Member: x_tgt
  cdr << ros_message.x_tgt;
  // Member: y_tgt
  cdr << ros_message.y_tgt;
  // Member: ang_tgt
  cdr << ros_message.ang_tgt;
  // Member: x_goal
  cdr << ros_message.x_goal;
  // Member: y_goal
  cdr << ros_message.y_goal;
  // Member: ang_goal
  cdr << ros_message.ang_goal;
  // Member: x_error
  cdr << ros_message.x_error;
  // Member: y_error
  cdr << ros_message.y_error;
  // Member: ang_error
  cdr << ros_message.ang_error;
  // Member: vel_x_world
  cdr << ros_message.vel_x_world;
  // Member: vel_y_world
  cdr << ros_message.vel_y_world;
  // Member: vel_ang_world
  cdr << ros_message.vel_ang_world;
  // Member: vel_x_robot
  cdr << ros_message.vel_x_robot;
  // Member: vel_y_robot
  cdr << ros_message.vel_y_robot;
  // Member: vel_ang_robot
  cdr << ros_message.vel_ang_robot;
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  interfaces::msg::ControlData & ros_message)
{
  // Member: time
  cdr >> ros_message.time;

  // Member: fase
  cdr >> ros_message.fase;

  // Member: x_robot
  cdr >> ros_message.x_robot;

  // Member: y_robot
  cdr >> ros_message.y_robot;

  // Member: ang_robot
  cdr >> ros_message.ang_robot;

  // Member: x_obj
  cdr >> ros_message.x_obj;

  // Member: y_obj
  cdr >> ros_message.y_obj;

  // Member: ang_obj
  cdr >> ros_message.ang_obj;

  // Member: x_tgt
  cdr >> ros_message.x_tgt;

  // Member: y_tgt
  cdr >> ros_message.y_tgt;

  // Member: ang_tgt
  cdr >> ros_message.ang_tgt;

  // Member: x_goal
  cdr >> ros_message.x_goal;

  // Member: y_goal
  cdr >> ros_message.y_goal;

  // Member: ang_goal
  cdr >> ros_message.ang_goal;

  // Member: x_error
  cdr >> ros_message.x_error;

  // Member: y_error
  cdr >> ros_message.y_error;

  // Member: ang_error
  cdr >> ros_message.ang_error;

  // Member: vel_x_world
  cdr >> ros_message.vel_x_world;

  // Member: vel_y_world
  cdr >> ros_message.vel_y_world;

  // Member: vel_ang_world
  cdr >> ros_message.vel_ang_world;

  // Member: vel_x_robot
  cdr >> ros_message.vel_x_robot;

  // Member: vel_y_robot
  cdr >> ros_message.vel_y_robot;

  // Member: vel_ang_robot
  cdr >> ros_message.vel_ang_robot;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
get_serialized_size(
  const interfaces::msg::ControlData & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: time
  {
    size_t item_size = sizeof(ros_message.time);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: fase
  {
    size_t item_size = sizeof(ros_message.fase);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: x_robot
  {
    size_t item_size = sizeof(ros_message.x_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: y_robot
  {
    size_t item_size = sizeof(ros_message.y_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: ang_robot
  {
    size_t item_size = sizeof(ros_message.ang_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: x_obj
  {
    size_t item_size = sizeof(ros_message.x_obj);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: y_obj
  {
    size_t item_size = sizeof(ros_message.y_obj);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: ang_obj
  {
    size_t item_size = sizeof(ros_message.ang_obj);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: x_tgt
  {
    size_t item_size = sizeof(ros_message.x_tgt);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: y_tgt
  {
    size_t item_size = sizeof(ros_message.y_tgt);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: ang_tgt
  {
    size_t item_size = sizeof(ros_message.ang_tgt);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: x_goal
  {
    size_t item_size = sizeof(ros_message.x_goal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: y_goal
  {
    size_t item_size = sizeof(ros_message.y_goal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: ang_goal
  {
    size_t item_size = sizeof(ros_message.ang_goal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: x_error
  {
    size_t item_size = sizeof(ros_message.x_error);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: y_error
  {
    size_t item_size = sizeof(ros_message.y_error);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: ang_error
  {
    size_t item_size = sizeof(ros_message.ang_error);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_x_world
  {
    size_t item_size = sizeof(ros_message.vel_x_world);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_y_world
  {
    size_t item_size = sizeof(ros_message.vel_y_world);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_ang_world
  {
    size_t item_size = sizeof(ros_message.vel_ang_world);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_x_robot
  {
    size_t item_size = sizeof(ros_message.vel_x_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_y_robot
  {
    size_t item_size = sizeof(ros_message.vel_y_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: vel_ang_robot
  {
    size_t item_size = sizeof(ros_message.vel_ang_robot);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_interfaces
max_serialized_size_ControlData(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: time
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: fase
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: x_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: y_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: ang_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: x_obj
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: y_obj
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: ang_obj
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: x_tgt
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: y_tgt
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: ang_tgt
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: x_goal
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: y_goal
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: ang_goal
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: x_error
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: y_error
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: ang_error
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_x_world
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_y_world
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_ang_world
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_x_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_y_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: vel_ang_robot
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static bool _ControlData__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const interfaces::msg::ControlData *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _ControlData__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<interfaces::msg::ControlData *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _ControlData__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const interfaces::msg::ControlData *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _ControlData__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_ControlData(full_bounded, 0);
}

static message_type_support_callbacks_t _ControlData__callbacks = {
  "interfaces::msg",
  "ControlData",
  _ControlData__cdr_serialize,
  _ControlData__cdr_deserialize,
  _ControlData__get_serialized_size,
  _ControlData__max_serialized_size
};

static rosidl_message_type_support_t _ControlData__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_ControlData__callbacks,
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
get_message_type_support_handle<interfaces::msg::ControlData>()
{
  return &interfaces::msg::typesupport_fastrtps_cpp::_ControlData__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, interfaces, msg, ControlData)() {
  return &interfaces::msg::typesupport_fastrtps_cpp::_ControlData__handle;
}

#ifdef __cplusplus
}
#endif
