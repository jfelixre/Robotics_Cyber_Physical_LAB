// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from interfaces:srv/RobotVel.idl
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
#include "interfaces/srv/detail/robot_vel__struct.h"
#include "interfaces/srv/detail/robot_vel__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__srv__robot_vel__request__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[43];
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
    assert(strncmp("interfaces.srv._robot_vel.RobotVel_Request", full_classname_dest, 42) == 0);
  }
  interfaces__srv__RobotVel_Request * ros_message = _ros_message;
  {  // x_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->x_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // y_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->y_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // ang_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "ang_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->ang_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // b1_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "b1_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->b1_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // b2_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "b2_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->b2_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // b3_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "b3_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->b3_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // g1_vel
    PyObject * field = PyObject_GetAttrString(_pymsg, "g1_vel");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->g1_vel = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__srv__robot_vel__request__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of RobotVel_Request */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.srv._robot_vel");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "RobotVel_Request");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__srv__RobotVel_Request * ros_message = (interfaces__srv__RobotVel_Request *)raw_ros_message;
  {  // x_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->x_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->y_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ang_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->ang_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ang_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // b1_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->b1_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "b1_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // b2_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->b2_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "b2_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // b3_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->b3_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "b3_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // g1_vel
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->g1_vel);
    {
      int rc = PyObject_SetAttrString(_pymessage, "g1_vel", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
// already included above
// #include <Python.h>
// already included above
// #include <stdbool.h>
// already included above
// #include "numpy/ndarrayobject.h"
// already included above
// #include "rosidl_runtime_c/visibility_control.h"
// already included above
// #include "interfaces/srv/detail/robot_vel__struct.h"
// already included above
// #include "interfaces/srv/detail/robot_vel__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__srv__robot_vel__response__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[44];
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
    assert(strncmp("interfaces.srv._robot_vel.RobotVel_Response", full_classname_dest, 43) == 0);
  }
  interfaces__srv__RobotVel_Response * ros_message = _ros_message;
  {  // sent
    PyObject * field = PyObject_GetAttrString(_pymsg, "sent");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->sent = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__srv__robot_vel__response__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of RobotVel_Response */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.srv._robot_vel");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "RobotVel_Response");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__srv__RobotVel_Response * ros_message = (interfaces__srv__RobotVel_Response *)raw_ros_message;
  {  // sent
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->sent);
    {
      int rc = PyObject_SetAttrString(_pymessage, "sent", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
