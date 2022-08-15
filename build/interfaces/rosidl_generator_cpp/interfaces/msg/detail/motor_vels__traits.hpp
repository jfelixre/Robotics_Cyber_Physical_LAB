// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/MotorVels.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS__TRAITS_HPP_

#include "interfaces/msg/detail/motor_vels__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::msg::MotorVels>()
{
  return "interfaces::msg::MotorVels";
}

template<>
inline const char * name<interfaces::msg::MotorVels>()
{
  return "interfaces/msg/MotorVels";
}

template<>
struct has_fixed_size<interfaces::msg::MotorVels>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::msg::MotorVels>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::msg::MotorVels>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS__TRAITS_HPP_
