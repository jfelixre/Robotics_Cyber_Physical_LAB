// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:srv/RobotVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__ROBOT_VEL__STRUCT_HPP_
#define INTERFACES__SRV__DETAIL__ROBOT_VEL__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__srv__RobotVel_Request __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__srv__RobotVel_Request __declspec(deprecated)
#endif

namespace interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct RobotVel_Request_
{
  using Type = RobotVel_Request_<ContainerAllocator>;

  explicit RobotVel_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x_vel = 0.0f;
      this->y_vel = 0.0f;
      this->ang_vel = 0.0f;
      this->b1_vel = 0.0f;
      this->b2_vel = 0.0f;
      this->b3_vel = 0.0f;
      this->g1_vel = 0.0f;
    }
  }

  explicit RobotVel_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x_vel = 0.0f;
      this->y_vel = 0.0f;
      this->ang_vel = 0.0f;
      this->b1_vel = 0.0f;
      this->b2_vel = 0.0f;
      this->b3_vel = 0.0f;
      this->g1_vel = 0.0f;
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
  using _b1_vel_type =
    float;
  _b1_vel_type b1_vel;
  using _b2_vel_type =
    float;
  _b2_vel_type b2_vel;
  using _b3_vel_type =
    float;
  _b3_vel_type b3_vel;
  using _g1_vel_type =
    float;
  _g1_vel_type g1_vel;

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
  Type & set__b1_vel(
    const float & _arg)
  {
    this->b1_vel = _arg;
    return *this;
  }
  Type & set__b2_vel(
    const float & _arg)
  {
    this->b2_vel = _arg;
    return *this;
  }
  Type & set__b3_vel(
    const float & _arg)
  {
    this->b3_vel = _arg;
    return *this;
  }
  Type & set__g1_vel(
    const float & _arg)
  {
    this->g1_vel = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::srv::RobotVel_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::srv::RobotVel_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::RobotVel_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::RobotVel_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__srv__RobotVel_Request
    std::shared_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__srv__RobotVel_Request
    std::shared_ptr<interfaces::srv::RobotVel_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RobotVel_Request_ & other) const
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
    if (this->b1_vel != other.b1_vel) {
      return false;
    }
    if (this->b2_vel != other.b2_vel) {
      return false;
    }
    if (this->b3_vel != other.b3_vel) {
      return false;
    }
    if (this->g1_vel != other.g1_vel) {
      return false;
    }
    return true;
  }
  bool operator!=(const RobotVel_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RobotVel_Request_

// alias to use template instance with default allocator
using RobotVel_Request =
  interfaces::srv::RobotVel_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace interfaces


#ifndef _WIN32
# define DEPRECATED__interfaces__srv__RobotVel_Response __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__srv__RobotVel_Response __declspec(deprecated)
#endif

namespace interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct RobotVel_Response_
{
  using Type = RobotVel_Response_<ContainerAllocator>;

  explicit RobotVel_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sent = 0ll;
    }
  }

  explicit RobotVel_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    interfaces::srv::RobotVel_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::srv::RobotVel_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::RobotVel_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::RobotVel_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__srv__RobotVel_Response
    std::shared_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__srv__RobotVel_Response
    std::shared_ptr<interfaces::srv::RobotVel_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RobotVel_Response_ & other) const
  {
    if (this->sent != other.sent) {
      return false;
    }
    return true;
  }
  bool operator!=(const RobotVel_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RobotVel_Response_

// alias to use template instance with default allocator
using RobotVel_Response =
  interfaces::srv::RobotVel_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace interfaces

namespace interfaces
{

namespace srv
{

struct RobotVel
{
  using Request = interfaces::srv::RobotVel_Request;
  using Response = interfaces::srv::RobotVel_Response;
};

}  // namespace srv

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__ROBOT_VEL__STRUCT_HPP_
