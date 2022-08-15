// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/ImgData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__IMG_DATA__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__IMG_DATA__BUILDER_HPP_

#include "interfaces/msg/detail/img_data__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_ImgData_tgt_ang
{
public:
  explicit Init_ImgData_tgt_ang(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::ImgData tgt_ang(::interfaces::msg::ImgData::_tgt_ang_type arg)
  {
    msg_.tgt_ang = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_tgt_y
{
public:
  explicit Init_ImgData_tgt_y(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_tgt_ang tgt_y(::interfaces::msg::ImgData::_tgt_y_type arg)
  {
    msg_.tgt_y = std::move(arg);
    return Init_ImgData_tgt_ang(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_tgt_x
{
public:
  explicit Init_ImgData_tgt_x(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_tgt_y tgt_x(::interfaces::msg::ImgData::_tgt_x_type arg)
  {
    msg_.tgt_x = std::move(arg);
    return Init_ImgData_tgt_y(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_obj2_ang
{
public:
  explicit Init_ImgData_obj2_ang(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_tgt_x obj2_ang(::interfaces::msg::ImgData::_obj2_ang_type arg)
  {
    msg_.obj2_ang = std::move(arg);
    return Init_ImgData_tgt_x(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_obj2_y
{
public:
  explicit Init_ImgData_obj2_y(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_obj2_ang obj2_y(::interfaces::msg::ImgData::_obj2_y_type arg)
  {
    msg_.obj2_y = std::move(arg);
    return Init_ImgData_obj2_ang(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_obj2_x
{
public:
  explicit Init_ImgData_obj2_x(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_obj2_y obj2_x(::interfaces::msg::ImgData::_obj2_x_type arg)
  {
    msg_.obj2_x = std::move(arg);
    return Init_ImgData_obj2_y(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_obj1_ang
{
public:
  explicit Init_ImgData_obj1_ang(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_obj2_x obj1_ang(::interfaces::msg::ImgData::_obj1_ang_type arg)
  {
    msg_.obj1_ang = std::move(arg);
    return Init_ImgData_obj2_x(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_obj1_y
{
public:
  explicit Init_ImgData_obj1_y(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_obj1_ang obj1_y(::interfaces::msg::ImgData::_obj1_y_type arg)
  {
    msg_.obj1_y = std::move(arg);
    return Init_ImgData_obj1_ang(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_obj1_x
{
public:
  explicit Init_ImgData_obj1_x(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_obj1_y obj1_x(::interfaces::msg::ImgData::_obj1_x_type arg)
  {
    msg_.obj1_x = std::move(arg);
    return Init_ImgData_obj1_y(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_r2_ang
{
public:
  explicit Init_ImgData_r2_ang(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_obj1_x r2_ang(::interfaces::msg::ImgData::_r2_ang_type arg)
  {
    msg_.r2_ang = std::move(arg);
    return Init_ImgData_obj1_x(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_r2_y
{
public:
  explicit Init_ImgData_r2_y(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_r2_ang r2_y(::interfaces::msg::ImgData::_r2_y_type arg)
  {
    msg_.r2_y = std::move(arg);
    return Init_ImgData_r2_ang(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_r2_x
{
public:
  explicit Init_ImgData_r2_x(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_r2_y r2_x(::interfaces::msg::ImgData::_r2_x_type arg)
  {
    msg_.r2_x = std::move(arg);
    return Init_ImgData_r2_y(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_r1_ang
{
public:
  explicit Init_ImgData_r1_ang(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_r2_x r1_ang(::interfaces::msg::ImgData::_r1_ang_type arg)
  {
    msg_.r1_ang = std::move(arg);
    return Init_ImgData_r2_x(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_r1_y
{
public:
  explicit Init_ImgData_r1_y(::interfaces::msg::ImgData & msg)
  : msg_(msg)
  {}
  Init_ImgData_r1_ang r1_y(::interfaces::msg::ImgData::_r1_y_type arg)
  {
    msg_.r1_y = std::move(arg);
    return Init_ImgData_r1_ang(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

class Init_ImgData_r1_x
{
public:
  Init_ImgData_r1_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ImgData_r1_y r1_x(::interfaces::msg::ImgData::_r1_x_type arg)
  {
    msg_.r1_x = std::move(arg);
    return Init_ImgData_r1_y(msg_);
  }

private:
  ::interfaces::msg::ImgData msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::ImgData>()
{
  return interfaces::msg::builder::Init_ImgData_r1_x();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__IMG_DATA__BUILDER_HPP_
