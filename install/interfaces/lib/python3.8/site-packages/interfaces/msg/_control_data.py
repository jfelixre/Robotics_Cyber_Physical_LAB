# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/ControlData.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ControlData(type):
    """Metaclass of message 'ControlData'."""

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
                'interfaces.msg.ControlData')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__control_data
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__control_data
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__control_data
            cls._TYPE_SUPPORT = module.type_support_msg__msg__control_data
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__control_data

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ControlData(metaclass=Metaclass_ControlData):
    """Message class 'ControlData'."""

    __slots__ = [
        '_time',
        '_fase',
        '_x_robot',
        '_y_robot',
        '_ang_robot',
        '_x_obj',
        '_y_obj',
        '_ang_obj',
        '_x_tgt',
        '_y_tgt',
        '_ang_tgt',
        '_x_goal',
        '_y_goal',
        '_ang_goal',
        '_x_error',
        '_y_error',
        '_ang_error',
        '_vel_x_world',
        '_vel_y_world',
        '_vel_ang_world',
        '_vel_x_robot',
        '_vel_y_robot',
        '_vel_ang_robot',
    ]

    _fields_and_field_types = {
        'time': 'float',
        'fase': 'int32',
        'x_robot': 'int32',
        'y_robot': 'int32',
        'ang_robot': 'float',
        'x_obj': 'int32',
        'y_obj': 'int32',
        'ang_obj': 'float',
        'x_tgt': 'int32',
        'y_tgt': 'int32',
        'ang_tgt': 'float',
        'x_goal': 'int32',
        'y_goal': 'int32',
        'ang_goal': 'float',
        'x_error': 'int32',
        'y_error': 'int32',
        'ang_error': 'float',
        'vel_x_world': 'float',
        'vel_y_world': 'float',
        'vel_ang_world': 'float',
        'vel_x_robot': 'float',
        'vel_y_robot': 'float',
        'vel_ang_robot': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
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
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.time = kwargs.get('time', float())
        self.fase = kwargs.get('fase', int())
        self.x_robot = kwargs.get('x_robot', int())
        self.y_robot = kwargs.get('y_robot', int())
        self.ang_robot = kwargs.get('ang_robot', float())
        self.x_obj = kwargs.get('x_obj', int())
        self.y_obj = kwargs.get('y_obj', int())
        self.ang_obj = kwargs.get('ang_obj', float())
        self.x_tgt = kwargs.get('x_tgt', int())
        self.y_tgt = kwargs.get('y_tgt', int())
        self.ang_tgt = kwargs.get('ang_tgt', float())
        self.x_goal = kwargs.get('x_goal', int())
        self.y_goal = kwargs.get('y_goal', int())
        self.ang_goal = kwargs.get('ang_goal', float())
        self.x_error = kwargs.get('x_error', int())
        self.y_error = kwargs.get('y_error', int())
        self.ang_error = kwargs.get('ang_error', float())
        self.vel_x_world = kwargs.get('vel_x_world', float())
        self.vel_y_world = kwargs.get('vel_y_world', float())
        self.vel_ang_world = kwargs.get('vel_ang_world', float())
        self.vel_x_robot = kwargs.get('vel_x_robot', float())
        self.vel_y_robot = kwargs.get('vel_y_robot', float())
        self.vel_ang_robot = kwargs.get('vel_ang_robot', float())

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
        if self.time != other.time:
            return False
        if self.fase != other.fase:
            return False
        if self.x_robot != other.x_robot:
            return False
        if self.y_robot != other.y_robot:
            return False
        if self.ang_robot != other.ang_robot:
            return False
        if self.x_obj != other.x_obj:
            return False
        if self.y_obj != other.y_obj:
            return False
        if self.ang_obj != other.ang_obj:
            return False
        if self.x_tgt != other.x_tgt:
            return False
        if self.y_tgt != other.y_tgt:
            return False
        if self.ang_tgt != other.ang_tgt:
            return False
        if self.x_goal != other.x_goal:
            return False
        if self.y_goal != other.y_goal:
            return False
        if self.ang_goal != other.ang_goal:
            return False
        if self.x_error != other.x_error:
            return False
        if self.y_error != other.y_error:
            return False
        if self.ang_error != other.ang_error:
            return False
        if self.vel_x_world != other.vel_x_world:
            return False
        if self.vel_y_world != other.vel_y_world:
            return False
        if self.vel_ang_world != other.vel_ang_world:
            return False
        if self.vel_x_robot != other.vel_x_robot:
            return False
        if self.vel_y_robot != other.vel_y_robot:
            return False
        if self.vel_ang_robot != other.vel_ang_robot:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def time(self):
        """Message field 'time'."""
        return self._time

    @time.setter
    def time(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'time' field must be of type 'float'"
        self._time = value

    @property
    def fase(self):
        """Message field 'fase'."""
        return self._fase

    @fase.setter
    def fase(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'fase' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'fase' field must be an integer in [-2147483648, 2147483647]"
        self._fase = value

    @property
    def x_robot(self):
        """Message field 'x_robot'."""
        return self._x_robot

    @x_robot.setter
    def x_robot(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'x_robot' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'x_robot' field must be an integer in [-2147483648, 2147483647]"
        self._x_robot = value

    @property
    def y_robot(self):
        """Message field 'y_robot'."""
        return self._y_robot

    @y_robot.setter
    def y_robot(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'y_robot' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'y_robot' field must be an integer in [-2147483648, 2147483647]"
        self._y_robot = value

    @property
    def ang_robot(self):
        """Message field 'ang_robot'."""
        return self._ang_robot

    @ang_robot.setter
    def ang_robot(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ang_robot' field must be of type 'float'"
        self._ang_robot = value

    @property
    def x_obj(self):
        """Message field 'x_obj'."""
        return self._x_obj

    @x_obj.setter
    def x_obj(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'x_obj' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'x_obj' field must be an integer in [-2147483648, 2147483647]"
        self._x_obj = value

    @property
    def y_obj(self):
        """Message field 'y_obj'."""
        return self._y_obj

    @y_obj.setter
    def y_obj(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'y_obj' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'y_obj' field must be an integer in [-2147483648, 2147483647]"
        self._y_obj = value

    @property
    def ang_obj(self):
        """Message field 'ang_obj'."""
        return self._ang_obj

    @ang_obj.setter
    def ang_obj(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ang_obj' field must be of type 'float'"
        self._ang_obj = value

    @property
    def x_tgt(self):
        """Message field 'x_tgt'."""
        return self._x_tgt

    @x_tgt.setter
    def x_tgt(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'x_tgt' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'x_tgt' field must be an integer in [-2147483648, 2147483647]"
        self._x_tgt = value

    @property
    def y_tgt(self):
        """Message field 'y_tgt'."""
        return self._y_tgt

    @y_tgt.setter
    def y_tgt(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'y_tgt' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'y_tgt' field must be an integer in [-2147483648, 2147483647]"
        self._y_tgt = value

    @property
    def ang_tgt(self):
        """Message field 'ang_tgt'."""
        return self._ang_tgt

    @ang_tgt.setter
    def ang_tgt(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ang_tgt' field must be of type 'float'"
        self._ang_tgt = value

    @property
    def x_goal(self):
        """Message field 'x_goal'."""
        return self._x_goal

    @x_goal.setter
    def x_goal(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'x_goal' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'x_goal' field must be an integer in [-2147483648, 2147483647]"
        self._x_goal = value

    @property
    def y_goal(self):
        """Message field 'y_goal'."""
        return self._y_goal

    @y_goal.setter
    def y_goal(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'y_goal' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'y_goal' field must be an integer in [-2147483648, 2147483647]"
        self._y_goal = value

    @property
    def ang_goal(self):
        """Message field 'ang_goal'."""
        return self._ang_goal

    @ang_goal.setter
    def ang_goal(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ang_goal' field must be of type 'float'"
        self._ang_goal = value

    @property
    def x_error(self):
        """Message field 'x_error'."""
        return self._x_error

    @x_error.setter
    def x_error(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'x_error' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'x_error' field must be an integer in [-2147483648, 2147483647]"
        self._x_error = value

    @property
    def y_error(self):
        """Message field 'y_error'."""
        return self._y_error

    @y_error.setter
    def y_error(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'y_error' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'y_error' field must be an integer in [-2147483648, 2147483647]"
        self._y_error = value

    @property
    def ang_error(self):
        """Message field 'ang_error'."""
        return self._ang_error

    @ang_error.setter
    def ang_error(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ang_error' field must be of type 'float'"
        self._ang_error = value

    @property
    def vel_x_world(self):
        """Message field 'vel_x_world'."""
        return self._vel_x_world

    @vel_x_world.setter
    def vel_x_world(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_x_world' field must be of type 'float'"
        self._vel_x_world = value

    @property
    def vel_y_world(self):
        """Message field 'vel_y_world'."""
        return self._vel_y_world

    @vel_y_world.setter
    def vel_y_world(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_y_world' field must be of type 'float'"
        self._vel_y_world = value

    @property
    def vel_ang_world(self):
        """Message field 'vel_ang_world'."""
        return self._vel_ang_world

    @vel_ang_world.setter
    def vel_ang_world(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_ang_world' field must be of type 'float'"
        self._vel_ang_world = value

    @property
    def vel_x_robot(self):
        """Message field 'vel_x_robot'."""
        return self._vel_x_robot

    @vel_x_robot.setter
    def vel_x_robot(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_x_robot' field must be of type 'float'"
        self._vel_x_robot = value

    @property
    def vel_y_robot(self):
        """Message field 'vel_y_robot'."""
        return self._vel_y_robot

    @vel_y_robot.setter
    def vel_y_robot(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_y_robot' field must be of type 'float'"
        self._vel_y_robot = value

    @property
    def vel_ang_robot(self):
        """Message field 'vel_ang_robot'."""
        return self._vel_ang_robot

    @vel_ang_robot.setter
    def vel_ang_robot(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'vel_ang_robot' field must be of type 'float'"
        self._vel_ang_robot = value
