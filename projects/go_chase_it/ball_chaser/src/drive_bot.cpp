#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request &request,
    ball_chaser::DriveToTarget::Response &response)
{
  geometry_msgs::Twist motor_command;
  motor_command.linear.x = request.linear_x;
  motor_command.angular.z = request.angular_z;
  motor_command_publisher.publish(motor_command);

  response.msg_feedback = "Commanded robot: linear x = " + std::to_string(request.linear_x)
    + " , angular z = " + std::to_string(request.angular_z);
  ROS_INFO_STREAM(response.msg_feedback);

  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "drive_bot");
  ros::NodeHandle n;

  motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  
  ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot",
      handle_drive_request);

  ros::spin();

  return 0;
}
