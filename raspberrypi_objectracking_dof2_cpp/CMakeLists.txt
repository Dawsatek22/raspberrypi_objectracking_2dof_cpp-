cmake_minimum_required(VERSION 3.25.1) # the current cmake version used
project(raspberry_tracking_robot_dof2)

set(CMAKE_CXX_STANDARD 20) # current standard



# below are the current packages used
find_package(PiPCA9685 REQUIRED)
find_package(OpenCV REQUIRED) # opencv pkg setup
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

include_directories(${LIBCAMERA_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRECTORIES})

# below are the executables with libraries linked

add_executable(calib src/calibrate.cpp)
add_executable(track_picamx src/track_picamx.cpp)
add_executable(track_picamy src/track_picamy.cpp)
add_executable(main src/main.cpp)
add_executable(yellow_picamx src/yellow_picamx.cpp)
add_executable(yellow_picamy src/yellow_picamy.cpp)

target_link_libraries(yellow_picamx  -llccv ${OpenCV_LIBS} )
target_link_libraries(yellow_picamy  -llccv ${OpenCV_LIBS} )
target_link_libraries(track_picamx PiPCA9685::PiPCA9685 -llccv ${OpenCV_LIBS} )
target_link_libraries(track_picamy PiPCA9685::PiPCA9685 -llccv ${OpenCV_LIBS} )
target_link_libraries(main PiPCA9685::PiPCA9685 -llccv ${OpenCV_LIBS} )
target_link_libraries(calib PiPCA9685::PiPCA9685)


