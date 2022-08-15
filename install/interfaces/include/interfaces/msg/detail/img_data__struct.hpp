// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/ImgData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__IMG_DATA__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__IMG_DATA__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__ImgData __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__ImgData __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ImgData_
{
  using Type = ImgData_<ContainerAllocator>;

  explicit ImgData_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->r1_x = 0l;
      this->r1_y = 0l;
      this->r1_ang = 0.0f;
      this->r2_x = 0l;
      this->r2_y = 0l;
      this->r2_ang = 0.0f;
      this->obj1_x = 0l;
      this->obj1_y = 0l;
      this->obj1_ang = 0.0f;
      this->obj2_x = 0l;
      this->obj2_y = 0l;
      this->obj2_ang = 0.0f;
      this->tgt_x = 0l;
      this->tgt_y = 0l;
      this->tgt_ang = 0.0f;
    }
  }

  explicit ImgData_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->r1_x = 0l;
      this->r1_y = 0l;
      this->r1_ang = 0.0f;
      this->r2_x = 0l;
      this->r2_y = 0l;
      this->r2_ang = 0.0f;
      this->obj1_x = 0l;
      this->obj1_y = 0l;
      this->obj1_ang = 0.0f;
      this->obj2_x = 0l;
      this->obj2_y = 0l;
      this->obj2_ang = 0.0f;
      this->tgt_x = 0l;
      this->tgt_y = 0l;
      this->tgt_ang = 0.0f;
    }
  }

  // field types and members
  using _r1_x_type =
    int32_t;
  _r1_x_type r1_x;
  using _r1_y_type =
    int32_t;
  _r1_y_type r1_y;
  using _r1_ang_type =
    float;
  _r1_ang_type r1_ang;
  using _r2_x_type =
    int32_t;
  _r2_x_type r2_x;
  using _r2_y_type =
    int32_t;
  _r2_y_type r2_y;
  using _r2_ang_type =
    float;
  _r2_ang_type r2_ang;
  using _obj1_x_type =
    int32_t;
  _obj1_x_type obj1_x;
  using _obj1_y_type =
    int32_t;
  _obj1_y_type obj1_y;
  using _obj1_ang_type =
    float;
  _obj1_ang_type obj1_ang;
  using _obj2_x_type =
    int32_t;
  _obj2_x_type obj2_x;
  using _obj2_y_type =
    int32_t;
  _obj2_y_type obj2_y;
  using _obj2_ang_type =
    float;
  _obj2_ang_type obj2_ang;
  using _tgt_x_type =
    int32_t;
  _tgt_x_type tgt_x;
  using _tgt_y_type =
    int32_t;
  _tgt_y_type tgt_y;
  using _tgt_ang_type =
    float;
  _tgt_ang_type tgt_ang;

  // setters for named parameter idiom
  Type & set__r1_x(
    const int32_t & _arg)
  {
    this->r1_x = _arg;
    return *this;
  }
  Type & set__r1_y(
    const int32_t & _arg)
  {
    this->r1_y = _arg;
    return *this;
  }
  Type & set__r1_ang(
    const float & _arg)
  {
    this->r1_ang = _arg;
    return *this;
  }
  Type & set__r2_x(
    const int32_t & _arg)
  {
    this->r2_x = _arg;
    return *this;
  }
  Type & set__r2_y(
    const int32_t & _arg)
  {
    this->r2_y = _arg;
    return *this;
  }
  Type & set__r2_ang(
    const float & _arg)
  {
    this->r2_ang = _arg;
    return *this;
  }
  Type & set__obj1_x(
    const int32_t & _arg)
  {
    this->obj1_x = _arg;
    return *this;
  }
  Type & set__obj1_y(
    const int32_t & _arg)
  {
    this->obj1_y = _arg;
    return *this;
  }
  Type & set__obj1_ang(
    const float & _arg)
  {
    this->obj1_ang = _arg;
    return *this;
  }
  Type & set__obj2_x(
    const int32_t & _arg)
  {
    this->obj2_x = _arg;
    return *this;
  }
  Type & set__obj2_y(
    const int32_t & _arg)
  {
    this->obj2_y = _arg;
    return *this;
  }
  Type & set__obj2_ang(
    const float & _arg)
  {
    this->obj2_ang = _arg;
    return *this;
  }
  Type & set__tgt_x(
    const int32_t & _arg)
  {
    this->tgt_x = _arg;
    return *this;
  }
  Type & set__tgt_y(
    const int32_t & _arg)
  {
    this->tgt_y = _arg;
    return *this;
  }
  Type & set__tgt_ang(
    const float & _arg)
  {
    this->tgt_ang = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::ImgData_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::ImgData_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::ImgData_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::ImgData_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::ImgData_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::ImgData_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::ImgData_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::ImgData_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::ImgData_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::ImgData_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__ImgData
    std::shared_ptr<interfaces::msg::ImgData_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__ImgData
    std::shared_ptr<interfaces::msg::ImgData_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ImgData_ & other) const
  {
    if (this->r1_x != other.r1_x) {
      return false;
    }
    if (this->r1_y != other.r1_y) {
      return false;
    }
    if (this->r1_ang != other.r1_ang) {
      return false;
    }
    if (this->r2_x != other.r2_x) {
      return false;
    }
    if (this->r2_y != other.r2_y) {
      return false;
    }
    if (this->r2_ang != other.r2_ang) {
      return false;
    }
    if (this->obj1_x != other.obj1_x) {
      return false;
    }
    if (this->obj1_y != other.obj1_y) {
      return false;
    }
    if (this->obj1_ang != other.obj1_ang) {
      return false;
    }
    if (this->obj2_x != other.obj2_x) {
      return false;
    }
    if (this->obj2_y != other.obj2_y) {
      return false;
    }
    if (this->obj2_ang != other.obj2_ang) {
      return false;
    }
    if (this->tgt_x != other.tgt_x) {
      return false;
    }
    if (this->tgt_y != other.tgt_y) {
      return false;
    }
    if (this->tgt_ang != other.tgt_ang) {
      return false;
    }
    return true;
  }
  bool operator!=(const ImgData_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ImgData_

// alias to use template instance with default allocator
using ImgData =
  interfaces::msg::ImgData_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__IMG_DATA__STRUCT_HPP_
