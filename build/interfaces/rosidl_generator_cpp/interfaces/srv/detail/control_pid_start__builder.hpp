// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:srv/ControlPidStart.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__CONTROL_PID_START__BUILDER_HPP_
#define INTERFACES__SRV__DETAIL__CONTROL_PID_START__BUILDER_HPP_

#include "interfaces/srv/detail/control_pid_start__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_ControlPidStart_Request_fase
{
public:
  Init_ControlPidStart_Request_fase()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::srv::ControlPidStart_Request fase(::interfaces::srv::ControlPidStart_Request::_fase_type arg)
  {
    msg_.fase = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::ControlPidStart_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::ControlPidStart_Request>()
{
  return interfaces::srv::builder::Init_ControlPidStart_Request_fase();
}

}  // namespace interfaces


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_ControlPidStart_Response_finish
{
public:
  Init_ControlPidStart_Response_finish()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::srv::ControlPidStart_Response finish(::interfaces::srv::ControlPidStart_Response::_finish_type arg)
  {
    msg_.finish = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::ControlPidStart_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::ControlPidStart_Response>()
{
  return interfaces::srv::builder::Init_ControlPidStart_Response_finish();
}

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__CONTROL_PID_START__BUILDER_HPP_
