# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:srv/RobotVel.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_RobotVel_Request(type):
    """Metaclass of message 'RobotVel_Request'."""

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
                'interfaces.srv.RobotVel_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__robot_vel__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__robot_vel__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__robot_vel__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__robot_vel__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__robot_vel__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class RobotVel_Request(metaclass=Metaclass_RobotVel_Request):
    """Message class 'RobotVel_Request'."""

    __slots__ = [
        '_x_vel',
        '_y_vel',
        '_ang_vel',
        '_b1_vel',
        '_b2_vel',
        '_b3_vel',
        '_g1_vel',
    ]

    _fields_and_field_types = {
        'x_vel': 'float',
        'y_vel': 'float',
        'ang_vel': 'float',
        'b1_vel': 'float',
        'b2_vel': 'float',
        'b3_vel': 'float',
        'g1_vel': 'float',
    }

    SLOT_TYPES = (
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
        self.x_vel = kwargs.get('x_vel', float())
        self.y_vel = kwargs.get('y_vel', float())
        self.ang_vel = kwargs.get('ang_vel', float())
        self.b1_vel = kwargs.get('b1_vel', float())
        self.b2_vel = kwargs.get('b2_vel', float())
        self.b3_vel = kwargs.get('b3_vel', float())
        self.g1_vel = kwargs.get('g1_vel', float())

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
        if self.x_vel != other.x_vel:
            return False
        if self.y_vel != other.y_vel:
            return False
        if self.ang_vel != other.ang_vel:
            return False
        if self.b1_vel != other.b1_vel:
            return False
        if self.b2_vel != other.b2_vel:
            return False
        if self.b3_vel != other.b3_vel:
            return False
        if self.g1_vel != other.g1_vel:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def x_vel(self):
        """Message field 'x_vel'."""
        return self._x_vel

    @x_vel.setter
    def x_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'x_vel' field must be of type 'float'"
        self._x_vel = value

    @property
    def y_vel(self):
        """Message field 'y_vel'."""
        return self._y_vel

    @y_vel.setter
    def y_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'y_vel' field must be of type 'float'"
        self._y_vel = value

    @property
    def ang_vel(self):
        """Message field 'ang_vel'."""
        return self._ang_vel

    @ang_vel.setter
    def ang_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'ang_vel' field must be of type 'float'"
        self._ang_vel = value

    @property
    def b1_vel(self):
        """Message field 'b1_vel'."""
        return self._b1_vel

    @b1_vel.setter
    def b1_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'b1_vel' field must be of type 'float'"
        self._b1_vel = value

    @property
    def b2_vel(self):
        """Message field 'b2_vel'."""
        return self._b2_vel

    @b2_vel.setter
    def b2_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'b2_vel' field must be of type 'float'"
        self._b2_vel = value

    @property
    def b3_vel(self):
        """Message field 'b3_vel'."""
        return self._b3_vel

    @b3_vel.setter
    def b3_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'b3_vel' field must be of type 'float'"
        self._b3_vel = value

    @property
    def g1_vel(self):
        """Message field 'g1_vel'."""
        return self._g1_vel

    @g1_vel.setter
    def g1_vel(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'g1_vel' field must be of type 'float'"
        self._g1_vel = value


# Import statements for member types

# already imported above
# import rosidl_parser.definition


class Metaclass_RobotVel_Response(type):
    """Metaclass of message 'RobotVel_Response'."""

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
                'interfaces.srv.RobotVel_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__robot_vel__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__robot_vel__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__robot_vel__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__robot_vel__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__robot_vel__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class RobotVel_Response(metaclass=Metaclass_RobotVel_Response):
    """Message class 'RobotVel_Response'."""

    __slots__ = [
        '_sent',
    ]

    _fields_and_field_types = {
        'sent': 'int64',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.sent = kwargs.get('sent', int())

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
        if self.sent != other.sent:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def sent(self):
        """Message field 'sent'."""
        return self._sent

    @sent.setter
    def sent(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'sent' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'sent' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._sent = value


class Metaclass_RobotVel(type):
    """Metaclass of service 'RobotVel'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'interfaces.srv.RobotVel')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__robot_vel

            from interfaces.srv import _robot_vel
            if _robot_vel.Metaclass_RobotVel_Request._TYPE_SUPPORT is None:
                _robot_vel.Metaclass_RobotVel_Request.__import_type_support__()
            if _robot_vel.Metaclass_RobotVel_Response._TYPE_SUPPORT is None:
                _robot_vel.Metaclass_RobotVel_Response.__import_type_support__()


class RobotVel(metaclass=Metaclass_RobotVel):
    from interfaces.srv._robot_vel import RobotVel_Request as Request
    from interfaces.srv._robot_vel import RobotVel_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
