// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/LimitSwitch.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__LIMIT_SWITCH__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__LIMIT_SWITCH__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__LimitSwitch __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__LimitSwitch __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LimitSwitch_
{
  using Type = LimitSwitch_<ContainerAllocator>;

  explicit LimitSwitch_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->b1_min = 0l;
      this->b1_max = 0l;
      this->b2_min = 0l;
      this->b2_max = 0l;
      this->b3_min = 0l;
      this->b3_max = 0l;
    }
  }

  explicit LimitSwitch_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->b1_min = 0l;
      this->b1_max = 0l;
      this->b2_min = 0l;
      this->b2_max = 0l;
      this->b3_min = 0l;
      this->b3_max = 0l;
    }
  }

  // field types and members
  using _b1_min_type =
    int32_t;
  _b1_min_type b1_min;
  using _b1_max_type =
    int32_t;
  _b1_max_type b1_max;
  using _b2_min_type =
    int32_t;
  _b2_min_type b2_min;
  using _b2_max_type =
    int32_t;
  _b2_max_type b2_max;
  using _b3_min_type =
    int32_t;
  _b3_min_type b3_min;
  using _b3_max_type =
    int32_t;
  _b3_max_type b3_max;

  // setters for named parameter idiom
  Type & set__b1_min(
    const int32_t & _arg)
  {
    this->b1_min = _arg;
    return *this;
  }
  Type & set__b1_max(
    const int32_t & _arg)
  {
    this->b1_max = _arg;
    return *this;
  }
  Type & set__b2_min(
    const int32_t & _arg)
  {
    this->b2_min = _arg;
    return *this;
  }
  Type & set__b2_max(
    const int32_t & _arg)
  {
    this->b2_max = _arg;
    return *this;
  }
  Type & set__b3_min(
    const int32_t & _arg)
  {
    this->b3_min = _arg;
    return *this;
  }
  Type & set__b3_max(
    const int32_t & _arg)
  {
    this->b3_max = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::LimitSwitch_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::LimitSwitch_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::LimitSwitch_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::LimitSwitch_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__LimitSwitch
    std::shared_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__LimitSwitch
    std::shared_ptr<interfaces::msg::LimitSwitch_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LimitSwitch_ & other) const
  {
    if (this->b1_min != other.b1_min) {
      return false;
    }
    if (this->b1_max != other.b1_max) {
      return false;
    }
    if (this->b2_min != other.b2_min) {
      return false;
    }
    if (this->b2_max != other.b2_max) {
      return false;
    }
    if (this->b3_min != other.b3_min) {
      return false;
    }
    if (this->b3_max != other.b3_max) {
      return false;
    }
    return true;
  }
  bool operator!=(const LimitSwitch_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LimitSwitch_

// alias to use template instance with default allocator
using LimitSwitch =
  interfaces::msg::LimitSwitch_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__LIMIT_SWITCH__STRUCT_HPP_
