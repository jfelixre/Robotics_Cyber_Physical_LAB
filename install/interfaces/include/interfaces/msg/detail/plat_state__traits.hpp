// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/PlatState.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__PLAT_STATE__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__PLAT_STATE__TRAITS_HPP_

#include "interfaces/msg/detail/plat_state__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::msg::PlatState>()
{
  return "interfaces::msg::PlatState";
}

template<>
inline const char * name<interfaces::msg::PlatState>()
{
  return "interfaces/msg/PlatState";
}

template<>
struct has_fixed_size<interfaces::msg::PlatState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::msg::PlatState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::msg::PlatState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__PLAT_STATE__TRAITS_HPP_
