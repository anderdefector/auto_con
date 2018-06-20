cmake_minimum_required(VERSION 2.8.3)
project(auto_con)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS

	actionlib
  actionlib_msgs
  cv_bridge
  image_transport
  sensor_msgs
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  std_msgs
)

find_package(OpenCV REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES auto_con
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}	
)

#Tests
add_executable(TEST1 src/Test1.cpp)
target_link_libraries(TEST1 ${catkin_LIBRARIES})

add_executable(TEST2 src/Test2.cpp)
target_link_libraries(TEST2 ${catkin_LIBRARIES})

add_executable(TEST4 src/TEST4.cpp)
target_link_libraries(TEST4 ${catkin_LIBRARIES})

add_executable(TEST5 src/Test5.cpp)
target_link_libraries(TEST5 ${catkin_LIBRARIES})

add_executable(TEST6 src/Test6.cpp)
target_link_libraries(TEST6 ${catkin_LIBRARIES})

add_executable(TEST7 src/Test7.cpp)
target_link_libraries(TEST7 ${catkin_LIBRARIES})

add_executable(imagen src/imagen.cpp)
target_link_libraries(imagen ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

#Controles
add_executable(MC src/MC.cpp)
target_link_libraries(MC ${catkin_LIBRARIES})


add_executable(CT src/CT.cpp)
target_link_libraries(CT ${catkin_LIBRARIES})
