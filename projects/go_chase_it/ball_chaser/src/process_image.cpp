#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <string>

ros::ServiceClient client;

void drive_robot(float linear_x, float angular_z)
{
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = linear_x;
  srv.request.angular_z = angular_z;

  if (!client.call(srv))
  {
    ROS_ERROR("Failed to call service drive_bot");
  }
}

void process_image_callback(const sensor_msgs::Image image)
{
  uint8_t target_pixel_rgb_vals[] = {255, 255, 255};

  int num_target_pixels = 0;
  float avg_target_pixel_column = 0.0;
  for (int row = 0; row < image.height; row++)
  {
    for (int col = 0; col < image.width; col += sizeof(target_pixel_rgb_vals))
    {
      bool target_pixel = true;
      for (int i = 0; i < sizeof(target_pixel_rgb_vals); i++)
      {
        if (image.data[(row * image.step) + (col * sizeof(target_pixel_rgb_vals)) + i]
            != target_pixel_rgb_vals[i])
        {
          target_pixel = false;
          break;
        }
      }

      if (target_pixel)
      {
        num_target_pixels++;
        avg_target_pixel_column += col;
      }
    }
  }

  if (num_target_pixels == 0)
  {
    drive_robot(0.0, 0.0); // stop
  }
  else
  {
    avg_target_pixel_column /= num_target_pixels;
    if (avg_target_pixel_column < (float) image.width / 3)
    {
      drive_robot(0.0, 0.5); // turn left
    }
    else if (avg_target_pixel_column > (float) image.width * 2 / 3)
    {
      drive_robot(0.0, -0.5); // turn right
    }
    else
    {
      drive_robot(0.5, 0.0); // drive straight
    }
  }
  ROS_INFO_STREAM("num target pixels = " + std::to_string(num_target_pixels) + " avg column = " + std::to_string(avg_target_pixel_column));
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "process_image");
  ros::NodeHandle n;

  client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

  ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw",
      10, process_image_callback);

  ros::spin();

  return 0;
}
