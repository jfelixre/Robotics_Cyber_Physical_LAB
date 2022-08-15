cmake_minimum_required(VERSION 3.5)
project(img_proc_pkg)

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
find_package(OpenCV 4.2 REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(interfaces REQUIRED)



add_executable(img_proc_node src/img_proc_node.cpp)
target_include_directories(img_proc_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(img_proc_node
  "OpenCV"
  sensor_msgs
  std_msgs
  rclcpp)



add_executable(img_publ_test_node src/img_publ_test_node.cpp)
target_include_directories(img_proc_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(img_publ_test_node
  "OpenCV"
  sensor_msgs
  std_msgs
  rclcpp
  cv_bridge)



  add_executable(img_subs_test_node src/img_subs_test_node.cpp)
target_include_directories(img_proc_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(img_subs_test_node
  "OpenCV"
  sensor_msgs
  std_msgs
  rclcpp
  cv_bridge)


add_executable(img_proc_id_node src/img_proc_id_node.cpp)
target_include_directories(img_proc_id_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(img_proc_id_node
  "OpenCV"
  sensor_msgs
  std_msgs
  rclcpp
  cv_bridge
  interfaces)


#include_directories(·${OpenCV_INCLUDE_DIRS}·)

install(TARGETS 
  img_proc_node
  img_publ_test_node
  img_subs_test_node
  img_proc_id_node
  DESTINATION lib/${PROJECT_NAME})



ament_package()
