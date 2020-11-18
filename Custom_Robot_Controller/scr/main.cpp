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


<<<<<<< HEAD
  //Setup fuzzy control
  //using namespace fl;

    //Setup Engine  
  fl::Engine* engine = new fl::Engine;
  engine->setName("obstacleAvoidance");
  engine->setDescription("");

// //old version -one input variable
    //Setup input variable
  // fl::InputVariable* obstacle = new fl::InputVariable;
  // obstacle->setName("obstacle");
  // obstacle->setDescription("");
  // obstacle->setEnabled(true);
  // obstacle->setRange(0.000 , 1.000);
  // obstacle->setLockValueInRange(false);
  // obstacle->addTerm(new Ramp("left", 0.000, 1.000));
  // obstacle->addTerm(new Ramp("right", 1.000, 0.000));
  // engine->addInputVariable(obstacle);

//altered version - tree input variables, left, right, center
    //Setup input variable
  fl::InputVariable* obstacleLeft = new fl::InputVariable;
  obstacleLeft->setName("obstacleLeft");
  obstacleLeft->setDescription("");
  obstacleLeft->setEnabled(true);
  obstacleLeft->setRange(0.000 , 1.000);
  obstacleLeft->setLockValueInRange(false);
  obstacleLeft->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleLeft->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleLeft->addTerm(new fl::Triangle("visible", 0.000, 0.250, 0.500, 1.0));
  engine->addInputVariable(obstacleLeft);

  fl::InputVariable* obstacleCenter = new fl::InputVariable;
  obstacleCenter->setName("obstacleCenter");
  obstacleCenter->setDescription("");
  obstacleCenter->setEnabled(true);
  obstacleCenter -> setRange(0.000 , 1.000);
  obstacleCenter->setLockValueInRange(false);
  obstacleCenter->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleCenter->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleCenter->addTerm(new fl::Triangle("visible", 0.000, 0.250, 0.500, 1.0));
  engine->addInputVariable(obstacleCenter);

    fl::InputVariable* obstacleRight = new fl::InputVariable;
  obstacleRight->setName("obstacleRight");
  obstacleRight->setDescription("");
  obstacleRight->setEnabled(true);
  obstacleRight->setRange(1.000 , 0.000);
  obstacleRight->setLockValueInRange(false);
  obstacleRight->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleRight->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleRight->addTerm(new fl::Triangle("visible", 0.000, 0.250, 0.500, 1.0));
  engine->addInputVariable(obstacleRight);
  
 
        //Setup output variable - speed
  fl::OutputVariable* mSpeed= new fl::OutputVariable;
  mSpeed->setName("mSpeed");
  mSpeed->setDescription("");
  mSpeed->setEnabled(true);
  mSpeed->setRange(1.000, 0.000);
  mSpeed->setLockValueInRange(false);
  mSpeed->setAggregation(new fl::Maximum);
  mSpeed->setDefuzzifier(new fl::Centroid(100));
  mSpeed->setDefaultValue(fl::nan);
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
  mSteer->setRange(1.000, 0.000);
  mSteer->setLockValueInRange(false);
  mSteer->setAggregation(new fl::Maximum);
  mSteer->setDefuzzifier(new fl::Centroid(100));
  mSteer->setDefaultValue(fl::nan);
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
  mamdani->setDisjunction(new fl::Maximum);
  mamdani->setImplication(new fl::AlgebraicProduct);
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
=======
  /********** FUZZY CONTROL SETUP **********/
  fl::Engine* engine = new fl::Engine;
  fl::InputVariable* obstacle = new fl::InputVariable;
  fl::OutputVariable* mSteer = new fl::OutputVariable;
  fuzzyController(engine, obstacle, mSteer);
>>>>>>> 2705ce39be312c651bb5ab2d542fd7242fc690b2




  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);
    
    static float dir = 0.0;
    static float speed = 0.0;

    keyboardControl(&dir, &speed);

    /********** FUZZY CONTROL **********/
    // Fuzzyfication - Test fuzzylite using center distance
      // Convert the distance from robot to an obstacle to a value between 0-1.
