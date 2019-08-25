#!/bin/sh

# Variable for sourcing catkin workspace setup file
SOURCE_CATKINWS="source ~/catkin_ws/devel/setup.bash"

# Deploy turtlebot in Gazebo
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_gazebo turtlebot_world.launch \
  world_file:=$(dirname $(pwd))/worlds/my_world_simple.world" &
sleep 20

# AMCL
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_gazebo amcl_demo.launch \
  map_file:=$(dirname $(pwd))/maps/my_world_simple_pgmcreator.yaml" &
sleep 5

# RViz
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_rviz_launchers view_navigation.launch" &
