# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/MotorVels.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_MotorVels(type):
    """Metaclass of message 'MotorVels'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'interfaces.msg.MotorVels')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__motor_vels
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__motor_vels
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__motor_vels
            cls._TYPE_SUPPORT = module.type_support_msg__msg__motor_vels
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__motor_vels

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class MotorVels(metaclass=Metaclass_MotorVels):
    """Message class 'MotorVels'."""

    __slots__ = [
        '_vel_m1',
        '_vel_m2',
        '_vel_m3',
        '_vel_m4',
    ]

    _fields_and_field_types = {
        'vel_m1': 'float',
        'vel_m2': 'float',
        'vel_m3': 'float',
        'vel_m4': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.vel_m1 = kwargs.get('vel_m1', float())
        self.vel_m2 = kwargs.get('vel_m2', float())
        self.vel_m3 = kwargs.get('vel_m3', float())
        self.vel_m4 = kwargs.get('vel_m4', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.vel_m1 != other.vel_m1:
            return False
        if self.vel_m2 != other.vel_m2:
            return False
        if self.vel_m3 != other.vel_m3:
            return False
        if self.vel_m4 != other.vel_m4:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def vel_m1(self):
        """Message field 'vel_m1'."""
        return self._vel_m1

    @vel_m1.setter
    def vel_m1(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_m1' field must be of type 'float'"
        self._vel_m1 = value

    @property
    def vel_m2(self):
        """Message field 'vel_m2'."""
        return self._vel_m2

    @vel_m2.setter
    def vel_m2(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_m2' field must be of type 'float'"
        self._vel_m2 = value

    @property
    def vel_m3(self):
        """Message field 'vel_m3'."""
        return self._vel_m3

    @vel_m3.setter
    def vel_m3(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_m3' field must be of type 'float'"
        self._vel_m3 = value

    @property
    def vel_m4(self):
        """Message field 'vel_m4'."""
        return self._vel_m4

    @vel_m4.setter
    def vel_m4(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_m4' field must be of type 'float'"
        self._vel_m4 = value
