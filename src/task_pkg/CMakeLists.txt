cmake_minimum_required(VERSION 3.5)
project(task_pkg)

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


add_executable(task_scheduler_node src/task_scheduler_node.cpp)
ament_target_dependencies(
  task_scheduler_node
  rclcpp
  interfaces
  geometry_msgs
  )

add_executable(task_manager_node src/task_manager_node.cpp)
ament_target_dependencies(
  task_manager_node
  rclcpp
  interfaces
  geometry_msgs
  )

  add_executable(task_manager_node_client src/task_manager_node_client.cpp)
  ament_target_dependencies(
    task_manager_node_client
    rclcpp
    interfaces
    geometry_msgs
    )

  add_executable(task_scheduler_node_service src/task_scheduler_node_service.cpp)
  ament_target_dependencies(
    task_scheduler_node_service
    rclcpp
    interfaces
    geometry_msgs
    )
  
  add_executable(heartbeat_checker_node src/heartbeat_checker_node.cpp)
  ament_target_dependencies(
    heartbeat_checker_node
    rclcpp
    interfaces
    geometry_msgs
  )


install(TARGETS
  task_scheduler_node
  task_manager_node
  task_manager_node_client
  task_scheduler_node_service
  heartbeat_checker_node
  DESTINATION lib/${PROJECT_NAME})



ament_package()
