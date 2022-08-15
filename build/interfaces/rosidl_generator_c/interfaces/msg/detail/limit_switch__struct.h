// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/LimitSwitch.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__LIMIT_SWITCH__STRUCT_H_
#define INTERFACES__MSG__DETAIL__LIMIT_SWITCH__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/LimitSwitch in the package interfaces.
typedef struct interfaces__msg__LimitSwitch
{
  int32_t b1_min;
  int32_t b1_max;
  int32_t b2_min;
  int32_t b2_max;
  int32_t b3_min;
  int32_t b3_max;
} interfaces__msg__LimitSwitch;

// Struct for a sequence of interfaces__msg__LimitSwitch.
typedef struct interfaces__msg__LimitSwitch__Sequence
{
  interfaces__msg__LimitSwitch * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__LimitSwitch__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__LIMIT_SWITCH__STRUCT_H_
