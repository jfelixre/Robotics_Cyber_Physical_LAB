# CMake generated Testfile for 
# Source directory: /home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs
# Build directory: /home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(copyright "/home/javierfr/.espressif/python_env/idf5.2_py3.8_env/bin/python3" "-u" "/opt/ros/foxy/share/ament_cmake_test/cmake/run_test.py" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/test_results/micro_ros_msgs/copyright.xunit.xml" "--package-name" "micro_ros_msgs" "--output-file" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/ament_copyright/copyright.txt" "--command" "/opt/ros/foxy/bin/ament_copyright" "--xunit-file" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/test_results/micro_ros_msgs/copyright.xunit.xml")
set_tests_properties(copyright PROPERTIES  LABELS "copyright;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs" _BACKTRACE_TRIPLES "/opt/ros/foxy/share/ament_cmake_test/cmake/ament_add_test.cmake;118;add_test;/opt/ros/foxy/share/ament_cmake_copyright/cmake/ament_copyright.cmake;41;ament_add_test;/opt/ros/foxy/share/ament_cmake_copyright/cmake/ament_cmake_copyright_lint_hook.cmake;18;ament_copyright;/opt/ros/foxy/share/ament_cmake_copyright/cmake/ament_cmake_copyright_lint_hook.cmake;0;;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/opt/ros/foxy/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/opt/ros/foxy/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs/CMakeLists.txt;36;ament_package;/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs/CMakeLists.txt;0;")
add_test(lint_cmake "/home/javierfr/.espressif/python_env/idf5.2_py3.8_env/bin/python3" "-u" "/opt/ros/foxy/share/ament_cmake_test/cmake/run_test.py" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/test_results/micro_ros_msgs/lint_cmake.xunit.xml" "--package-name" "micro_ros_msgs" "--output-file" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/ament_lint_cmake/lint_cmake.txt" "--command" "/opt/ros/foxy/bin/ament_lint_cmake" "--xunit-file" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/test_results/micro_ros_msgs/lint_cmake.xunit.xml")
set_tests_properties(lint_cmake PROPERTIES  LABELS "lint_cmake;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs" _BACKTRACE_TRIPLES "/opt/ros/foxy/share/ament_cmake_test/cmake/ament_add_test.cmake;118;add_test;/opt/ros/foxy/share/ament_cmake_lint_cmake/cmake/ament_lint_cmake.cmake;41;ament_add_test;/opt/ros/foxy/share/ament_cmake_lint_cmake/cmake/ament_cmake_lint_cmake_lint_hook.cmake;21;ament_lint_cmake;/opt/ros/foxy/share/ament_cmake_lint_cmake/cmake/ament_cmake_lint_cmake_lint_hook.cmake;0;;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/opt/ros/foxy/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/opt/ros/foxy/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs/CMakeLists.txt;36;ament_package;/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs/CMakeLists.txt;0;")
add_test(xmllint "/home/javierfr/.espressif/python_env/idf5.2_py3.8_env/bin/python3" "-u" "/opt/ros/foxy/share/ament_cmake_test/cmake/run_test.py" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/test_results/micro_ros_msgs/xmllint.xunit.xml" "--package-name" "micro_ros_msgs" "--output-file" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/ament_xmllint/xmllint.txt" "--command" "/opt/ros/foxy/bin/ament_xmllint" "--xunit-file" "/home/javierfr/microros_ws/uros_ws/build/micro_ros_msgs/test_results/micro_ros_msgs/xmllint.xunit.xml")
set_tests_properties(xmllint PROPERTIES  LABELS "xmllint;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs" _BACKTRACE_TRIPLES "/opt/ros/foxy/share/ament_cmake_test/cmake/ament_add_test.cmake;118;add_test;/opt/ros/foxy/share/ament_cmake_xmllint/cmake/ament_xmllint.cmake;50;ament_add_test;/opt/ros/foxy/share/ament_cmake_xmllint/cmake/ament_cmake_xmllint_lint_hook.cmake;18;ament_xmllint;/opt/ros/foxy/share/ament_cmake_xmllint/cmake/ament_cmake_xmllint_lint_hook.cmake;0;;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/opt/ros/foxy/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/opt/ros/foxy/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/opt/ros/foxy/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs/CMakeLists.txt;36;ament_package;/home/javierfr/microros_ws/uros_ws/src/uros/micro_ros_msgs/CMakeLists.txt;0;")
subdirs("micro_ros_msgs__py")