<<<<<<< HEAD





    // for (int i = 0; i < lidarMaxRange; i++)
    // {
    //   scalar test = 0 + i * 1/lidarMaxRange;
    //       //tets
    // std::cout << "distLeft: " << distLeft << std::endl;
    // //end test
    // obstacleLeft -> setValue(test);//distLeft);
    // }
    // distLeft = (obstacleLeft->getMinimum() +  left_distance * (obstacleLeft->range() / lidarMaxRange));


    //scalar location = obstacle->getMinimum() + center_distance * (obstacle->range() / lidarMaxRange);
    fl::scalar locationL = obstacleLeft->getMinimum() + left_distance * (obstacleLeft->range() / lidarMaxRange);
    obstacleLeft->setValue(locationL);
    fl::scalar locationR = obstacleRight->getMinimum() + right_distance * (obstacleRight->range() / lidarMaxRange);
    obstacleRight->setValue(locationR);
    fl::scalar locationC = obstacleCenter->getMinimum() + center_distance * (obstacleCenter->range() / lidarMaxRange);
    obstacleCenter->setValue(locationC);
    //obstacleRight->setValue(locationL);
    //test 
    //std::cout << "Location: " << location << std::endl;
    //end test 


    //old version
=======
    fl::scalar location = obstacle->getMinimum() + center_distance * (obstacle->range() / lidarMaxRange);
>>>>>>> 2705ce39be312c651bb5ab2d542fd7242fc690b2
      // Give value to input variable
    //obstacle->setValue(location);

    //Altered version
    //obstacleLeft->setValue(location);

    //std::cout << "ObstacleLeft: " << obstacleLeft->getValue() << std::endl;
    //new version
        //Give value to input variable
      //obstacle->setValue(right) = location;




    //OBS! Do not open gazebo_client.sh

      // Process fuzzylite
    engine->process();
      //Output fuzzylite
<<<<<<< HEAD
    fl::scalar fuzzyOutputDir = mSteer->getValue();
    fl::scalar fuzzyOutputSpeed = mSpeed->getValue();
    // //outcommented to test
    // std::cout << "ObstacleL.input = " << Op::str(locationL) <<"ObstacleR.input = " << Op::str(locationR) << "=>" << "steer.output = "<< Op::str(fuzzyOutputDir) << std::endl;
    // std::cout << "OL.in = " << Op::str(locationL) <<" OC.input = " << Op::str(locationC) << " OR.input = " << Op::str(locationR) << "=>" << "speed.output = "<< Op::str(fuzzyOutputSpeed) << std::endl;
    
=======
    fl::scalar fuzzyOutput = mSteer->getValue();
    std::cout << fl::Op::str(fuzzyOutput) << std::endl;
>>>>>>> 2705ce39be312c651bb5ab2d542fd7242fc690b2
    
    
  //Original version
    // if (key == key_esc)
    //   break;

    // if (((key == key_up)||(key == key_w)) && (speed <= 1.2f))
    //   speed += 0.05;
    // else if (((key == key_down)||(key == key_s)) && (speed >= -1.2f))
    //   speed -= 0.05;
    // else if (((key == key_right)||(key == key_d)) && (dir <= 0.4f))
    //   dir += 0.05;
    // else if (((key == key_left)||(key == key_a)) && (dir >= -0.4f))
    //   dir -= 0.05;
    // else if (key == key_space)
    // {
    //   dir = 0;
    //   speed = 0;
    // }
    // else {
    //   // //slow down
    //   // speed *= 0.001;
    //   // dir *= 0.001;

<<<<<<< HEAD
    // }

  //Speed max 1.25,
  //dir max +-0.4

  //Altered version½
  // if (speed != 1.25)
  // {
    
  // }
  if(speed != (float) fuzzyOutputSpeed * 1.25)
    speed = (float) fuzzyOutputSpeed * 1.25; //1.25;}
  
  // if(dir != (float) fuzzyOutputDir * 0.4)
  //   dir = (float) fuzzyOutputDir * 0.4;

  if (key == key_esc)
    break;
    // if (((key == key_up)||(key == key_w)) && (speed <= 1.2f))
    //   speed += 0.05;
    //else if (((key == key_down)||(key == key_s)) && (speed >= -1.2f))
      //speed -= 0.05;
    // if (((key == key_right)||(key == key_d)) && (dir <= 0.4f))    //else if 
    //   dir += 0.05;
    // else if (((key == key_left)||(key == key_a)) && (dir >= -0.4f))
    //   dir -= 0.05;
    // else if (key == key_space)
    // {
    //   dir = 0;
    //   speed = 0;
    // }
    // else {
    //   // //slow down
    //   // speed *= 0.001;
    //   // dir *= 0.001;

    // }

  // //test
  //std::cout << "speed: " << speed << std::endl;
  // std::cout << "direction: " << dir << std::endl;
=======
>>>>>>> 2705ce39be312c651bb5ab2d542fd7242fc690b2

    // Generate a pose
    ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));
    
    //printSomething();

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);
    movementPublisher->Publish(msg);


  /********** CAMERA AND MAP TEST **********/
  
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
