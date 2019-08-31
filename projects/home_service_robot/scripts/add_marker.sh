#!/bin/sh

# Variable for sourcing catkin workspace setup file
SOURCE_CATKINWS="source ~/catkin_ws/devel/setup.bash"

# Deploy turtlebot in Gazebo
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_gazebo turtlebot_world.launch \
  world_file:=$(dirname $(pwd))/worlds/earthquake.world" &
sleep 20

# AMCL
xterm -e "$SOURCE_CATKINWS && roslaunch turtlebot_gazebo amcl_demo.launch \
  map_file:=$(dirname $(pwd))/maps/earthquake_slam.yaml" &
sleep 5

# RViz
xterm -e "$SOURCE_CATKINWS && rosrun rviz rviz \
  -d $(dirname $(pwd))/rvizConfig/configuration.rviz" &
sleep 5

# Start add markers node
xterm -e "$SOURCE_CATKINWS && rosrun add_markers add_markers" &
