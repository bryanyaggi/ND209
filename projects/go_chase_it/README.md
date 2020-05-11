# Go Chase It!

## System Dependencies
- ROS Kinetic
- Gazebo 7 >=7.15.0
- RViz >=1.12.17

## Project Packages
- ball_chaser
: Controls robot to follow ball around world
- my_robot
: Files for robot and world

## Project Description
- A custom robot and a ball are placed in the world. The robot senses the ball using its camera and follows it. The `process_image` node subscribes to camera images and processes them to determine the location of the ball relative to the robot. It sends requests to the `drive_bot` node which drives the robot.

## Instructions
- Make sure the system dependencies are met and the packages are added to the catkin workspace.
- Build the project.
- Run `roslaunch my_robot world.launch` to launch ROS, Gazebo, and RViz with the robot and ball in the world.
- Run `roslaunch ball_chaser ball_chaser.launch` to start the `process_image` and `drive_bot` nodes.
- Move the ball around the world and watch the robot follow.
