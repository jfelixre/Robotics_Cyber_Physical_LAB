# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:srv/ControlPidStart.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ControlPidStart_Request(type):
    """Metaclass of message 'ControlPidStart_Request'."""

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
                'interfaces.srv.ControlPidStart_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__control_pid_start__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__control_pid_start__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__control_pid_start__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__control_pid_start__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__control_pid_start__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ControlPidStart_Request(metaclass=Metaclass_ControlPidStart_Request):
    """Message class 'ControlPidStart_Request'."""

    __slots__ = [
        '_fase',
    ]

    _fields_and_field_types = {
        'fase': 'int64',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.fase = kwargs.get('fase', int())

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
        if self.fase != other.fase:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

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
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'fase' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._fase = value


# Import statements for member types

# already imported above
# import rosidl_parser.definition


class Metaclass_ControlPidStart_Response(type):
    """Metaclass of message 'ControlPidStart_Response'."""

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
                'interfaces.srv.ControlPidStart_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__control_pid_start__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__control_pid_start__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__control_pid_start__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__control_pid_start__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__control_pid_start__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ControlPidStart_Response(metaclass=Metaclass_ControlPidStart_Response):
    """Message class 'ControlPidStart_Response'."""

    __slots__ = [
        '_finish',
    ]

    _fields_and_field_types = {
        'finish': 'int64',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.finish = kwargs.get('finish', int())

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
        if self.finish != other.finish:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def finish(self):
        """Message field 'finish'."""
        return self._finish

    @finish.setter
    def finish(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'finish' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'finish' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._finish = value


class Metaclass_ControlPidStart(type):
    """Metaclass of service 'ControlPidStart'."""

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
                'interfaces.srv.ControlPidStart')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__control_pid_start

            from interfaces.srv import _control_pid_start
            if _control_pid_start.Metaclass_ControlPidStart_Request._TYPE_SUPPORT is None:
                _control_pid_start.Metaclass_ControlPidStart_Request.__import_type_support__()
            if _control_pid_start.Metaclass_ControlPidStart_Response._TYPE_SUPPORT is None:
                _control_pid_start.Metaclass_ControlPidStart_Response.__import_type_support__()


class ControlPidStart(metaclass=Metaclass_ControlPidStart):
    from interfaces.srv._control_pid_start import ControlPidStart_Request as Request
    from interfaces.srv._control_pid_start import ControlPidStart_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
