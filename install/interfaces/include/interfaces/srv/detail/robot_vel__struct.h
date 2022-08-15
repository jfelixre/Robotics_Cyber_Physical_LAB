// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:srv/RobotVel.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__ROBOT_VEL__STRUCT_H_
#define INTERFACES__SRV__DETAIL__ROBOT_VEL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in srv/RobotVel in the package interfaces.
typedef struct interfaces__srv__RobotVel_Request
{
  float x_vel;
  float y_vel;
  float ang_vel;
  float b1_vel;
  float b2_vel;
  float b3_vel;
  float g1_vel;
} interfaces__srv__RobotVel_Request;

// Struct for a sequence of interfaces__srv__RobotVel_Request.
typedef struct interfaces__srv__RobotVel_Request__Sequence
{
  interfaces__srv__RobotVel_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__srv__RobotVel_Request__Sequence;


// Constants defined in the message

// Struct defined in srv/RobotVel in the package interfaces.
typedef struct interfaces__srv__RobotVel_Response
{
  int64_t sent;
} interfaces__srv__RobotVel_Response;

// Struct for a sequence of interfaces__srv__RobotVel_Response.
typedef struct interfaces__srv__RobotVel_Response__Sequence
{
  interfaces__srv__RobotVel_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__srv__RobotVel_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__SRV__DETAIL__ROBOT_VEL__STRUCT_H_
