// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/MotorVelsWArm.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__MotorVelsWArm __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__MotorVelsWArm __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MotorVelsWArm_
{
  using Type = MotorVelsWArm_<ContainerAllocator>;

  explicit MotorVelsWArm_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->vel_m1 = 0.0f;
      this->vel_m2 = 0.0f;
      this->vel_m3 = 0.0f;
      this->vel_m4 = 0.0f;
      this->vel_b1 = 0.0f;
      this->vel_b2 = 0.0f;
      this->vel_b3 = 0.0f;
      this->vel_g1 = 0.0f;
    }
  }

  explicit MotorVelsWArm_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->vel_m1 = 0.0f;
      this->vel_m2 = 0.0f;
      this->vel_m3 = 0.0f;
      this->vel_m4 = 0.0f;
      this->vel_b1 = 0.0f;
      this->vel_b2 = 0.0f;
      this->vel_b3 = 0.0f;
      this->vel_g1 = 0.0f;
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
  using _vel_b1_type =
    float;
  _vel_b1_type vel_b1;
  using _vel_b2_type =
    float;
  _vel_b2_type vel_b2;
  using _vel_b3_type =
    float;
  _vel_b3_type vel_b3;
  using _vel_g1_type =
    float;
  _vel_g1_type vel_g1;

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
  Type & set__vel_b1(
    const float & _arg)
  {
    this->vel_b1 = _arg;
    return *this;
  }
  Type & set__vel_b2(
    const float & _arg)
  {
    this->vel_b2 = _arg;
    return *this;
  }
  Type & set__vel_b3(
    const float & _arg)
  {
    this->vel_b3 = _arg;
    return *this;
  }
  Type & set__vel_g1(
    const float & _arg)
  {
    this->vel_g1 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::MotorVelsWArm_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::MotorVelsWArm_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::MotorVelsWArm_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::MotorVelsWArm_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__MotorVelsWArm
    std::shared_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__MotorVelsWArm
    std::shared_ptr<interfaces::msg::MotorVelsWArm_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MotorVelsWArm_ & other) const
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
    if (this->vel_b1 != other.vel_b1) {
      return false;
    }
    if (this->vel_b2 != other.vel_b2) {
      return false;
    }
    if (this->vel_b3 != other.vel_b3) {
      return false;
    }
    if (this->vel_g1 != other.vel_g1) {
      return false;
    }
    return true;
  }
  bool operator!=(const MotorVelsWArm_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MotorVelsWArm_

// alias to use template instance with default allocator
using MotorVelsWArm =
  interfaces::msg::MotorVelsWArm_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__STRUCT_HPP_
