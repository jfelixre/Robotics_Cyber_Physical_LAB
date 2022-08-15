// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from interfaces:msg/ControlData.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "interfaces/msg/detail/control_data__struct.h"
#include "interfaces/msg/detail/control_data__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__msg__control_data__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[41];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("interfaces.msg._control_data.ControlData", full_classname_dest, 40) == 0);
  }
  interfaces__msg__ControlData * ros_message = _ros_message;
  {  // time
    PyObject * field = PyObject_GetAttrString(_pymsg, "time");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->time = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // fase
    PyObject * field = PyObject_GetAttrString(_pymsg, "fase");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->fase = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // x_robot
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_robot");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->x_robot = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // y_robot
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_robot");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->y_robot = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // ang_robot
    PyObject * field = PyObject_GetAttrString(_pymsg, "ang_robot");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->ang_robot = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // x_obj
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_obj");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->x_obj = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // y_obj
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_obj");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->y_obj = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // ang_obj
    PyObject * field = PyObject_GetAttrString(_pymsg, "ang_obj");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->ang_obj = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // x_tgt
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_tgt");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->x_tgt = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // y_tgt
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_tgt");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->y_tgt = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // ang_tgt
    PyObject * field = PyObject_GetAttrString(_pymsg, "ang_tgt");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->ang_tgt = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // x_goal
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_goal");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->x_goal = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // y_goal
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_goal");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->y_goal = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // ang_goal
    PyObject * field = PyObject_GetAttrString(_pymsg, "ang_goal");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->ang_goal = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // x_error
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_error");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->x_error = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // y_error
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_error");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->y_error = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // ang_error
    PyObject * field = PyObject_GetAttrString(_pymsg, "ang_error");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->ang_error = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // vel_x_world
    PyObject * field = PyObject_GetAttrString(_pymsg, "vel_x_world");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->vel_x_world = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // vel_y_world
    PyObject * field = PyObject_GetAttrString(_pymsg, "vel_y_world");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->vel_y_world = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // vel_ang_world
    PyObject * field = PyObject_GetAttrString(_pymsg, "vel_ang_world");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->vel_ang_world = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // vel_x_robot
    PyObject * field = PyObject_GetAttrString(_pymsg, "vel_x_robot");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->vel_x_robot = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // vel_y_robot
    PyObject * field = PyObject_GetAttrString(_pymsg, "vel_y_robot");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->vel_y_robot = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // vel_ang_robot
    PyObject * field = PyObject_GetAttrString(_pymsg, "vel_ang_robot");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->vel_ang_robot = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__msg__control_data__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ControlData */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.msg._control_data");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ControlData");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__msg__ControlData * ros_message = (interfaces__msg__ControlData *)raw_ros_message;
  {  // time
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->time);
    {
      int rc = PyObject_SetAttrString(_pymessage, "time", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // fase
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->fase);
    {
      int rc = PyObject_SetAttrString(_pymessage, "fase", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x_robot
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->x_robot);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_robot", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_robot
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->y_robot);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_robot", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ang_robot
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->ang_robot);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ang_robot", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x_obj
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->x_obj);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_obj", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_obj
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->y_obj);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_obj", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ang_obj
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->ang_obj);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ang_obj", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x_tgt
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->x_tgt);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_tgt", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_tgt
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->y_tgt);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_tgt", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ang_tgt
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->ang_tgt);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ang_tgt", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x_goal
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->x_goal);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_goal", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_goal
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->y_goal);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_goal", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ang_goal
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->ang_goal);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ang_goal", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x_error
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->x_error);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_error", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_error
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->y_error);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_error", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ang_error
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->ang_error);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ang_error", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // vel_x_world
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->vel_x_world);
    {
      int rc = PyObject_SetAttrString(_pymessage, "vel_x_world", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // vel_y_world
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->vel_y_world);
    {
      int rc = PyObject_SetAttrString(_pymessage, "vel_y_world", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // vel_ang_world
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->vel_ang_world);
    {
      int rc = PyObject_SetAttrString(_pymessage, "vel_ang_world", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // vel_x_robot
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->vel_x_robot);
    {
      int rc = PyObject_SetAttrString(_pymessage, "vel_x_robot", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // vel_y_robot
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->vel_y_robot);
    {
      int rc = PyObject_SetAttrString(_pymessage, "vel_y_robot", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // vel_ang_robot
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->vel_ang_robot);
    {
      int rc = PyObject_SetAttrString(_pymessage, "vel_ang_robot", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
