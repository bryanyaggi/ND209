# Home Service Robot

## System Dependencies
- ROS Kinetic
- Gazebo 7 >=7.15.0
- RViz >=1.12.17

## ROS Package Dependencies
- ROS Navigation Stack
: `sudo apt-get ros-kinetic-navigation`
- turtlebot
: `git clone https://github.com/turtlebot/turtlebot.git`
- turtlebot_interactions
: `git clone https://github.com/turtlebot/turtlebot_interactions.git`
- turtlebot_simulator
: `git clone https://github.com/turtlebot/turtlebot_simulator.git`

## Project Packages
- pick_objects
: Sends navigation goals to navigation stack, requests marker updates from `add_markers`
- add_markers
: Services requests to update virtual markers in RViz

## Project Description
Initially, `test_slam.sh` is used to create a map of the world. `turtlebot_gazebo gmapping_demo.launch` handles SLAM while `turtlebot_teleop keyboard_teleop.launch` handles teleoperation for driving the robot in the world. The map can be saved using `map_server`.

`home_service.sh` uses the map previously generated via SLAM. `turtlebot_gazebo amcl_demo.launch` handles adaptive Monte Carlo localization. `add_markers` services requests to add virtual markers in RViz. `pick_objects` sends navigation goals to the navigation stack and requests marker updates from `add_markers`. The robot is tasked to pick up and drop off 2 virtual objects.

## Instructions
- Make sure the system dependencies are met and the ROS packages are added to the catkin workspace.
  - Clone the dependent packages to the catkin `src` directory.
  - One option for the project packages is to clone this repository elsewhere and symlink the packages to the catkin `src` directory.
- Build the packages.
- In the respository, navigate to the `scripts` directory and check the path for the catkin workspace setup file. Change if necessary.
- Run `./home_service.sh`.
  - Gazebo, RViz, and several emulated terminals will appear, and the demo will commence.

![](https://github.com/bryanyaggi/ND209/tree/master/projects/home_service_robot/images/ss.png)
