#include "fl/Headers.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <ParticleFilter.hpp>
#include <thread>

#include "../include/functions.hpp"
#include "../include/GazeboFunctions.hpp"

int main(int _argc, char **_argv) {
  // Load image
  cv::Mat image = cv::imread("../../Gazebo/models/bigworld/meshes/floor_plan.png");
  cv::Mat resize_image;
  cv::resize(image, resize_image, cv::Size(), 7, 7, cv::INTER_NEAREST);


  // Load gazebo
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Listen to Gazebo topics
  gazebo::transport::SubscriberPtr statSubscriber =
      node->Subscribe("~/world_stats", statCallback);

  gazebo::transport::SubscriberPtr poseSubscriber =
      node->Subscribe("~/pose/info", poseCallback);

  gazebo::transport::SubscriberPtr cameraSubscriber =
      node->Subscribe("~/pioneer2dx/camera/link/camera/image", cameraCallback);

  gazebo::transport::SubscriberPtr lidarSubscriber =
      node->Subscribe("~/pioneer2dx/hokuyo/link/laser/scan", lidarCallback);

  // Publish to the robot vel_cmd topic
  gazebo::transport::PublisherPtr movementPublisher =
      node->Advertise<gazebo::msgs::Pose>("~/pioneer2dx/vel_cmd");

  // Publish a reset of the world
  gazebo::transport::PublisherPtr worldPublisher =
      node->Advertise<gazebo::msgs::WorldControl>("~/world_control");
  gazebo::msgs::WorldControl controlMessage;
  controlMessage.mutable_reset()->set_all(true);
  worldPublisher->WaitForConnection();
  worldPublisher->Publish(controlMessage);

  const int key_left = 81;
  const int key_up = 82;
  const int key_down = 84;
  const int key_right = 83;
  const int key_esc = 27;
  const int key_w = 119;
  const int key_a = 97;
  const int key_s = 115;
  const int key_d = 100;
  const int key_space = 32;

  float speed = 0.0;
  float dir = 0.0;

  cv::Point topLeftCorner = cv::Point(-34.89 , 24.6);
  cv::Point buttomRightCorner = cv::Point(41.36, -25.96);

  ParticleFilter localise(resize_image, topLeftCorner, buttomRightCorner);

  //cv::imshow("animation", localise.animation);
  //float lidarScalar = localize.FindLidarLength(lidarMaxRange);
  //std::cout << "Lidar scalar is " << localize.mapScale << std::endl;
  cv::imshow("image", resize_image);

  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);

    //std::cout << "Lidar range is " << localize.mapScale << std::endl;

    mutex.lock();
    int key = cv::waitKey(1);
    mutex.unlock();
    
    localise.CreateParticles(center_distance);

    if (key == key_esc)
      break;

    if (((key == key_up)||(key == key_w)) && (speed <= 1.2f))
      speed += 0.05;
    else if (((key == key_down)||(key == key_s)) && (speed >= -1.2f))
      speed -= 0.05;
    else if (((key == key_right)||(key == key_d)) && (dir <= 0.4f))
      dir += 0.05;
    else if (((key == key_left)||(key == key_a)) && (dir >= -0.4f))
      dir -= 0.05;
    else if (key == key_space)
    {
      dir = 0;
      speed = 0;
    }
    else {
      // slow down
      //speed *= 0.001;
      //dir *= 0.001;
    }



    // Generate a pose
    ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));

    //printSomething();

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);
    movementPublisher->Publish(msg);
  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}