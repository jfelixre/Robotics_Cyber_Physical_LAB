cmake_minimum_required(VERSION 3.5)
project(inv_kinematics_pkg)

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
find_package(rclcpp REQUIRED)
find_package(interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)


add_executable(platform1_vel_server src/platform1_vel_server.cpp)
ament_target_dependencies(
  platform1_vel_server
  rclcpp
  interfaces
  geometry_msgs
  )

add_executable(platform1_vel_test_client src/platform1_vel_test_client.cpp)
ament_target_dependencies(
  platform1_vel_test_client
  rclcpp
  interfaces
  )

add_executable(platform2_vel_server src/platform2_vel_server.cpp)
ament_target_dependencies(
  platform2_vel_server
  rclcpp
  interfaces
  geometry_msgs
  )

add_executable(platform2_vel_test_client src/platform2_vel_test_client.cpp)
ament_target_dependencies(
  platform2_vel_test_client
  rclcpp
  interfaces
  )

add_executable(robot1_vel_server src/robot1_vel_server.cpp)
ament_target_dependencies(
  robot1_vel_server
  rclcpp
  interfaces
  geometry_msgs
  )

add_executable(robot2_vel_server src/robot2_vel_server.cpp)
ament_target_dependencies(
  robot2_vel_server
  rclcpp
  interfaces
  geometry_msgs
  )

add_executable(robot_1_platform_vel_server src/robot_1_platform_vel_server.cpp)
ament_target_dependencies(
  robot_1_platform_vel_server
  rclcpp
  interfaces
  geometry_msgs
  )

add_executable(robot_2_platform_vel_server src/robot_2_platform_vel_server.cpp)
ament_target_dependencies(
  robot_2_platform_vel_server
  rclcpp
  interfaces
  geometry_msgs
  )

install(TARGETS
  platform1_vel_server
  platform1_vel_test_client
  platform2_vel_server
  platform2_vel_test_client
  robot1_vel_server
  robot2_vel_server
  robot_1_platform_vel_server
  robot_2_platform_vel_server
  DESTINATION lib/${PROJECT_NAME})


ament_package()
