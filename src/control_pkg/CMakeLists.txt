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
target_include_directories(compute_trajectory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)


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
  target_include_directories(a_star_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

  ament_target_dependencies(a_star_server
    sensor_msgs
    std_msgs
    rclcpp
    interfaces
    tf2
    tf2_ros
    )

  add_executable(compute_trajectory_r1 src/compute_trajectory_r1.cpp)
  target_include_directories(compute_trajectory_r1 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

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

  add_executable(compute_trajectory_r2 src/compute_trajectory_r2.cpp)
  target_include_directories(compute_trajectory_r2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

  ament_target_dependencies(compute_trajectory_r2
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
  target_include_directories(event_driven_control_r1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

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

  add_executable(control_trajectory_r1 src/control_trajectory_r1.cpp)
  target_include_directories(control_trajectory_r1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

  ament_target_dependencies(control_trajectory_r1
    sensor_msgs
    std_msgs
    rclcpp
    cv_bridge
    interfaces
    tf2
    tf2_ros
    geometry_msgs
    Eigen3
    )

  add_executable(control_trajectory_r2 src/control_trajectory_r2.cpp)
  target_include_directories(control_trajectory_r2 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

  ament_target_dependencies(control_trajectory_r2
    sensor_msgs
    std_msgs
    rclcpp
    cv_bridge
    interfaces
    tf2
    tf2_ros
    geometry_msgs
    Eigen3
    )

  add_executable(test_control2 src/test_control2.cpp)
  target_include_directories(test_control2 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

  ament_target_dependencies(test_control2
    sensor_msgs
    std_msgs
    rclcpp
    cv_bridge
    interfaces
    tf2
    tf2_ros
    geometry_msgs
    )

    add_executable(test_control3 src/test_control3.cpp)
    target_include_directories(test_control3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
  
    ament_target_dependencies(test_control3
      sensor_msgs
      std_msgs
      rclcpp
      cv_bridge
      interfaces
      tf2
      tf2_ros
      geometry_msgs
      )

    add_executable(control_arm_pid_r1 src/control_arm_pid_r1.cpp)
    target_include_directories(control_arm_pid_r1 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
  
    ament_target_dependencies(control_arm_pid_r1
      sensor_msgs
      std_msgs
      rclcpp
      cv_bridge
      interfaces
      tf2
      tf2_ros
      geometry_msgs
      Eigen3
      )



install(TARGETS
  #control_pid_p1_node
  #control_pid_p2_node
  #control_pid_r1_node
  #control_pid_r2_node
  #compute_trajectory
  a_star_server
  compute_trajectory_r1
  event_driven_control_r1
  control_trajectory_r1
  #test_control2
  #test_control3
  control_arm_pid_r1
  compute_trajectory_r2
  control_trajectory_r2
  DESTINATION lib/${PROJECT_NAME})

ament_package()
