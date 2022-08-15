// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/MotorVels.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__MotorVels __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__MotorVels __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MotorVels_
{
  using Type = MotorVels_<ContainerAllocator>;

  explicit MotorVels_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->vel_m1 = 0.0f;
      this->vel_m2 = 0.0f;
      this->vel_m3 = 0.0f;
      this->vel_m4 = 0.0f;
    }
  }

  explicit MotorVels_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->vel_m1 = 0.0f;
      this->vel_m2 = 0.0f;
      this->vel_m3 = 0.0f;
      this->vel_m4 = 0.0f;
    }
  }

  // field types and members
  using _vel_m1_type =
    float;
  _vel_m1_type vel_m1;
  using _vel_m2_type =
    float;
  _vel_m2_type vel_m2;
  using _vel_m3_type =
    float;
  _vel_m3_type vel_m3;
  using _vel_m4_type =
    float;
  _vel_m4_type vel_m4;

  // setters for named parameter idiom
  Type & set__vel_m1(
    const float & _arg)
  {
    this->vel_m1 = _arg;
    return *this;
  }
  Type & set__vel_m2(
    const float & _arg)
  {
    this->vel_m2 = _arg;
    return *this;
  }
  Type & set__vel_m3(
    const float & _arg)
  {
    this->vel_m3 = _arg;
    return *this;
  }
  Type & set__vel_m4(
    const float & _arg)
  {
    this->vel_m4 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::MotorVels_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::MotorVels_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::MotorVels_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::MotorVels_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::MotorVels_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::MotorVels_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::MotorVels_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::MotorVels_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::MotorVels_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::MotorVels_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__MotorVels
    std::shared_ptr<interfaces::msg::MotorVels_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__MotorVels
    std::shared_ptr<interfaces::msg::MotorVels_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MotorVels_ & other) const
  {
    if (this->vel_m1 != other.vel_m1) {
      return false;
    }
    if (this->vel_m2 != other.vel_m2) {
      return false;
    }
    if (this->vel_m3 != other.vel_m3) {
      return false;
    }
    if (this->vel_m4 != other.vel_m4) {
      return false;
    }
    return true;
  }
  bool operator!=(const MotorVels_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MotorVels_

// alias to use template instance with default allocator
using MotorVels =
  interfaces::msg::MotorVels_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS__STRUCT_HPP_
