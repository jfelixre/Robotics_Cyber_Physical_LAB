// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/control_data__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


bool
interfaces__msg__ControlData__init(interfaces__msg__ControlData * msg)
{
  if (!msg) {
    return false;
  }
  // time
  // fase
  // x_robot
  // y_robot
  // ang_robot
  // x_obj
  // y_obj
  // ang_obj
  // x_tgt
  // y_tgt
  // ang_tgt
  // x_goal
  // y_goal
  // ang_goal
  // x_error
  // y_error
  // ang_error
  // vel_x_world
  // vel_y_world
  // vel_ang_world
  // vel_x_robot
  // vel_y_robot
  // vel_ang_robot
  return true;
}

void
interfaces__msg__ControlData__fini(interfaces__msg__ControlData * msg)
{
  if (!msg) {
    return;
  }
  // time
  // fase
  // x_robot
  // y_robot
  // ang_robot
  // x_obj
  // y_obj
  // ang_obj
  // x_tgt
  // y_tgt
  // ang_tgt
  // x_goal
  // y_goal
  // ang_goal
  // x_error
  // y_error
  // ang_error
  // vel_x_world
  // vel_y_world
  // vel_ang_world
  // vel_x_robot
  // vel_y_robot
  // vel_ang_robot
}

interfaces__msg__ControlData *
interfaces__msg__ControlData__create()
{
  interfaces__msg__ControlData * msg = (interfaces__msg__ControlData *)malloc(sizeof(interfaces__msg__ControlData));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(interfaces__msg__ControlData));
  bool success = interfaces__msg__ControlData__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
interfaces__msg__ControlData__destroy(interfaces__msg__ControlData * msg)
{
  if (msg) {
    interfaces__msg__ControlData__fini(msg);
  }
  free(msg);
}


bool
interfaces__msg__ControlData__Sequence__init(interfaces__msg__ControlData__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  interfaces__msg__ControlData * data = NULL;
  if (size) {
    data = (interfaces__msg__ControlData *)calloc(size, sizeof(interfaces__msg__ControlData));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = interfaces__msg__ControlData__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        interfaces__msg__ControlData__fini(&data[i - 1]);
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
interfaces__msg__ControlData__Sequence__fini(interfaces__msg__ControlData__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      interfaces__msg__ControlData__fini(&array->data[i]);
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

interfaces__msg__ControlData__Sequence *
interfaces__msg__ControlData__Sequence__create(size_t size)
{
  interfaces__msg__ControlData__Sequence * array = (interfaces__msg__ControlData__Sequence *)malloc(sizeof(interfaces__msg__ControlData__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = interfaces__msg__ControlData__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
interfaces__msg__ControlData__Sequence__destroy(interfaces__msg__ControlData__Sequence * array)
{
  if (array) {
    interfaces__msg__ControlData__Sequence__fini(array);
  }
  free(array);
}
