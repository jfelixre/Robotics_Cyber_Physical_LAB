cmake_minimum_required(VERSION 3.5)
project(interfaces)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/PlatformVel.srv"
  "msg/MotorVels.msg"
  "msg/ImgData.msg"
  "srv/ControlPidStart.srv"
  "msg/PlatState.msg"
  "msg/ControlData.msg"
  "msg/MotorVelsWArm.msg"
  "msg/LimitSwitch.msg"
  "srv/RobotVel.srv"
  "msg/RobotState.msg"
  "msg/Positions.msg"
  "srv/AStarService.srv"
  "msg/Path.msg"
  "msg/RobotObjective.msg"
  "srv/EventControl.srv"
  "srv/TrajectoryControl.srv"
  "msg/MotorPlatformVels.msg"
  "msg/MotorArmVels.msg"
  "msg/ArmJointsPositions.msg"
  "msg/DataControl.msg"
  "msg/PlatformVel.msg"
  "msg/TrajectoryControl.msg"
  "msg/NewTaskMsg.msg"
  "msg/TaskReport.msg"
  "msg/TaskDescription.msg"
  "msg/TaskMsg.msg"
  "srv/TaskListService.srv"
  "msg/PositionTag.msg"
  DEPENDENCIES geometry_msgs
 )



ament_package()
