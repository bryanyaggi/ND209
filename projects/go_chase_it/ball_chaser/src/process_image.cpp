#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <string>

/**
 * State enumeration for robot driving states
 */
enum State {STOPPED, TURNING_LEFT, TURNING_RIGHT, DRIVING_STRAIGHT};

ros::ServiceClient client;

bool drive_robot(float linear_x, float angular_z)
{
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = linear_x;
  srv.request.angular_z = angular_z;

  if (!client.call(srv))
  {
    ROS_ERROR("Failed to call service drive_bot");
    return false;
  }
  return true;
}

void process_image_callback(const sensor_msgs::Image image)
{
  static State state = STOPPED;
  uint8_t target_pixel_rgb_vals[] = {255, 255, 255}; // RGB values of white pixel

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

  // Determine next state
  State next_state;
  if (num_target_pixels == 0)
  {
    next_state = STOPPED;
  }
  else
  {
    avg_target_pixel_column /= num_target_pixels;
    if (avg_target_pixel_column < (float) image.width / 3)
    {
      next_state = TURNING_LEFT;
    }
    else if (avg_target_pixel_column > (float) image.width * 2 / 3)
    {
      next_state = TURNING_RIGHT;
    }
    else
    {
      next_state = DRIVING_STRAIGHT;
    }
  }

  // Command robot if necessary
  if (next_state != state)
  {
    float linear_x, angular_z;
    switch (next_state)
    {
      case STOPPED:
        linear_x = 0.0;
        angular_z = 0.0;
        break;
      case TURNING_LEFT:
        linear_x = 0.0;
        angular_z = 0.5;
        break;
      case TURNING_RIGHT:
        linear_x = 0.0;
        angular_z = -0.5;
        break;
      case DRIVING_STRAIGHT:
        linear_x = 0.5;
        angular_z = 0.0;
        break;
    }

    if (drive_robot(linear_x, angular_z))
    {
      state = next_state;
    }
  }
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
