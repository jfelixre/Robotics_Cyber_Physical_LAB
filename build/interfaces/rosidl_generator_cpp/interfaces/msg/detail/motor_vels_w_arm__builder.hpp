// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/MotorVelsWArm.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__BUILDER_HPP_

#include "interfaces/msg/detail/motor_vels_w_arm__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_MotorVelsWArm_vel_g1
{
public:
  explicit Init_MotorVelsWArm_vel_g1(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::MotorVelsWArm vel_g1(::interfaces::msg::MotorVelsWArm::_vel_g1_type arg)
  {
    msg_.vel_g1 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_b3
{
public:
  explicit Init_MotorVelsWArm_vel_b3(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  Init_MotorVelsWArm_vel_g1 vel_b3(::interfaces::msg::MotorVelsWArm::_vel_b3_type arg)
  {
    msg_.vel_b3 = std::move(arg);
    return Init_MotorVelsWArm_vel_g1(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_b2
{
public:
  explicit Init_MotorVelsWArm_vel_b2(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  Init_MotorVelsWArm_vel_b3 vel_b2(::interfaces::msg::MotorVelsWArm::_vel_b2_type arg)
  {
    msg_.vel_b2 = std::move(arg);
    return Init_MotorVelsWArm_vel_b3(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_b1
{
public:
  explicit Init_MotorVelsWArm_vel_b1(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  Init_MotorVelsWArm_vel_b2 vel_b1(::interfaces::msg::MotorVelsWArm::_vel_b1_type arg)
  {
    msg_.vel_b1 = std::move(arg);
    return Init_MotorVelsWArm_vel_b2(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_m4
{
public:
  explicit Init_MotorVelsWArm_vel_m4(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  Init_MotorVelsWArm_vel_b1 vel_m4(::interfaces::msg::MotorVelsWArm::_vel_m4_type arg)
  {
    msg_.vel_m4 = std::move(arg);
    return Init_MotorVelsWArm_vel_b1(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_m3
{
public:
  explicit Init_MotorVelsWArm_vel_m3(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  Init_MotorVelsWArm_vel_m4 vel_m3(::interfaces::msg::MotorVelsWArm::_vel_m3_type arg)
  {
    msg_.vel_m3 = std::move(arg);
    return Init_MotorVelsWArm_vel_m4(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_m2
{
public:
  explicit Init_MotorVelsWArm_vel_m2(::interfaces::msg::MotorVelsWArm & msg)
  : msg_(msg)
  {}
  Init_MotorVelsWArm_vel_m3 vel_m2(::interfaces::msg::MotorVelsWArm::_vel_m2_type arg)
  {
    msg_.vel_m2 = std::move(arg);
    return Init_MotorVelsWArm_vel_m3(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

class Init_MotorVelsWArm_vel_m1
{
public:
  Init_MotorVelsWArm_vel_m1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotorVelsWArm_vel_m2 vel_m1(::interfaces::msg::MotorVelsWArm::_vel_m1_type arg)
  {
    msg_.vel_m1 = std::move(arg);
    return Init_MotorVelsWArm_vel_m2(msg_);
  }

private:
  ::interfaces::msg::MotorVelsWArm msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::MotorVelsWArm>()
{
  return interfaces::msg::builder::Init_MotorVelsWArm_vel_m1();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__BUILDER_HPP_
