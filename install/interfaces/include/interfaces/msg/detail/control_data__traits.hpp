// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__CONTROL_DATA__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__CONTROL_DATA__TRAITS_HPP_

#include "interfaces/msg/detail/control_data__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::msg::ControlData>()
{
  return "interfaces::msg::ControlData";
}

template<>
inline const char * name<interfaces::msg::ControlData>()
{
  return "interfaces/msg/ControlData";
}

template<>
struct has_fixed_size<interfaces::msg::ControlData>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::msg::ControlData>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::msg::ControlData>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__CONTROL_DATA__TRAITS_HPP_
