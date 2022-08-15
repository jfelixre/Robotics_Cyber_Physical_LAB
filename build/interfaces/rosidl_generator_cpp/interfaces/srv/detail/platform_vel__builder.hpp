// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:srv/PlatformVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__PLATFORM_VEL__BUILDER_HPP_
#define INTERFACES__SRV__DETAIL__PLATFORM_VEL__BUILDER_HPP_

#include "interfaces/srv/detail/platform_vel__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_PlatformVel_Request_ang_vel
{
public:
  explicit Init_PlatformVel_Request_ang_vel(::interfaces::srv::PlatformVel_Request & msg)
  : msg_(msg)
  {}
  ::interfaces::srv::PlatformVel_Request ang_vel(::interfaces::srv::PlatformVel_Request::_ang_vel_type arg)
  {
    msg_.ang_vel = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::PlatformVel_Request msg_;
};

class Init_PlatformVel_Request_y_vel
{
public:
  explicit Init_PlatformVel_Request_y_vel(::interfaces::srv::PlatformVel_Request & msg)
  : msg_(msg)
  {}
  Init_PlatformVel_Request_ang_vel y_vel(::interfaces::srv::PlatformVel_Request::_y_vel_type arg)
  {
    msg_.y_vel = std::move(arg);
    return Init_PlatformVel_Request_ang_vel(msg_);
  }

private:
  ::interfaces::srv::PlatformVel_Request msg_;
};

class Init_PlatformVel_Request_x_vel
{
public:
  Init_PlatformVel_Request_x_vel()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PlatformVel_Request_y_vel x_vel(::interfaces::srv::PlatformVel_Request::_x_vel_type arg)
  {
    msg_.x_vel = std::move(arg);
    return Init_PlatformVel_Request_y_vel(msg_);
  }

private:
  ::interfaces::srv::PlatformVel_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::PlatformVel_Request>()
{
  return interfaces::srv::builder::Init_PlatformVel_Request_x_vel();
}

}  // namespace interfaces


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_PlatformVel_Response_sent
{
public:
  Init_PlatformVel_Response_sent()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::srv::PlatformVel_Response sent(::interfaces::srv::PlatformVel_Response::_sent_type arg)
  {
    msg_.sent = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::PlatformVel_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::PlatformVel_Response>()
{
  return interfaces::srv::builder::Init_PlatformVel_Response_sent();
}

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__PLATFORM_VEL__BUILDER_HPP_
