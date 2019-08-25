#!/bin/sh

# Variable for sourcing catkin workspace setup file
SOURCE_CATKINWS="source ~/catkin_ws/devel/setup.bash"

# Deploy turtlebot in Gazebo
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_gazebo turtlebot_world.launch \
  world_file:=$(dirname $(pwd))/worlds/home.world" &
sleep 15

# SLAM
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_gazebo gmapping_demo.launch" &
sleep 5

# RViz
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_rviz_launchers view_navigation.launch" &
sleep 5

# Teleop
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_teleop keyboard_teleop.launch" &
