// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from interfaces:srv/ControlPidStart.idl
// generated code does not contain a copyright notice
#include "interfaces/srv/detail/control_pid_start__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "interfaces/srv/detail/control_pid_start__struct.h"
#include "interfaces/srv/detail/control_pid_start__functions.h"
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


using _ControlPidStart_Request__ros_msg_type = interfaces__srv__ControlPidStart_Request;

static bool _ControlPidStart_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ControlPidStart_Request__ros_msg_type * ros_message = static_cast<const _ControlPidStart_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: fase
  {
    cdr << ros_message->fase;
  }

  return true;
}

static bool _ControlPidStart_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ControlPidStart_Request__ros_msg_type * ros_message = static_cast<_ControlPidStart_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: fase
  {
    cdr >> ros_message->fase;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__srv__ControlPidStart_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ControlPidStart_Request__ros_msg_type * ros_message = static_cast<const _ControlPidStart_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name fase
  {
    size_t item_size = sizeof(ros_message->fase);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ControlPidStart_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__srv__ControlPidStart_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__srv__ControlPidStart_Request(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: fase
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _ControlPidStart_Request__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__srv__ControlPidStart_Request(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_ControlPidStart_Request = {
  "interfaces::srv",
  "ControlPidStart_Request",
  _ControlPidStart_Request__cdr_serialize,
  _ControlPidStart_Request__cdr_deserialize,
  _ControlPidStart_Request__get_serialized_size,
  _ControlPidStart_Request__max_serialized_size
};

static rosidl_message_type_support_t _ControlPidStart_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ControlPidStart_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, ControlPidStart_Request)() {
  return &_ControlPidStart_Request__type_support;
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
// #include "interfaces/srv/detail/control_pid_start__struct.h"
// already included above
// #include "interfaces/srv/detail/control_pid_start__functions.h"
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


using _ControlPidStart_Response__ros_msg_type = interfaces__srv__ControlPidStart_Response;

static bool _ControlPidStart_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ControlPidStart_Response__ros_msg_type * ros_message = static_cast<const _ControlPidStart_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: finish
  {
    cdr << ros_message->finish;
  }

  return true;
}

static bool _ControlPidStart_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ControlPidStart_Response__ros_msg_type * ros_message = static_cast<_ControlPidStart_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: finish
  {
    cdr >> ros_message->finish;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t get_serialized_size_interfaces__srv__ControlPidStart_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ControlPidStart_Response__ros_msg_type * ros_message = static_cast<const _ControlPidStart_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name finish
  {
    size_t item_size = sizeof(ros_message->finish);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ControlPidStart_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_interfaces__srv__ControlPidStart_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_interfaces
size_t max_serialized_size_interfaces__srv__ControlPidStart_Response(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: finish
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _ControlPidStart_Response__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_interfaces__srv__ControlPidStart_Response(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_ControlPidStart_Response = {
  "interfaces::srv",
  "ControlPidStart_Response",
  _ControlPidStart_Response__cdr_serialize,
  _ControlPidStart_Response__cdr_deserialize,
  _ControlPidStart_Response__get_serialized_size,
  _ControlPidStart_Response__max_serialized_size
};

static rosidl_message_type_support_t _ControlPidStart_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ControlPidStart_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, ControlPidStart_Response)() {
  return &_ControlPidStart_Response__type_support;
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
#include "interfaces/srv/control_pid_start.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t ControlPidStart__callbacks = {
  "interfaces::srv",
  "ControlPidStart",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, ControlPidStart_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, ControlPidStart_Response)(),
};

static rosidl_service_type_support_t ControlPidStart__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &ControlPidStart__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, interfaces, srv, ControlPidStart)() {
  return &ControlPidStart__handle;
}

#if defined(__cplusplus)
}
#endif
