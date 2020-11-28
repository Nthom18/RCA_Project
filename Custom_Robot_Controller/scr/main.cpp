#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include "fl/Headers.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <thread>

#include "../include/functions.hpp"
#include "../include/GlobalVars.hpp"
#include "../include/GazeboFunctions.hpp"
#include "keyboardControl.cpp"
#include "fuzzyController.cpp"
#include "ParticleFilter.hpp"
#include "cv.hpp"


/*   main   */
int main(int _argc, char **_argv) {

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


  /***** SETUP FUZZY CONTROLLER *****/
  //Setup Engine  
  fl::Engine* engine = new fl::Engine;  
  //Setup input variable
  fl::InputVariable* obstacleLeft = new fl::InputVariable;
  fl::InputVariable* obstacleCenter = new fl::InputVariable;
  fl::InputVariable* obstacleRight = new fl::InputVariable;
  //Setup output variable - speed
  fl::OutputVariable* mSpeed= new fl::OutputVariable;
  //Setup output variable - direction
  fl::OutputVariable* mSteer = new fl::OutputVariable;
  // Setup ruleblock
  fl::RuleBlock* mamdani = new fl::RuleBlock;

  fuzzyController(engine, obstacleLeft, obstacleCenter, obstacleRight,
  mSpeed, mSteer, mamdani);


  cv::Point topLeftCorner = cv::Point(-34.89 , 24.6);
  cv::Point buttomRightCorner = cv::Point(41.36, -25.96);

  ParticleFilter localise(resize_image, topLeftCorner, buttomRightCorner);

  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);
    
    static float dir = 0.0;
    static float speed = 0.0;

    localise.CreateParticles(center_distance);

    /********** CONTROL WITH KEYBOARD **********/
    keyboardControl(&dir, &speed);


    /********** FUZZY CONTROL **********/
    fl::scalar locationL = obstacleLeft->getMinimum() + left_distance * (obstacleLeft->range() / lidarMaxRange);
    obstacleLeft->setValue(locationL);
    fl::scalar locationR = obstacleRight->getMinimum() + right_distance * (obstacleRight->range() / lidarMaxRange);
    obstacleRight->setValue(locationR);
    fl::scalar locationC = obstacleCenter->getMinimum() + center_distance * (obstacleCenter->range() / lidarMaxRange);
    obstacleCenter->setValue(locationC);

    //Test
    std::cout << "LocationL: " << locationL << std::endl;
    std::cout << "LocationC  " << locationC << std::endl;
    std::cout << "LocationR: " << locationR << std::endl;
    //end test

    //OBS! Do not open gazebo_client.sh

    // Process fuzzylite
    engine->process();

    // Output fuzzylite
    fl::scalar fuzzyOutputDir = mSteer->getValue();
    fl::scalar fuzzyOutputSpeed = mSpeed->getValue();

    if( ( (speed - ( (float) fuzzyOutputSpeed * 1.25) ) > 0.05) || ( ( ( (float) fuzzyOutputSpeed * 1.25)  - speed) > 0.05)) //If the difference is greater than 0.05
    {
      speed = (float) (fuzzyOutputSpeed * 1.25);
    }  

    if((abs(dir - ((float) fuzzyOutputDir * 0.8)-0.4) > 0.05) || (((abs((float) fuzzyOutputDir * 0.8)-0.4) - dir)  > 0.05))  //((abs(((float) fuzzyOutputDir * 0.8)-0.4) - dir)  > 0.05)
    {
      dir = ((float) fuzzyOutputDir * 0.8)-0.4;
    }
    
    //test
    std::cout << "Dir - stuff: " << (dir - (((float) fuzzyOutputDir * 0.8)-0.4) > 0.01) << " or stuff- dir: " << (((((float) fuzzyOutputDir * 0.8)-0.4) - dir)) << std::endl;
    std::cout << "fuzzyOutDir: " << fuzzyOutputDir << "  dir calculated: "<< ((float) fuzzyOutputDir * 0.8)-0.4 << " ->  dir: " << dir << std::endl;
    //end test

  if (key == key_esc)
    break;

    // Generate a pose
    ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));
    
    //printSomething();

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);
    movementPublisher->Publish(msg);


    /********** HOUGH TRANSFORM **********/

    cv::Mat cam_cal = hough(cam);

    if( !(cam_cal.size().width == 0 && cam_cal.size().height == 0) )
    {
      mutex.lock();
      cv::imshow("Hough Detection", cam_cal);
      mutex.unlock();
    }
  }
  // Make sure to shut everything down.
  gazebo::client::shutdown();
}
