// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/MotorVels.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS__BUILDER_HPP_

#include "interfaces/msg/detail/motor_vels__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_MotorVels_vel_m4
{
public:
  explicit Init_MotorVels_vel_m4(::interfaces::msg::MotorVels & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::MotorVels vel_m4(::interfaces::msg::MotorVels::_vel_m4_type arg)
  {
    msg_.vel_m4 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::MotorVels msg_;
};

class Init_MotorVels_vel_m3
{
public:
  explicit Init_MotorVels_vel_m3(::interfaces::msg::MotorVels & msg)
  : msg_(msg)
  {}
  Init_MotorVels_vel_m4 vel_m3(::interfaces::msg::MotorVels::_vel_m3_type arg)
  {
    msg_.vel_m3 = std::move(arg);
    return Init_MotorVels_vel_m4(msg_);
  }

private:
  ::interfaces::msg::MotorVels msg_;
};

class Init_MotorVels_vel_m2
{
public:
  explicit Init_MotorVels_vel_m2(::interfaces::msg::MotorVels & msg)
  : msg_(msg)
  {}
  Init_MotorVels_vel_m3 vel_m2(::interfaces::msg::MotorVels::_vel_m2_type arg)
  {
    msg_.vel_m2 = std::move(arg);
    return Init_MotorVels_vel_m3(msg_);
  }

private:
  ::interfaces::msg::MotorVels msg_;
};

class Init_MotorVels_vel_m1
{
public:
  Init_MotorVels_vel_m1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotorVels_vel_m2 vel_m1(::interfaces::msg::MotorVels::_vel_m1_type arg)
  {
    msg_.vel_m1 = std::move(arg);
    return Init_MotorVels_vel_m2(msg_);
  }

private:
  ::interfaces::msg::MotorVels msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::MotorVels>()
{
  return interfaces::msg::builder::Init_MotorVels_vel_m1();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS__BUILDER_HPP_
