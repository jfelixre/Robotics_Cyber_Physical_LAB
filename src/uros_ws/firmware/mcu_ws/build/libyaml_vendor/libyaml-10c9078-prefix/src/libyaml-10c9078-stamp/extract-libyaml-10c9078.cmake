# Make file names absolute:
#
get_filename_component(filename "/home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/build/libyaml_vendor/libyaml-10c9078-prefix/src/10c907871f1ccd779c7fccf6b81a62762b5c4e7b.zip" ABSOLUTE)
get_filename_component(directory "/home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/build/libyaml_vendor/libyaml-10c9078-prefix/src/libyaml-10c9078" ABSOLUTE)

message(STATUS "extracting...
     src='${filename}'
     dst='${directory}'")

if(NOT EXISTS "${filename}")
  message(FATAL_ERROR "error: file to extract does not exist: '${filename}'")
endif()

# Prepare a space for extracting:
#
set(i 1234)
while(EXISTS "${directory}/../ex-libyaml-10c9078${i}")
  math(EXPR i "${i} + 1")
endwhile()
set(ut_dir "${directory}/../ex-libyaml-10c9078${i}")
file(MAKE_DIRECTORY "${ut_dir}")

# Extract it:
#
message(STATUS "extracting... [tar xfz]")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${filename}
  WORKING_DIRECTORY ${ut_dir}
  RESULT_VARIABLE rv)

if(NOT rv EQUAL 0)
  message(STATUS "extracting... [error clean up]")
  file(REMOVE_RECURSE "${ut_dir}")
  message(FATAL_ERROR "error: extract of '${filename}' failed")
endif()

# Analyze what came out of the tar file:
#
message(STATUS "extracting... [analysis]")
file(GLOB contents "${ut_dir}/*")
list(REMOVE_ITEM contents "${ut_dir}/.DS_Store")
list(LENGTH contents n)
if(NOT n EQUAL 1 OR NOT IS_DIRECTORY "${contents}")
  set(contents "${ut_dir}")
endif()

# Move "the one" directory to the final directory:
#
message(STATUS "extracting... [rename]")
file(REMOVE_RECURSE ${directory})
get_filename_component(contents ${contents} ABSOLUTE)
file(RENAME ${contents} ${directory})

# Clean up:
#
message(STATUS "extracting... [clean up]")
file(REMOVE_RECURSE "${ut_dir}")

message(STATUS "extracting... done")
