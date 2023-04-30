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
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Eigen3 REQUIRED)


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

add_executable(compute_trajectory src/compute_trajectory.cpp)
ament_target_dependencies(compute_trajectory
  sensor_msgs
  std_msgs
  rclcpp
  cv_bridge
  interfaces
  tf2
  tf2_ros
  geometry_msgs
  )

  add_executable(a_star_server src/a_star_server.cpp)
  ament_target_dependencies(a_star_server
    sensor_msgs
    std_msgs
    rclcpp
    interfaces
    tf2
    tf2_ros
    )

  add_executable(compute_trajectory_r1 src/compute_trajectory_r1.cpp)
  ament_target_dependencies(compute_trajectory_r1
    sensor_msgs
    std_msgs
    rclcpp
    cv_bridge
    interfaces
    tf2
    tf2_ros
    geometry_msgs
    )

  add_executable(event_driven_control_r1 src/event_driven_control_r1.cpp)
  ament_target_dependencies(event_driven_control_r1
    sensor_msgs
    std_msgs
    rclcpp
    cv_bridge
    interfaces
    tf2
    tf2_ros
    geometry_msgs
    )

install(TARGETS
  control_pid_p1_node
  control_pid_p2_node
  control_pid_r1_node
  control_pid_r2_node
  compute_trajectory
  a_star_server
  compute_trajectory_r1
  event_driven_control_r1
  DESTINATION lib/${PROJECT_NAME})

ament_package()
