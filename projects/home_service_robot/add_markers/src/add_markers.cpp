#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <tf/tf.h>
#include <string>

#include "add_markers/AddMarker.h"

ros::Publisher publisher;

bool addMarker(add_markers::AddMarker::Request &req,
    add_markers::AddMarker::Response &res)
{
  visualization_msgs::Marker marker;

  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "add_marker";
  marker.type = visualization_msgs::Marker::CYLINDER;

  marker.id = req.id;

  if (req.action)
  {
    marker.action = visualization_msgs::Marker::ADD;
    res.response = "Added/modified marker " + std::to_string(marker.id);
  }
  else
  {
    marker.action = visualization_msgs::Marker::DELETE;
    res.response = "Deleted marker " + std::to_string(marker.id);
  }

  marker.pose.position.x = req.x;
  marker.pose.position.y = req.y;
  marker.pose.position.z = 0.0;
  marker.pose.orientation = tf::createQuaternionMsgFromYaw(req.yaw);

  marker.scale.x = 1.0;
  marker.scale.y = 1.0;
  marker.scale.z = 1.0;

  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration();

  publisher.publish(marker);
  ROS_INFO_STREAM(res.response);

  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;

  publisher = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::ServiceServer service = n.advertiseService("add_marker", addMarker);

  ros::spin();

  return 0;
}
