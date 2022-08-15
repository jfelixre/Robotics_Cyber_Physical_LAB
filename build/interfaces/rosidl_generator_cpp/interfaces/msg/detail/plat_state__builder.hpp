// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/PlatState.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__PLAT_STATE__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__PLAT_STATE__BUILDER_HPP_

#include "interfaces/msg/detail/plat_state__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_PlatState_platform_state
{
public:
  Init_PlatState_platform_state()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::msg::PlatState platform_state(::interfaces::msg::PlatState::_platform_state_type arg)
  {
    msg_.platform_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::PlatState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::PlatState>()
{
  return interfaces::msg::builder::Init_PlatState_platform_state();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__PLAT_STATE__BUILDER_HPP_
