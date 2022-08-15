// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from interfaces:msg/ImgData.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/img_data__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


bool
interfaces__msg__ImgData__init(interfaces__msg__ImgData * msg)
{
  if (!msg) {
    return false;
  }
  // r1_x
  // r1_y
  // r1_ang
  // r2_x
  // r2_y
  // r2_ang
  // obj1_x
  // obj1_y
  // obj1_ang
  // obj2_x
  // obj2_y
  // obj2_ang
  // tgt_x
  // tgt_y
  // tgt_ang
  return true;
}

void
interfaces__msg__ImgData__fini(interfaces__msg__ImgData * msg)
{
  if (!msg) {
    return;
  }
  // r1_x
  // r1_y
  // r1_ang
  // r2_x
  // r2_y
  // r2_ang
  // obj1_x
  // obj1_y
  // obj1_ang
  // obj2_x
  // obj2_y
  // obj2_ang
  // tgt_x
  // tgt_y
  // tgt_ang
}

interfaces__msg__ImgData *
interfaces__msg__ImgData__create()
{
  interfaces__msg__ImgData * msg = (interfaces__msg__ImgData *)malloc(sizeof(interfaces__msg__ImgData));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(interfaces__msg__ImgData));
  bool success = interfaces__msg__ImgData__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
interfaces__msg__ImgData__destroy(interfaces__msg__ImgData * msg)
{
  if (msg) {
    interfaces__msg__ImgData__fini(msg);
  }
  free(msg);
}


bool
interfaces__msg__ImgData__Sequence__init(interfaces__msg__ImgData__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  interfaces__msg__ImgData * data = NULL;
  if (size) {
    data = (interfaces__msg__ImgData *)calloc(size, sizeof(interfaces__msg__ImgData));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = interfaces__msg__ImgData__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        interfaces__msg__ImgData__fini(&data[i - 1]);
      }
      free(data);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
interfaces__msg__ImgData__Sequence__fini(interfaces__msg__ImgData__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      interfaces__msg__ImgData__fini(&array->data[i]);
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

interfaces__msg__ImgData__Sequence *
interfaces__msg__ImgData__Sequence__create(size_t size)
{
  interfaces__msg__ImgData__Sequence * array = (interfaces__msg__ImgData__Sequence *)malloc(sizeof(interfaces__msg__ImgData__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = interfaces__msg__ImgData__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
interfaces__msg__ImgData__Sequence__destroy(interfaces__msg__ImgData__Sequence * array)
{
  if (array) {
    interfaces__msg__ImgData__Sequence__fini(array);
  }
  free(array);
}
