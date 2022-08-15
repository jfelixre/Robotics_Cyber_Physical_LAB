// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/MotorVelsWArm.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__TRAITS_HPP_

#include "interfaces/msg/detail/motor_vels_w_arm__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::msg::MotorVelsWArm>()
{
  return "interfaces::msg::MotorVelsWArm";
}

template<>
inline const char * name<interfaces::msg::MotorVelsWArm>()
{
  return "interfaces/msg/MotorVelsWArm";
}

template<>
struct has_fixed_size<interfaces::msg::MotorVelsWArm>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::msg::MotorVelsWArm>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::msg::MotorVelsWArm>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__TRAITS_HPP_
