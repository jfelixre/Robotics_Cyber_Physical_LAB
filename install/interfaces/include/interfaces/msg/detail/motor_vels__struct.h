// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/MotorVels.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS__STRUCT_H_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/MotorVels in the package interfaces.
typedef struct interfaces__msg__MotorVels
{
  float vel_m1;
  float vel_m2;
  float vel_m3;
  float vel_m4;
} interfaces__msg__MotorVels;

// Struct for a sequence of interfaces__msg__MotorVels.
typedef struct interfaces__msg__MotorVels__Sequence
{
  interfaces__msg__MotorVels * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__MotorVels__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS__STRUCT_H_
