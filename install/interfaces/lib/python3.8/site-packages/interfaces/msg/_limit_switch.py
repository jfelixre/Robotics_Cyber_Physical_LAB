# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/LimitSwitch.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_LimitSwitch(type):
    """Metaclass of message 'LimitSwitch'."""

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
                'interfaces.msg.LimitSwitch')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__limit_switch
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__limit_switch
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__limit_switch
            cls._TYPE_SUPPORT = module.type_support_msg__msg__limit_switch
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__limit_switch

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class LimitSwitch(metaclass=Metaclass_LimitSwitch):
    """Message class 'LimitSwitch'."""

    __slots__ = [
        '_b1_min',
        '_b1_max',
        '_b2_min',
        '_b2_max',
        '_b3_min',
        '_b3_max',
    ]

    _fields_and_field_types = {
        'b1_min': 'int32',
        'b1_max': 'int32',
        'b2_min': 'int32',
        'b2_max': 'int32',
        'b3_min': 'int32',
        'b3_max': 'int32',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.b1_min = kwargs.get('b1_min', int())
        self.b1_max = kwargs.get('b1_max', int())
        self.b2_min = kwargs.get('b2_min', int())
        self.b2_max = kwargs.get('b2_max', int())
        self.b3_min = kwargs.get('b3_min', int())
        self.b3_max = kwargs.get('b3_max', int())

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
        if self.b1_min != other.b1_min:
            return False
        if self.b1_max != other.b1_max:
            return False
        if self.b2_min != other.b2_min:
            return False
        if self.b2_max != other.b2_max:
            return False
        if self.b3_min != other.b3_min:
            return False
        if self.b3_max != other.b3_max:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def b1_min(self):
        """Message field 'b1_min'."""
        return self._b1_min

    @b1_min.setter
    def b1_min(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b1_min' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'b1_min' field must be an integer in [-2147483648, 2147483647]"
        self._b1_min = value

    @property
    def b1_max(self):
        """Message field 'b1_max'."""
        return self._b1_max

    @b1_max.setter
    def b1_max(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b1_max' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'b1_max' field must be an integer in [-2147483648, 2147483647]"
        self._b1_max = value

    @property
    def b2_min(self):
        """Message field 'b2_min'."""
        return self._b2_min

    @b2_min.setter
    def b2_min(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b2_min' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'b2_min' field must be an integer in [-2147483648, 2147483647]"
        self._b2_min = value

    @property
    def b2_max(self):
        """Message field 'b2_max'."""
        return self._b2_max

    @b2_max.setter
    def b2_max(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b2_max' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'b2_max' field must be an integer in [-2147483648, 2147483647]"
        self._b2_max = value

    @property
    def b3_min(self):
        """Message field 'b3_min'."""
        return self._b3_min

    @b3_min.setter
    def b3_min(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b3_min' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'b3_min' field must be an integer in [-2147483648, 2147483647]"
        self._b3_min = value

    @property
    def b3_max(self):
        """Message field 'b3_max'."""
        return self._b3_max

    @b3_max.setter
    def b3_max(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b3_max' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'b3_max' field must be an integer in [-2147483648, 2147483647]"
        self._b3_max = value
