cmake_minimum_required(VERSION 3.5)
project(control_pkg)

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

add_executable(control_pid_p1_node src/control_pid_p1_node.cpp)
ament_target_dependencies(
  control_pid_p1_node
  rclcpp
  interfaces
  )

add_executable(control_pid_p2_node src/control_pid_p2_node.cpp)
ament_target_dependencies(
  control_pid_p2_node
  rclcpp
  interfaces
  )

add_executable(control_pid_r1_node src/control_pid_r1_node.cpp)
ament_target_dependencies(
  control_pid_r1_node
  rclcpp
  interfaces
  )

add_executable(control_pid_r2_node src/control_pid_r2_node.cpp)
ament_target_dependencies(
  control_pid_r2_node
  rclcpp
  interfaces
  )

install(TARGETS
  control_pid_p1_node
  control_pid_p2_node
  control_pid_r1_node
  control_pid_r2_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
