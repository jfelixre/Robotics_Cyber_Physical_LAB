// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:srv/RobotVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__ROBOT_VEL__BUILDER_HPP_
#define INTERFACES__SRV__DETAIL__ROBOT_VEL__BUILDER_HPP_

#include "interfaces/srv/detail/robot_vel__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_RobotVel_Request_g1_vel
{
public:
  explicit Init_RobotVel_Request_g1_vel(::interfaces::srv::RobotVel_Request & msg)
  : msg_(msg)
  {}
  ::interfaces::srv::RobotVel_Request g1_vel(::interfaces::srv::RobotVel_Request::_g1_vel_type arg)
  {
    msg_.g1_vel = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

class Init_RobotVel_Request_b3_vel
{
public:
  explicit Init_RobotVel_Request_b3_vel(::interfaces::srv::RobotVel_Request & msg)
  : msg_(msg)
  {}
  Init_RobotVel_Request_g1_vel b3_vel(::interfaces::srv::RobotVel_Request::_b3_vel_type arg)
  {
    msg_.b3_vel = std::move(arg);
    return Init_RobotVel_Request_g1_vel(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

class Init_RobotVel_Request_b2_vel
{
public:
  explicit Init_RobotVel_Request_b2_vel(::interfaces::srv::RobotVel_Request & msg)
  : msg_(msg)
  {}
  Init_RobotVel_Request_b3_vel b2_vel(::interfaces::srv::RobotVel_Request::_b2_vel_type arg)
  {
    msg_.b2_vel = std::move(arg);
    return Init_RobotVel_Request_b3_vel(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

class Init_RobotVel_Request_b1_vel
{
public:
  explicit Init_RobotVel_Request_b1_vel(::interfaces::srv::RobotVel_Request & msg)
  : msg_(msg)
  {}
  Init_RobotVel_Request_b2_vel b1_vel(::interfaces::srv::RobotVel_Request::_b1_vel_type arg)
  {
    msg_.b1_vel = std::move(arg);
    return Init_RobotVel_Request_b2_vel(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

class Init_RobotVel_Request_ang_vel
{
public:
  explicit Init_RobotVel_Request_ang_vel(::interfaces::srv::RobotVel_Request & msg)
  : msg_(msg)
  {}
  Init_RobotVel_Request_b1_vel ang_vel(::interfaces::srv::RobotVel_Request::_ang_vel_type arg)
  {
    msg_.ang_vel = std::move(arg);
    return Init_RobotVel_Request_b1_vel(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

class Init_RobotVel_Request_y_vel
{
public:
  explicit Init_RobotVel_Request_y_vel(::interfaces::srv::RobotVel_Request & msg)
  : msg_(msg)
  {}
  Init_RobotVel_Request_ang_vel y_vel(::interfaces::srv::RobotVel_Request::_y_vel_type arg)
  {
    msg_.y_vel = std::move(arg);
    return Init_RobotVel_Request_ang_vel(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

class Init_RobotVel_Request_x_vel
{
public:
  Init_RobotVel_Request_x_vel()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotVel_Request_y_vel x_vel(::interfaces::srv::RobotVel_Request::_x_vel_type arg)
  {
    msg_.x_vel = std::move(arg);
    return Init_RobotVel_Request_y_vel(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::RobotVel_Request>()
{
  return interfaces::srv::builder::Init_RobotVel_Request_x_vel();
}

}  // namespace interfaces


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_RobotVel_Response_sent
{
public:
  Init_RobotVel_Response_sent()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::srv::RobotVel_Response sent(::interfaces::srv::RobotVel_Response::_sent_type arg)
  {
    msg_.sent = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::RobotVel_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::RobotVel_Response>()
{
  return interfaces::srv::builder::Init_RobotVel_Response_sent();
}

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__ROBOT_VEL__BUILDER_HPP_
