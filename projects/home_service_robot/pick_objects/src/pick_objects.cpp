#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/tf.h>
#include <math.h>
#include <string>
#include <vector>

#include "add_markers/AddMarker.h"

ros::ServiceClient client;

/*
 * Class for storing navigation goal
 */
class Goal
{
  public:
    std::string name; // for messages
    int id; // marker ID
    bool pickup; // represents pick-up location
    double x;
    double y;
    double yaw;

    Goal(std::string nameVal, int idVal, double xVal, double yVal,
        double yawVal, bool pickupVal)
    {
      name = nameVal;
      id = idVal;
      x = xVal;
      y = yVal;
      yaw = yawVal;
      pickup = pickupVal;

      if (pickup)
      {
        // Request marker to be added in RViz
        add_markers::AddMarker srv;
        srv.request.id = id;
        srv.request.x = x;
        srv.request.y = y;
        srv.request.yaw = yaw;
        srv.request.action = true;

        if (client.call(srv))
        {
          ROS_INFO("Added pickup marker %d.", id);
        }
        else
        {
          ROS_INFO("Failed to add pickup marker %d.", id);
        }
      }
    }
};

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

/*
 * Completes navigation goal.
 * Sends MoveBaseGoal message and waits for completion.
 */
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
  ros::NodeHandle n;

  client = n.serviceClient<add_markers::AddMarker>("add_marker");
  MoveBaseClient ac("move_base", true);

  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up.");
  }

  move_base_msgs::MoveBaseGoal mbg;

  mbg.target_pose.header.frame_id = "map";

  std::vector<Goal> goals;
  int id = 0;
  goals.push_back(Goal("pick-up 1", id, 2.0, 2.0, M_PI/4, true));
  goals.push_back(Goal("drop-off 1", id, -5.0, -5.0, 5*M_PI/4, false));
  id++;
  goals.push_back(Goal("pick-up 2", id, -5.0, 2.0, M_PI/2, true));
  goals.push_back(Goal("drop-off 2", id, -5.0, -5.0, 3*M_PI/2, false));

  for (auto& goal : goals)
  {
    if (completeGoal(ac, mbg, goal))
    {
      ros::Duration(5.0).sleep();

      // Update marker
      add_markers::AddMarker srv;
      srv.request.id = goal.id;
      srv.request.x = goal.x;
      srv.request.y = goal.y;
      srv.request.yaw = goal.yaw;
      if (goal.pickup)
      {
        srv.request.action = false; // delete marker
      }
      else
      {
        srv.request.action = true; // add marker
      }
      if (client.call(srv))
      {
        ROS_INFO("Updated marker.");
      }
      else
      {
        ROS_INFO("Failed to update marker.");
      }
    }
    else
    {
      break;
    }
  }

  return 0;
}
