// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from interfaces:msg/ImgData.idl
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
#include "interfaces/msg/detail/img_data__struct.h"
#include "interfaces/msg/detail/img_data__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__msg__img_data__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[33];
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
    assert(strncmp("interfaces.msg._img_data.ImgData", full_classname_dest, 32) == 0);
  }
  interfaces__msg__ImgData * ros_message = _ros_message;
  {  // r1_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "r1_x");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->r1_x = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // r1_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "r1_y");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->r1_y = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // r1_ang
    PyObject * field = PyObject_GetAttrString(_pymsg, "r1_ang");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->r1_ang = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // r2_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "r2_x");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->r2_x = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // r2_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "r2_y");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->r2_y = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // r2_ang
    PyObject * field = PyObject_GetAttrString(_pymsg, "r2_ang");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->r2_ang = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // obj1_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "obj1_x");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->obj1_x = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // obj1_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "obj1_y");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->obj1_y = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // obj1_ang
    PyObject * field = PyObject_GetAttrString(_pymsg, "obj1_ang");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->obj1_ang = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // obj2_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "obj2_x");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->obj2_x = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // obj2_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "obj2_y");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->obj2_y = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // obj2_ang
    PyObject * field = PyObject_GetAttrString(_pymsg, "obj2_ang");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->obj2_ang = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // tgt_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "tgt_x");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->tgt_x = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // tgt_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "tgt_y");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->tgt_y = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // tgt_ang
    PyObject * field = PyObject_GetAttrString(_pymsg, "tgt_ang");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->tgt_ang = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__msg__img_data__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ImgData */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.msg._img_data");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ImgData");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__msg__ImgData * ros_message = (interfaces__msg__ImgData *)raw_ros_message;
  {  // r1_x
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->r1_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r1_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // r1_y
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->r1_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r1_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // r1_ang
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->r1_ang);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r1_ang", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // r2_x
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->r2_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r2_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // r2_y
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->r2_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r2_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // r2_ang
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->r2_ang);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r2_ang", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // obj1_x
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->obj1_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "obj1_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // obj1_y
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->obj1_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "obj1_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // obj1_ang
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->obj1_ang);
    {
      int rc = PyObject_SetAttrString(_pymessage, "obj1_ang", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // obj2_x
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->obj2_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "obj2_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // obj2_y
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->obj2_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "obj2_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // obj2_ang
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->obj2_ang);
    {
      int rc = PyObject_SetAttrString(_pymessage, "obj2_ang", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // tgt_x
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->tgt_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "tgt_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // tgt_y
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->tgt_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "tgt_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // tgt_ang
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->tgt_ang);
    {
      int rc = PyObject_SetAttrString(_pymessage, "tgt_ang", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
