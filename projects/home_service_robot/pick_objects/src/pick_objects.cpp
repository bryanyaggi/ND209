#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/tf.h>
#include <math.h>
#include <string>
#include <vector>

#include "add_markers/AddMarker.h"

class Goal
{
  public:
    std::string name;
    double x;
    double y;
    double yaw;

    Goal(std::string nameVal, double xVal, double yVal, double yawVal)
    {
      name = nameVal;
      x = xVal;
      y = yVal;
      yaw = yawVal;
    }
};

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

bool completeGoal(MoveBaseClient &ac, move_base_msgs::MoveBaseGoal &mbg, Goal goal)
{
  mbg.target_pose.header.stamp = ros::Time::now();
  mbg.target_pose.pose.position.x = goal.x;
  mbg.target_pose.pose.position.y = goal.y;
  mbg.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(goal.yaw);
  
  ROS_INFO("Sending %s location goal.", goal.name.c_str());
  ac.sendGoal(mbg);

  ac.waitForResult();

  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("Reached %s location.", goal.name.c_str());
  }
  else
  {
    ROS_INFO("Failed to reach %s location.", goal.name.c_str());
    return false;
  }
  
  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pick_objects");

  MoveBaseClient ac("move_base", true);

  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up.");
  }

  move_base_msgs::MoveBaseGoal mbg;

  mbg.target_pose.header.frame_id = "map";

  std::vector<Goal> goals;
  goals.push_back(Goal("pick-up", 2.0, 2.0, M_PI/4));
  goals.push_back(Goal("drop-off", -2.0, -2.0, 5*M_PI/4));

  for (auto& goal : goals)
  {
    if (completeGoal(ac, mbg, goal))
    {
      ros::Duration(5.0).sleep();
    }
    else
    {
      break;
    }
  }

  return 0;
}
