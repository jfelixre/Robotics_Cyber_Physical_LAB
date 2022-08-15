// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from interfaces:srv/ControlPidStart.idl
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
#include "interfaces/srv/detail/control_pid_start__struct.h"
#include "interfaces/srv/detail/control_pid_start__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__srv__control_pid_start__request__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[58];
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
    assert(strncmp("interfaces.srv._control_pid_start.ControlPidStart_Request", full_classname_dest, 57) == 0);
  }
  interfaces__srv__ControlPidStart_Request * ros_message = _ros_message;
  {  // fase
    PyObject * field = PyObject_GetAttrString(_pymsg, "fase");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->fase = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__srv__control_pid_start__request__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ControlPidStart_Request */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.srv._control_pid_start");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ControlPidStart_Request");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__srv__ControlPidStart_Request * ros_message = (interfaces__srv__ControlPidStart_Request *)raw_ros_message;
  {  // fase
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->fase);
    {
      int rc = PyObject_SetAttrString(_pymessage, "fase", field);
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
// #include "interfaces/srv/detail/control_pid_start__struct.h"
// already included above
// #include "interfaces/srv/detail/control_pid_start__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__srv__control_pid_start__response__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[59];
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
    assert(strncmp("interfaces.srv._control_pid_start.ControlPidStart_Response", full_classname_dest, 58) == 0);
  }
  interfaces__srv__ControlPidStart_Response * ros_message = _ros_message;
  {  // finish
    PyObject * field = PyObject_GetAttrString(_pymsg, "finish");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->finish = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__srv__control_pid_start__response__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ControlPidStart_Response */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.srv._control_pid_start");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ControlPidStart_Response");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__srv__ControlPidStart_Response * ros_message = (interfaces__srv__ControlPidStart_Response *)raw_ros_message;
  {  // finish
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->finish);
    {
      int rc = PyObject_SetAttrString(_pymessage, "finish", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
