// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__CONTROL_DATA__STRUCT_H_
#define INTERFACES__MSG__DETAIL__CONTROL_DATA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/ControlData in the package interfaces.
typedef struct interfaces__msg__ControlData
{
  float time;
  int32_t fase;
  int32_t x_robot;
  int32_t y_robot;
  float ang_robot;
  int32_t x_obj;
  int32_t y_obj;
  float ang_obj;
  int32_t x_tgt;
  int32_t y_tgt;
  float ang_tgt;
  int32_t x_goal;
  int32_t y_goal;
  float ang_goal;
  int32_t x_error;
  int32_t y_error;
  float ang_error;
  float vel_x_world;
  float vel_y_world;
  float vel_ang_world;
  float vel_x_robot;
  float vel_y_robot;
  float vel_ang_robot;
} interfaces__msg__ControlData;

// Struct for a sequence of interfaces__msg__ControlData.
typedef struct interfaces__msg__ControlData__Sequence
{
  interfaces__msg__ControlData * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__ControlData__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__CONTROL_DATA__STRUCT_H_
