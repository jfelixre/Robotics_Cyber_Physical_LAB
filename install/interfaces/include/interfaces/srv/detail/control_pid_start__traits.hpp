// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:srv/ControlPidStart.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__CONTROL_PID_START__TRAITS_HPP_
#define INTERFACES__SRV__DETAIL__CONTROL_PID_START__TRAITS_HPP_

#include "interfaces/srv/detail/control_pid_start__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::ControlPidStart_Request>()
{
  return "interfaces::srv::ControlPidStart_Request";
}

template<>
inline const char * name<interfaces::srv::ControlPidStart_Request>()
{
  return "interfaces/srv/ControlPidStart_Request";
}

template<>
struct has_fixed_size<interfaces::srv::ControlPidStart_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::srv::ControlPidStart_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::srv::ControlPidStart_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::ControlPidStart_Response>()
{
  return "interfaces::srv::ControlPidStart_Response";
}

template<>
inline const char * name<interfaces::srv::ControlPidStart_Response>()
{
  return "interfaces/srv/ControlPidStart_Response";
}

template<>
struct has_fixed_size<interfaces::srv::ControlPidStart_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::srv::ControlPidStart_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::srv::ControlPidStart_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::ControlPidStart>()
{
  return "interfaces::srv::ControlPidStart";
}

template<>
inline const char * name<interfaces::srv::ControlPidStart>()
{
  return "interfaces/srv/ControlPidStart";
}

template<>
struct has_fixed_size<interfaces::srv::ControlPidStart>
  : std::integral_constant<
    bool,
    has_fixed_size<interfaces::srv::ControlPidStart_Request>::value &&
    has_fixed_size<interfaces::srv::ControlPidStart_Response>::value
  >
{
};

template<>
struct has_bounded_size<interfaces::srv::ControlPidStart>
  : std::integral_constant<
    bool,
    has_bounded_size<interfaces::srv::ControlPidStart_Request>::value &&
    has_bounded_size<interfaces::srv::ControlPidStart_Response>::value
  >
{
};

template<>
struct is_service<interfaces::srv::ControlPidStart>
  : std::true_type
{
};

template<>
struct is_service_request<interfaces::srv::ControlPidStart_Request>
  : std::true_type
{
};

template<>
struct is_service_response<interfaces::srv::ControlPidStart_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__SRV__DETAIL__CONTROL_PID_START__TRAITS_HPP_
