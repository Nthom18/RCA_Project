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


/*   main   */
int main(int _argc, char **_argv) {
  
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

  /********** FUZZY CONTROL **********/
  //Setup Engine  
  fl::Engine* engine = new fl::Engine;
  engine->setName("obstacleAvoidance");
  engine->setDescription("");

  //Setup input variable
  fl::InputVariable* obstacle = new fl::InputVariable;
  obstacle->setName("obstacle");
  obstacle->setDescription("");
  obstacle->setEnabled(true);
  obstacle->setRange(0.000 , 1.000);
  obstacle->setLockValueInRange(false);
  obstacle->addTerm(new fl::Ramp("left", 0.000, 1.000));
  obstacle->addTerm(new fl::Ramp("right", 1.000, 0.000));
  engine->addInputVariable(obstacle);
  
  //Setup output variable
  fl::OutputVariable* mSteer = new fl::OutputVariable;
  mSteer->setName("mSteer");
  mSteer->setDescription("");
  mSteer->setEnabled(true);
  mSteer->setRange(0.000, 1.000);
  mSteer->setLockValueInRange(false);
  mSteer->setAggregation(new fl::Maximum);
  mSteer->setDefuzzifier(new fl::Centroid(100));
  mSteer->setDefaultValue(fl::nan);
  mSteer->setLockPreviousValue(false);
  mSteer->addTerm(new fl::Ramp("left", 0.000, 1.000));
  mSteer->addTerm(new fl::Ramp("right", 0.000, 1.000));
  engine->addOutputVariable(mSteer);

  // Setup ruleblock
  fl::RuleBlock* mamdani = new fl::RuleBlock;
  mamdani->setName("mamdani");
  mamdani->setDescription("");
  mamdani->setEnabled(true);
  mamdani->setConjunction(fl::null);
  mamdani->setDisjunction(fl::null);
  mamdani->setImplication(new fl::AlgebraicProduct);
  mamdani->setActivation(new fl::General);
  mamdani->addRule(fl::Rule::parse("if obstacle is left then mSteer is right", engine));
  mamdani->addRule(fl::Rule::parse("if obstacle is right then mSteer is left", engine));
  engine->addRuleBlock(mamdani);

  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);

    mutex.lock();
    int key = cv::waitKey(1);
    mutex.unlock();

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
      // slow down - This is a heavy calculation and slows down the whole program!
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


    /*  Get map from var map declared in GlobalVars.hpp */
    // // Show map:
    // if (!map.data) {
    //     return 1;
    // }
    // mutex.lock();
    // cv::imshow("Map", map);
    // mutex.unlock();

    /*  Get camera stream from var cam declared in GlobalVars.hpp 
        and updated in GazeboFunctions.hpp */

    // // Proof camera is accessable from var cam:    
    // mutex.lock();
    // cv::imshow("camera2", cam);
    // mutex.unlock();

  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}
