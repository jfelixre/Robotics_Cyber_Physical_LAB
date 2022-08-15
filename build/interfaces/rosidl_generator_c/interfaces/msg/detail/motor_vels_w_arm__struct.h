// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/MotorVelsWArm.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__STRUCT_H_
#define INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/MotorVelsWArm in the package interfaces.
typedef struct interfaces__msg__MotorVelsWArm
{
  float vel_m1;
  float vel_m2;
  float vel_m3;
  float vel_m4;
  float vel_b1;
  float vel_b2;
  float vel_b3;
  float vel_g1;
} interfaces__msg__MotorVelsWArm;

// Struct for a sequence of interfaces__msg__MotorVelsWArm.
typedef struct interfaces__msg__MotorVelsWArm__Sequence
{
  interfaces__msg__MotorVelsWArm * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__MotorVelsWArm__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__MOTOR_VELS_W_ARM__STRUCT_H_
