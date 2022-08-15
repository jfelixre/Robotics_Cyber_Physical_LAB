// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
#define INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/RobotState in the package interfaces.
typedef struct interfaces__msg__RobotState
{
  int32_t robot_state;
} interfaces__msg__RobotState;

// Struct for a sequence of interfaces__msg__RobotState.
typedef struct interfaces__msg__RobotState__Sequence
{
  interfaces__msg__RobotState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__RobotState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
