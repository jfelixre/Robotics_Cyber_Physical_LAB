// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from interfaces:srv/RobotVel.idl
// generated code does not contain a copyright notice
#include "interfaces/srv/detail/robot_vel__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/srv/detail/robot_vel__struct.h"
#include "interfaces/srv/detail/robot_vel__functions.h"
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


using _RobotVel_Request__ros_msg_type = interfaces__srv__RobotVel_Request;

static bool _RobotVel_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _RobotVel_Request__ros_msg_type * ros_message = static_cast<const _RobotVel_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: x_vel
  {
    cdr << ros_message->x_vel;
  }

  // Field name: y_vel
  {
    cdr << ros_message->y_vel;
  }

  // Field name: ang_vel
  {
    cdr << ros_message->ang_vel;
  }

  // Field name: b1_vel
  {
    cdr << ros_message->b1_vel;
  }

  // Field name: b2_vel
  {
    cdr << ros_message->b2_vel;
  }

  // Field name: b3_vel
  {
    cdr << ros_message->b3_vel;
  }

  // Field name: g1_vel
  {
    cdr << ros_message->g1_vel;
  }

  return true;
}

static bool _RobotVel_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _RobotVel_Request__ros_msg_type * ros_message = static_cast<_RobotVel_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: x_vel
  {
    cdr >> ros_message->x_vel;
  }

  // Field name: y_vel
  {
    cdr >> ros_message->y_vel;
  }

  // Field name: ang_vel
  {
    cdr >> ros_message->ang_vel;
  }

  // Field name: b1_vel
  {
    cdr >> ros_message->b1_vel;
  }

  // Field name: b2_vel
  {
    cdr >> ros_message->b2_vel;
  }

  // Field name: b3_vel
  {
    cdr >> ros_message->b3_vel;
  }

  // Field name: g1_vel
  {
    cdr >> ros_message->g1_vel;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__srv__RobotVel_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _RobotVel_Request__ros_msg_type * ros_message = static_cast<const _RobotVel_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name x_vel
  {
    size_t item_size = sizeof(ros_message->x_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y_vel
  {
    size_t item_size = sizeof(ros_message->y_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ang_vel
  {
    size_t item_size = sizeof(ros_message->ang_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b1_vel
  {
    size_t item_size = sizeof(ros_message->b1_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b2_vel
  {
    size_t item_size = sizeof(ros_message->b2_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name b3_vel
  {
    size_t item_size = sizeof(ros_message->b3_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name g1_vel
  {
    size_t item_size = sizeof(ros_message->g1_vel);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _RobotVel_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__srv__RobotVel_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__srv__RobotVel_Request(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: x_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ang_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b1_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b2_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: b3_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: g1_vel
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _RobotVel_Request__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__srv__RobotVel_Request(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_RobotVel_Request = {
  "interfaces::srv",
  "RobotVel_Request",
  _RobotVel_Request__cdr_serialize,
  _RobotVel_Request__cdr_deserialize,
  _RobotVel_Request__get_serialized_size,
  _RobotVel_Request__max_serialized_size
};

static rosidl_message_type_support_t _RobotVel_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_RobotVel_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, RobotVel_Request)() {
  return &_RobotVel_Request__type_support;
}

#if defined(__cplusplus)
}
#endif

// already included above
// #include <cassert>
// already included above
// #include <limits>
// already included above
// #include <string>
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
// already included above
// #include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
// already included above
// #include "interfaces/srv/detail/robot_vel__struct.h"
// already included above
// #include "interfaces/srv/detail/robot_vel__functions.h"
// already included above
// #include "fastcdr/Cdr.h"

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


using _RobotVel_Response__ros_msg_type = interfaces__srv__RobotVel_Response;

static bool _RobotVel_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _RobotVel_Response__ros_msg_type * ros_message = static_cast<const _RobotVel_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: sent
  {
    cdr << ros_message->sent;
  }

  return true;
}

static bool _RobotVel_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _RobotVel_Response__ros_msg_type * ros_message = static_cast<_RobotVel_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: sent
  {
    cdr >> ros_message->sent;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__srv__RobotVel_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _RobotVel_Response__ros_msg_type * ros_message = static_cast<const _RobotVel_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name sent
  {
    size_t item_size = sizeof(ros_message->sent);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _RobotVel_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__srv__RobotVel_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__srv__RobotVel_Response(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: sent
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _RobotVel_Response__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__srv__RobotVel_Response(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_RobotVel_Response = {
  "interfaces::srv",
  "RobotVel_Response",
  _RobotVel_Response__cdr_serialize,
  _RobotVel_Response__cdr_deserialize,
  _RobotVel_Response__get_serialized_size,
  _RobotVel_Response__max_serialized_size
};

static rosidl_message_type_support_t _RobotVel_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_RobotVel_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, RobotVel_Response)() {
  return &_RobotVel_Response__type_support;
}

#if defined(__cplusplus)
}
#endif

#include "rosidl_typesupport_fastrtps_cpp/service_type_support.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/srv/robot_vel.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t RobotVel__callbacks = {
  "interfaces::srv",
  "RobotVel",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, RobotVel_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, RobotVel_Response)(),
};

static rosidl_service_type_support_t RobotVel__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &RobotVel__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, RobotVel)() {
  return &RobotVel__handle;
}

#if defined(__cplusplus)
}
#endif
