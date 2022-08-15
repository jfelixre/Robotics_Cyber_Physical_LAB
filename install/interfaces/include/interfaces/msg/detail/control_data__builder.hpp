// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__CONTROL_DATA__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__CONTROL_DATA__BUILDER_HPP_

#include "interfaces/msg/detail/control_data__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_ControlData_vel_ang_robot
{
public:
  explicit Init_ControlData_vel_ang_robot(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::ControlData vel_ang_robot(::interfaces::msg::ControlData::_vel_ang_robot_type arg)
  {
    msg_.vel_ang_robot = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_vel_y_robot
{
public:
  explicit Init_ControlData_vel_y_robot(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_vel_ang_robot vel_y_robot(::interfaces::msg::ControlData::_vel_y_robot_type arg)
  {
    msg_.vel_y_robot = std::move(arg);
    return Init_ControlData_vel_ang_robot(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_vel_x_robot
{
public:
  explicit Init_ControlData_vel_x_robot(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_vel_y_robot vel_x_robot(::interfaces::msg::ControlData::_vel_x_robot_type arg)
  {
    msg_.vel_x_robot = std::move(arg);
    return Init_ControlData_vel_y_robot(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_vel_ang_world
{
public:
  explicit Init_ControlData_vel_ang_world(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_vel_x_robot vel_ang_world(::interfaces::msg::ControlData::_vel_ang_world_type arg)
  {
    msg_.vel_ang_world = std::move(arg);
    return Init_ControlData_vel_x_robot(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_vel_y_world
{
public:
  explicit Init_ControlData_vel_y_world(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_vel_ang_world vel_y_world(::interfaces::msg::ControlData::_vel_y_world_type arg)
  {
    msg_.vel_y_world = std::move(arg);
    return Init_ControlData_vel_ang_world(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_vel_x_world
{
public:
  explicit Init_ControlData_vel_x_world(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_vel_y_world vel_x_world(::interfaces::msg::ControlData::_vel_x_world_type arg)
  {
    msg_.vel_x_world = std::move(arg);
    return Init_ControlData_vel_y_world(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_ang_error
{
public:
  explicit Init_ControlData_ang_error(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_vel_x_world ang_error(::interfaces::msg::ControlData::_ang_error_type arg)
  {
    msg_.ang_error = std::move(arg);
    return Init_ControlData_vel_x_world(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_y_error
{
public:
  explicit Init_ControlData_y_error(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_ang_error y_error(::interfaces::msg::ControlData::_y_error_type arg)
  {
    msg_.y_error = std::move(arg);
    return Init_ControlData_ang_error(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_x_error
{
public:
  explicit Init_ControlData_x_error(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_y_error x_error(::interfaces::msg::ControlData::_x_error_type arg)
  {
    msg_.x_error = std::move(arg);
    return Init_ControlData_y_error(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_ang_goal
{
public:
  explicit Init_ControlData_ang_goal(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_x_error ang_goal(::interfaces::msg::ControlData::_ang_goal_type arg)
  {
    msg_.ang_goal = std::move(arg);
    return Init_ControlData_x_error(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_y_goal
{
public:
  explicit Init_ControlData_y_goal(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_ang_goal y_goal(::interfaces::msg::ControlData::_y_goal_type arg)
  {
    msg_.y_goal = std::move(arg);
    return Init_ControlData_ang_goal(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_x_goal
{
public:
  explicit Init_ControlData_x_goal(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_y_goal x_goal(::interfaces::msg::ControlData::_x_goal_type arg)
  {
    msg_.x_goal = std::move(arg);
    return Init_ControlData_y_goal(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_ang_tgt
{
public:
  explicit Init_ControlData_ang_tgt(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_x_goal ang_tgt(::interfaces::msg::ControlData::_ang_tgt_type arg)
  {
    msg_.ang_tgt = std::move(arg);
    return Init_ControlData_x_goal(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_y_tgt
{
public:
  explicit Init_ControlData_y_tgt(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_ang_tgt y_tgt(::interfaces::msg::ControlData::_y_tgt_type arg)
  {
    msg_.y_tgt = std::move(arg);
    return Init_ControlData_ang_tgt(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_x_tgt
{
public:
  explicit Init_ControlData_x_tgt(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_y_tgt x_tgt(::interfaces::msg::ControlData::_x_tgt_type arg)
  {
    msg_.x_tgt = std::move(arg);
    return Init_ControlData_y_tgt(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_ang_obj
{
public:
  explicit Init_ControlData_ang_obj(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_x_tgt ang_obj(::interfaces::msg::ControlData::_ang_obj_type arg)
  {
    msg_.ang_obj = std::move(arg);
    return Init_ControlData_x_tgt(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_y_obj
{
public:
  explicit Init_ControlData_y_obj(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_ang_obj y_obj(::interfaces::msg::ControlData::_y_obj_type arg)
  {
    msg_.y_obj = std::move(arg);
    return Init_ControlData_ang_obj(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_x_obj
{
public:
  explicit Init_ControlData_x_obj(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_y_obj x_obj(::interfaces::msg::ControlData::_x_obj_type arg)
  {
    msg_.x_obj = std::move(arg);
    return Init_ControlData_y_obj(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_ang_robot
{
public:
  explicit Init_ControlData_ang_robot(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_x_obj ang_robot(::interfaces::msg::ControlData::_ang_robot_type arg)
  {
    msg_.ang_robot = std::move(arg);
    return Init_ControlData_x_obj(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_y_robot
{
public:
  explicit Init_ControlData_y_robot(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_ang_robot y_robot(::interfaces::msg::ControlData::_y_robot_type arg)
  {
    msg_.y_robot = std::move(arg);
    return Init_ControlData_ang_robot(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_x_robot
{
public:
  explicit Init_ControlData_x_robot(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_y_robot x_robot(::interfaces::msg::ControlData::_x_robot_type arg)
  {
    msg_.x_robot = std::move(arg);
    return Init_ControlData_y_robot(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_fase
{
public:
  explicit Init_ControlData_fase(::interfaces::msg::ControlData & msg)
  : msg_(msg)
  {}
  Init_ControlData_x_robot fase(::interfaces::msg::ControlData::_fase_type arg)
  {
    msg_.fase = std::move(arg);
    return Init_ControlData_x_robot(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

class Init_ControlData_time
{
public:
  Init_ControlData_time()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControlData_fase time(::interfaces::msg::ControlData::_time_type arg)
  {
    msg_.time = std::move(arg);
    return Init_ControlData_fase(msg_);
  }

private:
  ::interfaces::msg::ControlData msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::ControlData>()
{
  return interfaces::msg::builder::Init_ControlData_time();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__CONTROL_DATA__BUILDER_HPP_
