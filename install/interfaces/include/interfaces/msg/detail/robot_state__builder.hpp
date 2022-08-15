// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_

#include "interfaces/msg/detail/robot_state__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_RobotState_robot_state
{
public:
  Init_RobotState_robot_state()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::msg::RobotState robot_state(::interfaces::msg::RobotState::_robot_state_type arg)
  {
    msg_.robot_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::RobotState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::RobotState>()
{
  return interfaces::msg::builder::Init_RobotState_robot_state();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
