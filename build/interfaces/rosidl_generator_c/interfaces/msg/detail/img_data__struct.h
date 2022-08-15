// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/ImgData.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__IMG_DATA__STRUCT_H_
#define INTERFACES__MSG__DETAIL__IMG_DATA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/ImgData in the package interfaces.
typedef struct interfaces__msg__ImgData
{
  int32_t r1_x;
  int32_t r1_y;
  float r1_ang;
  int32_t r2_x;
  int32_t r2_y;
  float r2_ang;
  int32_t obj1_x;
  int32_t obj1_y;
  float obj1_ang;
  int32_t obj2_x;
  int32_t obj2_y;
  float obj2_ang;
  int32_t tgt_x;
  int32_t tgt_y;
  float tgt_ang;
} interfaces__msg__ImgData;

// Struct for a sequence of interfaces__msg__ImgData.
typedef struct interfaces__msg__ImgData__Sequence
{
  interfaces__msg__ImgData * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__ImgData__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__IMG_DATA__STRUCT_H_
