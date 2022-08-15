# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/ImgData.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ImgData(type):
    """Metaclass of message 'ImgData'."""

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
                'interfaces.msg.ImgData')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__img_data
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__img_data
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__img_data
            cls._TYPE_SUPPORT = module.type_support_msg__msg__img_data
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__img_data

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ImgData(metaclass=Metaclass_ImgData):
    """Message class 'ImgData'."""

    __slots__ = [
        '_r1_x',
        '_r1_y',
        '_r1_ang',
        '_r2_x',
        '_r2_y',
        '_r2_ang',
        '_obj1_x',
        '_obj1_y',
        '_obj1_ang',
        '_obj2_x',
        '_obj2_y',
        '_obj2_ang',
        '_tgt_x',
        '_tgt_y',
        '_tgt_ang',
    ]

    _fields_and_field_types = {
        'r1_x': 'int32',
        'r1_y': 'int32',
        'r1_ang': 'float',
        'r2_x': 'int32',
        'r2_y': 'int32',
        'r2_ang': 'float',
        'obj1_x': 'int32',
        'obj1_y': 'int32',
        'obj1_ang': 'float',
        'obj2_x': 'int32',
        'obj2_y': 'int32',
        'obj2_ang': 'float',
        'tgt_x': 'int32',
        'tgt_y': 'int32',
        'tgt_ang': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.r1_x = kwargs.get('r1_x', int())
        self.r1_y = kwargs.get('r1_y', int())
        self.r1_ang = kwargs.get('r1_ang', float())
        self.r2_x = kwargs.get('r2_x', int())
        self.r2_y = kwargs.get('r2_y', int())
        self.r2_ang = kwargs.get('r2_ang', float())
        self.obj1_x = kwargs.get('obj1_x', int())
        self.obj1_y = kwargs.get('obj1_y', int())
        self.obj1_ang = kwargs.get('obj1_ang', float())
        self.obj2_x = kwargs.get('obj2_x', int())
        self.obj2_y = kwargs.get('obj2_y', int())
        self.obj2_ang = kwargs.get('obj2_ang', float())
        self.tgt_x = kwargs.get('tgt_x', int())
        self.tgt_y = kwargs.get('tgt_y', int())
        self.tgt_ang = kwargs.get('tgt_ang', float())

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
        if self.r1_x != other.r1_x:
            return False
        if self.r1_y != other.r1_y:
            return False
        if self.r1_ang != other.r1_ang:
            return False
        if self.r2_x != other.r2_x:
            return False
        if self.r2_y != other.r2_y:
            return False
        if self.r2_ang != other.r2_ang:
            return False
        if self.obj1_x != other.obj1_x:
            return False
        if self.obj1_y != other.obj1_y:
            return False
        if self.obj1_ang != other.obj1_ang:
            return False
        if self.obj2_x != other.obj2_x:
            return False
        if self.obj2_y != other.obj2_y:
            return False
        if self.obj2_ang != other.obj2_ang:
            return False
        if self.tgt_x != other.tgt_x:
            return False
        if self.tgt_y != other.tgt_y:
            return False
        if self.tgt_ang != other.tgt_ang:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def r1_x(self):
        """Message field 'r1_x'."""
        return self._r1_x

    @r1_x.setter
    def r1_x(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'r1_x' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'r1_x' field must be an integer in [-2147483648, 2147483647]"
        self._r1_x = value

    @property
    def r1_y(self):
        """Message field 'r1_y'."""
        return self._r1_y

    @r1_y.setter
    def r1_y(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'r1_y' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'r1_y' field must be an integer in [-2147483648, 2147483647]"
        self._r1_y = value

    @property
    def r1_ang(self):
        """Message field 'r1_ang'."""
        return self._r1_ang

    @r1_ang.setter
    def r1_ang(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'r1_ang' field must be of type 'float'"
        self._r1_ang = value

    @property
    def r2_x(self):
        """Message field 'r2_x'."""
        return self._r2_x

    @r2_x.setter
    def r2_x(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'r2_x' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'r2_x' field must be an integer in [-2147483648, 2147483647]"
        self._r2_x = value

    @property
    def r2_y(self):
        """Message field 'r2_y'."""
        return self._r2_y

    @r2_y.setter
    def r2_y(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'r2_y' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'r2_y' field must be an integer in [-2147483648, 2147483647]"
        self._r2_y = value

    @property
    def r2_ang(self):
        """Message field 'r2_ang'."""
        return self._r2_ang

    @r2_ang.setter
    def r2_ang(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'r2_ang' field must be of type 'float'"
        self._r2_ang = value

    @property
    def obj1_x(self):
        """Message field 'obj1_x'."""
        return self._obj1_x

    @obj1_x.setter
    def obj1_x(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'obj1_x' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'obj1_x' field must be an integer in [-2147483648, 2147483647]"
        self._obj1_x = value

    @property
    def obj1_y(self):
        """Message field 'obj1_y'."""
        return self._obj1_y

    @obj1_y.setter
    def obj1_y(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'obj1_y' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'obj1_y' field must be an integer in [-2147483648, 2147483647]"
        self._obj1_y = value

    @property
    def obj1_ang(self):
        """Message field 'obj1_ang'."""
        return self._obj1_ang

    @obj1_ang.setter
    def obj1_ang(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'obj1_ang' field must be of type 'float'"
        self._obj1_ang = value

    @property
    def obj2_x(self):
        """Message field 'obj2_x'."""
        return self._obj2_x

    @obj2_x.setter
    def obj2_x(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'obj2_x' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'obj2_x' field must be an integer in [-2147483648, 2147483647]"
        self._obj2_x = value

    @property
    def obj2_y(self):
        """Message field 'obj2_y'."""
        return self._obj2_y

    @obj2_y.setter
    def obj2_y(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'obj2_y' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'obj2_y' field must be an integer in [-2147483648, 2147483647]"
        self._obj2_y = value

    @property
    def obj2_ang(self):
        """Message field 'obj2_ang'."""
        return self._obj2_ang

    @obj2_ang.setter
    def obj2_ang(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'obj2_ang' field must be of type 'float'"
        self._obj2_ang = value

    @property
    def tgt_x(self):
        """Message field 'tgt_x'."""
        return self._tgt_x

    @tgt_x.setter
    def tgt_x(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'tgt_x' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'tgt_x' field must be an integer in [-2147483648, 2147483647]"
        self._tgt_x = value

    @property
    def tgt_y(self):
        """Message field 'tgt_y'."""
        return self._tgt_y

    @tgt_y.setter
    def tgt_y(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'tgt_y' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'tgt_y' field must be an integer in [-2147483648, 2147483647]"
        self._tgt_y = value

    @property
    def tgt_ang(self):
        """Message field 'tgt_ang'."""
        return self._tgt_ang

    @tgt_ang.setter
    def tgt_ang(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'tgt_ang' field must be of type 'float'"
        self._tgt_ang = value
