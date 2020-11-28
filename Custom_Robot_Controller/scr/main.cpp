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


  //Setup fuzzy control

    //Setup Engine  
  fl::Engine* engine = new fl::Engine;
  engine->setName("obstacleAvoidance");
  engine->setDescription("");
  
  //Setup input variable
  fl::InputVariable* obstacleLeft = new fl::InputVariable;
  obstacleLeft->setName("obstacleLeft");
  obstacleLeft->setDescription("");
  obstacleLeft->setEnabled(true);
  obstacleLeft->setRange(0.000 , 1.000); 
  obstacleLeft->setLockValueInRange(false);
  obstacleLeft->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleLeft->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleLeft->addTerm(new fl::Triangle("visible", 0.500, 0.750, 1.000, 1.0));
  engine->addInputVariable(obstacleLeft);

  fl::InputVariable* obstacleCenter = new fl::InputVariable;
  obstacleCenter->setName("obstacleCenter");
  obstacleCenter->setDescription("");
  obstacleCenter->setEnabled(true);
  obstacleCenter -> setRange(0.000 , 1.000);
  obstacleCenter->setLockValueInRange(false);
  obstacleCenter->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleCenter->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleCenter->addTerm(new fl::Triangle("visible", 0.500, 0.750, 1.000, 1.0));
  engine->addInputVariable(obstacleCenter);

  fl::InputVariable* obstacleRight = new fl::InputVariable;
  obstacleRight->setName("obstacleRight");
  obstacleRight->setDescription("");
  obstacleRight->setEnabled(true);
  obstacleRight->setRange(0.000 , 1.000);
  obstacleRight->setLockValueInRange(false);
  obstacleRight->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleRight->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleRight->addTerm(new fl::Triangle("visible", 0.500, 0.750, 1.000, 1.0));
  engine->addInputVariable(obstacleRight);
  
 
  //Setup output variable - speed
  fl::OutputVariable* mSpeed= new fl::OutputVariable;
  mSpeed->setName("mSpeed");
  mSpeed->setDescription("");
  mSpeed->setEnabled(true);
  mSpeed->setRange(0.000, 1.000);
  mSpeed->setLockValueInRange(false);
  mSpeed->setAggregation(new fl::Maximum);
  mSpeed->setDefuzzifier(new fl::Centroid(100));
  mSpeed->setDefaultValue(1.000);
  mSpeed->setLockPreviousValue(false);
  mSpeed->addTerm(new fl::Ramp("break", 0.5, 0, 1.0));
  mSpeed->addTerm(new fl::Triangle("slow", 0.250, 0.500, 0.750, 1.0));
  mSpeed->addTerm(new fl::Ramp("maximum", 0.500, 1.000, 1.0));

  engine->addOutputVariable(mSpeed);

  //Setup output variable - direction
  fl::OutputVariable* mSteer = new fl::OutputVariable;
  mSteer->setName("mSteer");
  mSteer->setDescription("");
  mSteer->setEnabled(true);
  mSteer->setRange(0.000, 1.000);
  mSteer->setLockValueInRange(false);
  mSteer->setAggregation(new fl::NormalizedSum);
  mSteer->setDefuzzifier(new fl::Centroid(100));
  mSteer->setDefaultValue(0.5);
  mSteer->setLockPreviousValue(false);
  //left - maximum left = 0
  mSteer->addTerm(new fl::Ramp("hardLeft", 0.250, 0, 1.0));
  mSteer->addTerm(new fl::Triangle("left", 0.125, 0.250, 0.375, 1.0));
  mSteer->addTerm(new fl::Triangle("softLeft", 0.250, 0.375, 0.500, 1.0));
  //right - maximum right = 1
  mSteer->addTerm(new fl::Ramp("hardRight", 0.750, 1.000, 1.0));
  mSteer->addTerm(new fl::Triangle("right", 0.625, 0.750, 0.875, 1.0));
  mSteer->addTerm(new fl::Triangle("softRight", 0.500, 0.650, 0.750, 1.0));

  engine->addOutputVariable(mSteer);

  // Setup ruleblock
  fl::RuleBlock* mamdani = new fl::RuleBlock;
  mamdani->setName("mamdani");
  mamdani->setDescription("");
  mamdani->setEnabled(true);
  mamdani->setConjunction(fl::null);
  mamdani->setDisjunction(new fl::NormalizedSum);///new fl::WeightedSum); //new fl::Maximum
  mamdani->setImplication(new fl::BoundedDifference);//AlgebraicProduct);
  mamdani->setActivation(new fl::General);
  //Direction
  mamdani->addRule(fl::Rule::parse("if obstacleLeft is veryClose then mSteer is hardRight", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleLeft is close then mSteer is right", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleLeft is visible then mSteer is softRight", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is veryClose then mSteer is hardLeft", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is close then mSteer is left", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is visible then mSteer is softLeft", engine));
  //Speed
  mamdani->addRule(fl::Rule::parse("if obstacleRight is veryClose or obstacleLeft is veryClose or obstacleCenter is veryClose then mSpeed is break", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is close or obstacleLeft is close or obstacleCenter is close then mSpeed is slow", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is visible or obstacleLeft is visible or obstacleCenter is visible then mSpeed is maximum", engine));
  engine->addRuleBlock(mamdani);



  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);
    
    static float dir = 0.0;
    static float speed = 0.0;

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
