// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:srv/ControlPidStart.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__CONTROL_PID_START__STRUCT_HPP_
#define INTERFACES__SRV__DETAIL__CONTROL_PID_START__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__srv__ControlPidStart_Request __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__srv__ControlPidStart_Request __declspec(deprecated)
#endif

namespace interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ControlPidStart_Request_
{
  using Type = ControlPidStart_Request_<ContainerAllocator>;

  explicit ControlPidStart_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->fase = 0ll;
    }
  }

  explicit ControlPidStart_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->fase = 0ll;
    }
  }

  // field types and members
  using _fase_type =
    int64_t;
  _fase_type fase;

  // setters for named parameter idiom
  Type & set__fase(
    const int64_t & _arg)
  {
    this->fase = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::srv::ControlPidStart_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::srv::ControlPidStart_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::ControlPidStart_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::ControlPidStart_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__srv__ControlPidStart_Request
    std::shared_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__srv__ControlPidStart_Request
    std::shared_ptr<interfaces::srv::ControlPidStart_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ControlPidStart_Request_ & other) const
  {
    if (this->fase != other.fase) {
      return false;
    }
    return true;
  }
  bool operator!=(const ControlPidStart_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ControlPidStart_Request_

// alias to use template instance with default allocator
using ControlPidStart_Request =
  interfaces::srv::ControlPidStart_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace interfaces


#ifndef _WIN32
# define DEPRECATED__interfaces__srv__ControlPidStart_Response __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__srv__ControlPidStart_Response __declspec(deprecated)
#endif

namespace interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ControlPidStart_Response_
{
  using Type = ControlPidStart_Response_<ContainerAllocator>;

  explicit ControlPidStart_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->finish = 0ll;
    }
  }

  explicit ControlPidStart_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->finish = 0ll;
    }
  }

  // field types and members
  using _finish_type =
    int64_t;
  _finish_type finish;

  // setters for named parameter idiom
  Type & set__finish(
    const int64_t & _arg)
  {
    this->finish = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::srv::ControlPidStart_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::srv::ControlPidStart_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::ControlPidStart_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::srv::ControlPidStart_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__srv__ControlPidStart_Response
    std::shared_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__srv__ControlPidStart_Response
    std::shared_ptr<interfaces::srv::ControlPidStart_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ControlPidStart_Response_ & other) const
  {
    if (this->finish != other.finish) {
      return false;
    }
    return true;
  }
  bool operator!=(const ControlPidStart_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ControlPidStart_Response_

// alias to use template instance with default allocator
using ControlPidStart_Response =
  interfaces::srv::ControlPidStart_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace interfaces

namespace interfaces
{

namespace srv
{

struct ControlPidStart
{
  using Request = interfaces::srv::ControlPidStart_Request;
  using Response = interfaces::srv::ControlPidStart_Response;
};

}  // namespace srv

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__CONTROL_PID_START__STRUCT_HPP_
