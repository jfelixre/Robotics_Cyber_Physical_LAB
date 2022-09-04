cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

PROJECT( CameraCalibration )

find_package( OpenCV 4 REQUIRED ) #find_package( OpenCV 4 REQUIRED )
find_package( aruco REQUIRED ) #find_package( OpenCV 4 REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS})

MACRO(add_example name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS})
ENDMACRO()

#add_example(CalibrationUsingCpp1)
add_example(CalibrationUsingCpp2)
#add_example(ArucoDetectionCpp)


