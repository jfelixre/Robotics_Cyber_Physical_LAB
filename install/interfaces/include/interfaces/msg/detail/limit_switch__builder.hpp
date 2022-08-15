// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/LimitSwitch.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__LIMIT_SWITCH__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__LIMIT_SWITCH__BUILDER_HPP_

#include "interfaces/msg/detail/limit_switch__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_LimitSwitch_b3_max
{
public:
  explicit Init_LimitSwitch_b3_max(::interfaces::msg::LimitSwitch & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::LimitSwitch b3_max(::interfaces::msg::LimitSwitch::_b3_max_type arg)
  {
    msg_.b3_max = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::LimitSwitch msg_;
};

class Init_LimitSwitch_b3_min
{
public:
  explicit Init_LimitSwitch_b3_min(::interfaces::msg::LimitSwitch & msg)
  : msg_(msg)
  {}
  Init_LimitSwitch_b3_max b3_min(::interfaces::msg::LimitSwitch::_b3_min_type arg)
  {
    msg_.b3_min = std::move(arg);
    return Init_LimitSwitch_b3_max(msg_);
  }

private:
  ::interfaces::msg::LimitSwitch msg_;
};

class Init_LimitSwitch_b2_max
{
public:
  explicit Init_LimitSwitch_b2_max(::interfaces::msg::LimitSwitch & msg)
  : msg_(msg)
  {}
  Init_LimitSwitch_b3_min b2_max(::interfaces::msg::LimitSwitch::_b2_max_type arg)
  {
    msg_.b2_max = std::move(arg);
    return Init_LimitSwitch_b3_min(msg_);
  }

private:
  ::interfaces::msg::LimitSwitch msg_;
};

class Init_LimitSwitch_b2_min
{
public:
  explicit Init_LimitSwitch_b2_min(::interfaces::msg::LimitSwitch & msg)
  : msg_(msg)
  {}
  Init_LimitSwitch_b2_max b2_min(::interfaces::msg::LimitSwitch::_b2_min_type arg)
  {
    msg_.b2_min = std::move(arg);
    return Init_LimitSwitch_b2_max(msg_);
  }

private:
  ::interfaces::msg::LimitSwitch msg_;
};

class Init_LimitSwitch_b1_max
{
public:
  explicit Init_LimitSwitch_b1_max(::interfaces::msg::LimitSwitch & msg)
  : msg_(msg)
  {}
  Init_LimitSwitch_b2_min b1_max(::interfaces::msg::LimitSwitch::_b1_max_type arg)
  {
    msg_.b1_max = std::move(arg);
    return Init_LimitSwitch_b2_min(msg_);
  }

private:
  ::interfaces::msg::LimitSwitch msg_;
};

class Init_LimitSwitch_b1_min
{
public:
  Init_LimitSwitch_b1_min()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LimitSwitch_b1_max b1_min(::interfaces::msg::LimitSwitch::_b1_min_type arg)
  {
    msg_.b1_min = std::move(arg);
    return Init_LimitSwitch_b1_max(msg_);
  }

private:
  ::interfaces::msg::LimitSwitch msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::LimitSwitch>()
{
  return interfaces::msg::builder::Init_LimitSwitch_b1_min();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__LIMIT_SWITCH__BUILDER_HPP_
