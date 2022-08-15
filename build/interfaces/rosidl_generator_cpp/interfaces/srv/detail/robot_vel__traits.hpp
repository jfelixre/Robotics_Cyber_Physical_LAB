// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:srv/RobotVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__ROBOT_VEL__TRAITS_HPP_
#define INTERFACES__SRV__DETAIL__ROBOT_VEL__TRAITS_HPP_

#include "interfaces/srv/detail/robot_vel__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::RobotVel_Request>()
{
  return "interfaces::srv::RobotVel_Request";
}

template<>
inline const char * name<interfaces::srv::RobotVel_Request>()
{
  return "interfaces/srv/RobotVel_Request";
}

template<>
struct has_fixed_size<interfaces::srv::RobotVel_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::srv::RobotVel_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::srv::RobotVel_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::RobotVel_Response>()
{
  return "interfaces::srv::RobotVel_Response";
}

template<>
inline const char * name<interfaces::srv::RobotVel_Response>()
{
  return "interfaces/srv/RobotVel_Response";
}

template<>
struct has_fixed_size<interfaces::srv::RobotVel_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::srv::RobotVel_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::srv::RobotVel_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::RobotVel>()
{
  return "interfaces::srv::RobotVel";
}

template<>
inline const char * name<interfaces::srv::RobotVel>()
{
  return "interfaces/srv/RobotVel";
}

template<>
struct has_fixed_size<interfaces::srv::RobotVel>
  : std::integral_constant<
    bool,
    has_fixed_size<interfaces::srv::RobotVel_Request>::value &&
    has_fixed_size<interfaces::srv::RobotVel_Response>::value
  >
{
};

template<>
struct has_bounded_size<interfaces::srv::RobotVel>
  : std::integral_constant<
    bool,
    has_bounded_size<interfaces::srv::RobotVel_Request>::value &&
    has_bounded_size<interfaces::srv::RobotVel_Response>::value
  >
{
};

template<>
struct is_service<interfaces::srv::RobotVel>
  : std::true_type
{
};

template<>
struct is_service_request<interfaces::srv::RobotVel_Request>
  : std::true_type
{
};

template<>
struct is_service_response<interfaces::srv::RobotVel_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__SRV__DETAIL__ROBOT_VEL__TRAITS_HPP_
