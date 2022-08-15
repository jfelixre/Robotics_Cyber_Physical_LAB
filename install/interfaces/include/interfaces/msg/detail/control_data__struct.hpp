// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__CONTROL_DATA__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__CONTROL_DATA__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__ControlData __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__ControlData __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ControlData_
{
  using Type = ControlData_<ContainerAllocator>;

  explicit ControlData_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->time = 0.0f;
      this->fase = 0l;
      this->x_robot = 0l;
      this->y_robot = 0l;
      this->ang_robot = 0.0f;
      this->x_obj = 0l;
      this->y_obj = 0l;
      this->ang_obj = 0.0f;
      this->x_tgt = 0l;
      this->y_tgt = 0l;
      this->ang_tgt = 0.0f;
      this->x_goal = 0l;
      this->y_goal = 0l;
      this->ang_goal = 0.0f;
      this->x_error = 0l;
      this->y_error = 0l;
      this->ang_error = 0.0f;
      this->vel_x_world = 0.0f;
      this->vel_y_world = 0.0f;
      this->vel_ang_world = 0.0f;
      this->vel_x_robot = 0.0f;
      this->vel_y_robot = 0.0f;
      this->vel_ang_robot = 0.0f;
    }
  }

  explicit ControlData_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->time = 0.0f;
      this->fase = 0l;
      this->x_robot = 0l;
      this->y_robot = 0l;
      this->ang_robot = 0.0f;
      this->x_obj = 0l;
      this->y_obj = 0l;
      this->ang_obj = 0.0f;
      this->x_tgt = 0l;
      this->y_tgt = 0l;
      this->ang_tgt = 0.0f;
      this->x_goal = 0l;
      this->y_goal = 0l;
      this->ang_goal = 0.0f;
      this->x_error = 0l;
      this->y_error = 0l;
      this->ang_error = 0.0f;
      this->vel_x_world = 0.0f;
      this->vel_y_world = 0.0f;
      this->vel_ang_world = 0.0f;
      this->vel_x_robot = 0.0f;
      this->vel_y_robot = 0.0f;
      this->vel_ang_robot = 0.0f;
    }
  }

  // field types and members
  using _time_type =
    float;
  _time_type time;
  using _fase_type =
    int32_t;
  _fase_type fase;
  using _x_robot_type =
    int32_t;
  _x_robot_type x_robot;
  using _y_robot_type =
    int32_t;
  _y_robot_type y_robot;
  using _ang_robot_type =
    float;
  _ang_robot_type ang_robot;
  using _x_obj_type =
    int32_t;
  _x_obj_type x_obj;
  using _y_obj_type =
    int32_t;
  _y_obj_type y_obj;
  using _ang_obj_type =
    float;
  _ang_obj_type ang_obj;
  using _x_tgt_type =
    int32_t;
  _x_tgt_type x_tgt;
  using _y_tgt_type =
    int32_t;
  _y_tgt_type y_tgt;
  using _ang_tgt_type =
    float;
  _ang_tgt_type ang_tgt;
  using _x_goal_type =
    int32_t;
  _x_goal_type x_goal;
  using _y_goal_type =
    int32_t;
  _y_goal_type y_goal;
  using _ang_goal_type =
    float;
  _ang_goal_type ang_goal;
  using _x_error_type =
    int32_t;
  _x_error_type x_error;
  using _y_error_type =
    int32_t;
  _y_error_type y_error;
  using _ang_error_type =
    float;
  _ang_error_type ang_error;
  using _vel_x_world_type =
    float;
  _vel_x_world_type vel_x_world;
  using _vel_y_world_type =
    float;
  _vel_y_world_type vel_y_world;
  using _vel_ang_world_type =
    float;
  _vel_ang_world_type vel_ang_world;
  using _vel_x_robot_type =
    float;
  _vel_x_robot_type vel_x_robot;
  using _vel_y_robot_type =
    float;
  _vel_y_robot_type vel_y_robot;
  using _vel_ang_robot_type =
    float;
  _vel_ang_robot_type vel_ang_robot;

  // setters for named parameter idiom
  Type & set__time(
    const float & _arg)
  {
    this->time = _arg;
    return *this;
  }
  Type & set__fase(
    const int32_t & _arg)
  {
    this->fase = _arg;
    return *this;
  }
  Type & set__x_robot(
    const int32_t & _arg)
  {
    this->x_robot = _arg;
    return *this;
  }
  Type & set__y_robot(
    const int32_t & _arg)
  {
    this->y_robot = _arg;
    return *this;
  }
  Type & set__ang_robot(
    const float & _arg)
  {
    this->ang_robot = _arg;
    return *this;
  }
  Type & set__x_obj(
    const int32_t & _arg)
  {
    this->x_obj = _arg;
    return *this;
  }
  Type & set__y_obj(
    const int32_t & _arg)
  {
    this->y_obj = _arg;
    return *this;
  }
  Type & set__ang_obj(
    const float & _arg)
  {
    this->ang_obj = _arg;
    return *this;
  }
  Type & set__x_tgt(
    const int32_t & _arg)
  {
    this->x_tgt = _arg;
    return *this;
  }
  Type & set__y_tgt(
    const int32_t & _arg)
  {
    this->y_tgt = _arg;
    return *this;
  }
  Type & set__ang_tgt(
    const float & _arg)
  {
    this->ang_tgt = _arg;
    return *this;
  }
  Type & set__x_goal(
    const int32_t & _arg)
  {
    this->x_goal = _arg;
    return *this;
  }
  Type & set__y_goal(
    const int32_t & _arg)
  {
    this->y_goal = _arg;
    return *this;
  }
  Type & set__ang_goal(
    const float & _arg)
  {
    this->ang_goal = _arg;
    return *this;
  }
  Type & set__x_error(
    const int32_t & _arg)
  {
    this->x_error = _arg;
    return *this;
  }
  Type & set__y_error(
    const int32_t & _arg)
  {
    this->y_error = _arg;
    return *this;
  }
  Type & set__ang_error(
    const float & _arg)
  {
    this->ang_error = _arg;
    return *this;
  }
  Type & set__vel_x_world(
    const float & _arg)
  {
    this->vel_x_world = _arg;
    return *this;
  }
  Type & set__vel_y_world(
    const float & _arg)
  {
    this->vel_y_world = _arg;
    return *this;
  }
  Type & set__vel_ang_world(
    const float & _arg)
  {
    this->vel_ang_world = _arg;
    return *this;
  }
  Type & set__vel_x_robot(
    const float & _arg)
  {
    this->vel_x_robot = _arg;
    return *this;
  }
  Type & set__vel_y_robot(
    const float & _arg)
  {
    this->vel_y_robot = _arg;
    return *this;
  }
  Type & set__vel_ang_robot(
    const float & _arg)
  {
    this->vel_ang_robot = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::ControlData_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::ControlData_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::ControlData_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::ControlData_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::ControlData_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::ControlData_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::ControlData_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::ControlData_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::ControlData_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::ControlData_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__ControlData
    std::shared_ptr<interfaces::msg::ControlData_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__ControlData
    std::shared_ptr<interfaces::msg::ControlData_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ControlData_ & other) const
  {
    if (this->time != other.time) {
      return false;
    }
    if (this->fase != other.fase) {
      return false;
    }
    if (this->x_robot != other.x_robot) {
      return false;
    }
    if (this->y_robot != other.y_robot) {
      return false;
    }
    if (this->ang_robot != other.ang_robot) {
      return false;
    }
    if (this->x_obj != other.x_obj) {
      return false;
    }
    if (this->y_obj != other.y_obj) {
      return false;
    }
    if (this->ang_obj != other.ang_obj) {
      return false;
    }
    if (this->x_tgt != other.x_tgt) {
      return false;
    }
    if (this->y_tgt != other.y_tgt) {
      return false;
    }
    if (this->ang_tgt != other.ang_tgt) {
      return false;
    }
    if (this->x_goal != other.x_goal) {
      return false;
    }
    if (this->y_goal != other.y_goal) {
      return false;
    }
    if (this->ang_goal != other.ang_goal) {
      return false;
    }
    if (this->x_error != other.x_error) {
      return false;
    }
    if (this->y_error != other.y_error) {
      return false;
    }
    if (this->ang_error != other.ang_error) {
      return false;
    }
    if (this->vel_x_world != other.vel_x_world) {
      return false;
    }
    if (this->vel_y_world != other.vel_y_world) {
      return false;
    }
    if (this->vel_ang_world != other.vel_ang_world) {
      return false;
    }
    if (this->vel_x_robot != other.vel_x_robot) {
      return false;
    }
    if (this->vel_y_robot != other.vel_y_robot) {
      return false;
    }
    if (this->vel_ang_robot != other.vel_ang_robot) {
      return false;
    }
    return true;
  }
  bool operator!=(const ControlData_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ControlData_

// alias to use template instance with default allocator
using ControlData =
  interfaces::msg::ControlData_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__CONTROL_DATA__STRUCT_HPP_
