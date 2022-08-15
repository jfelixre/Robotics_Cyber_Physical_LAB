// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/PlatState.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__PLAT_STATE__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__PLAT_STATE__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__PlatState __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__PlatState __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct PlatState_
{
  using Type = PlatState_<ContainerAllocator>;

  explicit PlatState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->platform_state = 0l;
    }
  }

  explicit PlatState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->platform_state = 0l;
    }
  }

  // field types and members
  using _platform_state_type =
    int32_t;
  _platform_state_type platform_state;

  // setters for named parameter idiom
  Type & set__platform_state(
    const int32_t & _arg)
  {
    this->platform_state = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::PlatState_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::PlatState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::PlatState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::PlatState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::PlatState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::PlatState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::PlatState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::PlatState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::PlatState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::PlatState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__PlatState
    std::shared_ptr<interfaces::msg::PlatState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__PlatState
    std::shared_ptr<interfaces::msg::PlatState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlatState_ & other) const
  {
    if (this->platform_state != other.platform_state) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlatState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlatState_

// alias to use template instance with default allocator
using PlatState =
  interfaces::msg::PlatState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__PLAT_STATE__STRUCT_HPP_
