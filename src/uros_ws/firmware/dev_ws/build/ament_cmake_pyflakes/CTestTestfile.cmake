# CMake generated Testfile for 
# Source directory: /home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes
# Build directory: /home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(copyright "/usr/bin/python3" "-u" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes/test_results/ament_cmake_pyflakes/copyright.xunit.xml" "--package-name" "ament_cmake_pyflakes" "--output-file" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes/ament_copyright/copyright.txt" "--command" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_copyright/bin/ament_copyright" "--xunit-file" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes/test_results/ament_cmake_pyflakes/copyright.xunit.xml")
set_tests_properties(copyright PROPERTIES  LABELS "copyright;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes" _BACKTRACE_TRIPLES "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;118;add_test;/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_cmake_copyright/share/ament_cmake_copyright/cmake/ament_copyright.cmake;41;ament_add_test;/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes/CMakeLists.txt;19;ament_copyright;/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes/CMakeLists.txt;0;")
add_test(lint_cmake "/usr/bin/python3" "-u" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes/test_results/ament_cmake_pyflakes/lint_cmake.xunit.xml" "--package-name" "ament_cmake_pyflakes" "--output-file" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes/ament_lint_cmake/lint_cmake.txt" "--command" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_lint_cmake/bin/ament_lint_cmake" "--xunit-file" "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/build/ament_cmake_pyflakes/test_results/ament_cmake_pyflakes/lint_cmake.xunit.xml")
set_tests_properties(lint_cmake PROPERTIES  LABELS "lint_cmake;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes" _BACKTRACE_TRIPLES "/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;118;add_test;/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/install/ament_cmake_lint_cmake/share/ament_cmake_lint_cmake/cmake/ament_lint_cmake.cmake;41;ament_add_test;/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes/CMakeLists.txt;22;ament_lint_cmake;/home/javierfr/microros_ws/uros_ws/firmware/dev_ws/ament/ament_lint/ament_cmake_pyflakes/CMakeLists.txt;0;")
