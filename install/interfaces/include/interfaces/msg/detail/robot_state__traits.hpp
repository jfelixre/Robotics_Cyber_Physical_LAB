// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_

#include "interfaces/msg/detail/robot_state__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::msg::RobotState>()
{
  return "interfaces::msg::RobotState";
}

template<>
inline const char * name<interfaces::msg::RobotState>()
{
  return "interfaces/msg/RobotState";
}

template<>
struct has_fixed_size<interfaces::msg::RobotState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::msg::RobotState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::msg::RobotState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
