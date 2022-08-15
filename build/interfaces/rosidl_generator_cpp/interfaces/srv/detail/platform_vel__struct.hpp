// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:srv/PlatformVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__PLATFORM_VEL__STRUCT_HPP_
#define INTERFACES__SRV__DETAIL__PLATFORM_VEL__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__srv__PlatformVel_Request __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__srv__PlatformVel_Request __declspec(deprecated)
#endif

namespace interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct PlatformVel_Request_
{
  using Type = PlatformVel_Request_<ContainerAllocator>;

  explicit PlatformVel_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x_vel = 0.0f;
      this->y_vel = 0.0f;
      this->ang_vel = 0.0f;
    }
  }

  explicit PlatformVel_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x_vel = 0.0f;
      this->y_vel = 0.0f;
      this->ang_vel = 0.0f;
    }
  }

  // field types and members
  using _x_vel_type =
    float;
  _x_vel_type x_vel;
  using _y_vel_type =
    float;
  _y_vel_type y_vel;
  using _ang_vel_type =
    float;
  _ang_vel_type ang_vel;

  // setters for named parameter idiom
  Type & set__x_vel(
    const float & _arg)
  {
    this->x_vel = _arg;
    return *this;
  }
  Type & set__y_vel(
    const float & _arg)
  {
    this->y_vel = _arg;
    return *this;
  }
  Type & set__ang_vel(
    const float & _arg)
  {
    this->ang_vel = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::srv::PlatformVel_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::srv::PlatformVel_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::PlatformVel_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::PlatformVel_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__srv__PlatformVel_Request
    std::shared_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__srv__PlatformVel_Request
    std::shared_ptr<interfaces::srv::PlatformVel_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlatformVel_Request_ & other) const
  {
    if (this->x_vel != other.x_vel) {
      return false;
    }
    if (this->y_vel != other.y_vel) {
      return false;
    }
    if (this->ang_vel != other.ang_vel) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlatformVel_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlatformVel_Request_

// alias to use template instance with default allocator
using PlatformVel_Request =
  interfaces::srv::PlatformVel_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace interfaces


#ifndef _WIN32
# define DEPRECATED__interfaces__srv__PlatformVel_Response __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__srv__PlatformVel_Response __declspec(deprecated)
#endif

namespace interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct PlatformVel_Response_
{
  using Type = PlatformVel_Response_<ContainerAllocator>;

  explicit PlatformVel_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sent = 0ll;
    }
  }

  explicit PlatformVel_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sent = 0ll;
    }
  }

  // field types and members
  using _sent_type =
    int64_t;
  _sent_type sent;

  // setters for named parameter idiom
  Type & set__sent(
    const int64_t & _arg)
  {
    this->sent = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::srv::PlatformVel_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::srv::PlatformVel_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::PlatformVel_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::PlatformVel_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__srv__PlatformVel_Response
    std::shared_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__srv__PlatformVel_Response
    std::shared_ptr<interfaces::srv::PlatformVel_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlatformVel_Response_ & other) const
  {
    if (this->sent != other.sent) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlatformVel_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlatformVel_Response_

// alias to use template instance with default allocator
using PlatformVel_Response =
  interfaces::srv::PlatformVel_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace interfaces

namespace interfaces
{

namespace srv
{

struct PlatformVel
{
  using Request = interfaces::srv::PlatformVel_Request;
  using Response = interfaces::srv::PlatformVel_Response;
};

}  // namespace srv

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__PLATFORM_VEL__STRUCT_HPP_
