#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include "fl/Headers.h"

#include <iostream>
#include <vector>
#include <math.h>

#include "../include/functions.hpp"
#include "../include/GlobalVars.hpp"
#include "../include/GazeboFunctions.hpp"
#include "keyboardControl.cpp"
#include "fuzzyController.cpp"
#include "cv.hpp"
// #include "trackPos.hpp"


/*   main   */
int main(int _argc, char **_argv) {

  /*  Get map from var mapO declared in GlobalVars.hpp */
  while(!mapO.data)
    std::cout << "Map not ready" << std::endl;
  cv::Mat map = mapO.clone();

  
  /********** GAZEBO SETUP SETUP **********/
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


  /********** FUZZY CONTROL SETUP **********/
  fl::Engine* engine = new fl::Engine;
  fl::InputVariable* obstacle = new fl::InputVariable;
  fl::OutputVariable* mSteer = new fl::OutputVariable;
  fuzzyController(engine, obstacle, mSteer);

  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);
    
    static float dir = 0.0;
    static float speed = 0.0;

    keyboardControl(&dir, &speed);

    /********** FUZZY CONTROL **********/
    // Fuzzyfication - Test fuzzylite using center distance
      // Convert the distance from robot to an obstacle to a value between 0-1.
    fl::scalar location = obstacle->getMinimum() + center_distance * (obstacle->range() / lidarMaxRange);
      // Give value to input variable
    obstacle->setValue(location);
      // Process fuzzylite
    engine->process();
      //Output fuzzylite
    fl::scalar fuzzyOutput = mSteer->getValue();
    std::cout << fl::Op::str(fuzzyOutput) << std::endl;
    


    // Generate a pose
    ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));

    //printSomething();

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);
    movementPublisher->Publish(msg);


    /********** CAMERA AND MAP TEST **********/
  
    // Show map:

    // mutex.lock();
    // cv::imshow("Map", map);
    // mutex.unlock();






    std::vector<cv::Vec3f> circles = hough(cam);

    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        circle( cam, center, 1, cv::Scalar(255,0,255), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle( cam, center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    }

    if( !(cam.size().width == 0 && cam.size().height == 0) )
    {
      mutex.lock();
      cv::imshow("Edges", cam);
      mutex.unlock();
    }
  }
  // Make sure to shut everything down.
  gazebo::client::shutdown();
}
