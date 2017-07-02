cmake_minimum_required(VERSION 2.8.3)
project(rossubsciber)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy  gazebo_ros std_msgs genmsg message_generation)
find_package(gazebo REQUIRED)

## Declare ROS messages and services

generate_messages(DEPENDENCIES std_msgs)
catkin_package(CATKIN_DEPENDS message_runtime roscpp gazebo_ros std_msgs)

## Generate added messages and services

## Declare a catkin package

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(${roscpp_INCLUDE_DIRS})
include_directories(${std_msgs_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")
add_library(velodyne_plugin SHARED src/velodyne_plugin.cc)
target_link_libraries(velodyne_plugin ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})
add_dependencies(velodyne_plugin ${${PROJECT_NAME}_EXPORTED_TARGETS})

