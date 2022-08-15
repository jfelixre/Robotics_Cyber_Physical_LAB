// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:srv/PlatformVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__PLATFORM_VEL__TRAITS_HPP_
#define INTERFACES__SRV__DETAIL__PLATFORM_VEL__TRAITS_HPP_

#include "interfaces/srv/detail/platform_vel__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::PlatformVel_Request>()
{
  return "interfaces::srv::PlatformVel_Request";
}

template<>
inline const char * name<interfaces::srv::PlatformVel_Request>()
{
  return "interfaces/srv/PlatformVel_Request";
}

template<>
struct has_fixed_size<interfaces::srv::PlatformVel_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::srv::PlatformVel_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::srv::PlatformVel_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::PlatformVel_Response>()
{
  return "interfaces::srv::PlatformVel_Response";
}

template<>
inline const char * name<interfaces::srv::PlatformVel_Response>()
{
  return "interfaces/srv/PlatformVel_Response";
}

template<>
struct has_fixed_size<interfaces::srv::PlatformVel_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::srv::PlatformVel_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::srv::PlatformVel_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<interfaces::srv::PlatformVel>()
{
  return "interfaces::srv::PlatformVel";
}

template<>
inline const char * name<interfaces::srv::PlatformVel>()
{
  return "interfaces/srv/PlatformVel";
}

template<>
struct has_fixed_size<interfaces::srv::PlatformVel>
  : std::integral_constant<
    bool,
    has_fixed_size<interfaces::srv::PlatformVel_Request>::value &&
    has_fixed_size<interfaces::srv::PlatformVel_Response>::value
  >
{
};

template<>
struct has_bounded_size<interfaces::srv::PlatformVel>
  : std::integral_constant<
    bool,
    has_bounded_size<interfaces::srv::PlatformVel_Request>::value &&
    has_bounded_size<interfaces::srv::PlatformVel_Response>::value
  >
{
};

template<>
struct is_service<interfaces::srv::PlatformVel>
  : std::true_type
{
};

template<>
struct is_service_request<interfaces::srv::PlatformVel_Request>
  : std::true_type
{
};

template<>
struct is_service_response<interfaces::srv::PlatformVel_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__SRV__DETAIL__PLATFORM_VEL__TRAITS_HPP_
