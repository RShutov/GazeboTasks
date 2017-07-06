#!/bin/bash
source /opt/ros/kinetic/setup.bash
source $HOME/catkin_ws/devel/setup.bash
rospack find rossubscriber
roslaunch rossubscriber Main.launch
