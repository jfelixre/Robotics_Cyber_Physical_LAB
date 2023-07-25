cmake_minimum_required(VERSION 3.5)
project(gazebo_plugin_sim)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # we dont use add_compile_options with pedantic in message packages
  # because the Python C extensions dont comply with it
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-inconsistent-missing-override")
endif()

if(WIN32)
  add_compile_definitions(
    # For math constants
    _USE_MATH_DEFINES
    # Minimize Windows namespace collision
    NOMINMAX
    WIN32_LEAN_AND_MEAN
  )
endif()

find_package(ament_cmake REQUIRED)
find_package(camera_info_manager REQUIRED)
find_package(gazebo_dev REQUIRED)
find_package(gazebo_msgs REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(interfaces REQUIRED)

link_directories(${gazebo_dev_LIBRARY_DIRS})

option(ENABLE_PROFILER "Enable Ignition Profiler" FALSE)
if(ENABLE_PROFILER)
  find_package(ignition-common3 QUIET COMPONENTS profiler)
  if(ignition-common3_FOUND)
    add_definitions("-DIGN_PROFILER_ENABLE=1" "-DIGN_PROFILER_REMOTERY=1")
    message(STATUS "Profiler is active")
  else()
    message(WARNING "Can't find Ignition common3. Profiler will not be actived")
    add_definitions("-DIGN_PROFILER_ENABLE=0" "-DIGN_PROFILER_REMOTERY=0")
  endif()
endif()

# Platform Pulgin Test
add_library(platform_plugin_test SHARED
  src/platform_plugin_test.cpp
)
target_include_directories(platform_plugin_test PUBLIC include)
ament_target_dependencies(platform_plugin_test
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_msgs"
)
ament_export_libraries(platform_plugin_test)
if(ENABLE_PROFILER)
  target_include_directories(platform_plugin_test PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(platform_plugin_test ${ignition-common3_LIBRARIES})
endif()


# Platform 1 Pulgin
add_library(platform1_plugin SHARED
  src/platform1_plugin.cpp
)
target_include_directories(platform1_plugin PUBLIC include)
ament_target_dependencies(platform1_plugin
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_msgs"
  "interfaces"
)
ament_export_libraries(platform1_plugin)
if(ENABLE_PROFILER)
  target_include_directories(platform1_plugin PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(platform1_plugin ${ignition-common3_LIBRARIES})
endif()


# Platform 2 Pulgin
add_library(platform2_plugin SHARED
  src/platform2_plugin.cpp
)
target_include_directories(platform2_plugin PUBLIC include)
ament_target_dependencies(platform2_plugin
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_msgs"
  "interfaces"
)
ament_export_libraries(platform2_plugin)
if(ENABLE_PROFILER)
  target_include_directories(platform2_plugin PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(platform2_plugin ${ignition-common3_LIBRARIES})
endif()


# Robot 1 Pulgin
add_library(robot1_plugin SHARED
  src/robot1_plugin.cpp
)
target_include_directories(robot1_plugin PUBLIC include)
ament_target_dependencies(robot1_plugin
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_msgs"
  "interfaces"
)
ament_export_libraries(robot1_plugin)
if(ENABLE_PROFILER)
  target_include_directories(robot1_plugin PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(robot1_plugin ${ignition-common3_LIBRARIES})
endif()

# Robot 2 Pulgin
add_library(robot2_plugin SHARED
  src/robot2_plugin.cpp
)
target_include_directories(robot2_plugin PUBLIC include)
ament_target_dependencies(robot2_plugin
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_msgs"
  "interfaces"
)
ament_export_libraries(robot2_plugin)
if(ENABLE_PROFILER)
  target_include_directories(robot2_plugin PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(robot2_plugin ${ignition-common3_LIBRARIES})
endif()

# Real Time Factor Pulgin
add_library(real_time_factor_plugin SHARED
  src/real_time_factor_plugin.cpp
)
target_include_directories(real_time_factor_plugin PUBLIC include)
ament_target_dependencies(real_time_factor_plugin
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_msgs"
  "interfaces"
)
ament_export_libraries(real_time_factor_plugin)
if(ENABLE_PROFILER)
  target_include_directories(real_time_factor_plugin PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(real_time_factor_plugin ${ignition-common3_LIBRARIES})
endif()


ament_export_include_directories(include)
ament_export_dependencies(rclcpp)
ament_export_dependencies(gazebo_dev)
ament_export_dependencies(gazebo_msgs)
ament_export_dependencies(gazebo_ros)
ament_export_dependencies(std_msgs)



if(NOT WIN32)
  if(NOT APPLE)
    set(
      AMENT_CMAKE_ENVIRONMENT_HOOKS_DESC_gazebo_plugin_sim
      "prepend-non-duplicate;LD_LIBRARY_PATH;${GAZEBO_PLUGIN_PATH}")
  else()
    set(
      AMENT_CMAKE_ENVIRONMENT_HOOKS_DESC_gazebo_plugin_sim
      "prepend-non-duplicate;DYLD_LIBRARY_PATH;${GAZEBO_PLUGIN_PATH}")
  endif()
endif()
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/env-hooks/gazebo_plugin_sim.sh.in")

ament_package()

install(DIRECTORY include/
        DESTINATION include)

install(TARGETS
  platform_plugin_test
  platform1_plugin
  platform2_plugin
  robot1_plugin
  robot2_plugin
  real_time_factor_plugin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)


install(DIRECTORY
  worlds
  DESTINATION share/${PROJECT_NAME}/
)
