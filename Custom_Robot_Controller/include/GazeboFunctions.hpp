#pragma once

#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>

#include "GlobalVars.hpp"

#include <iostream>
#include <vector>
#include <math.h>

static boost::mutex mutex;

int distance;
float center_distance;
float left_distance;
float right_distance;
int lidarMaxRange;
int lidarMinRange; //added
std::vector<int> lidars;

//temp
int right_closest, left_closest, center_closest;
//end temtp

void statCallback(ConstWorldStatisticsPtr &_msg) {
  (void)_msg;
  // Dump the message contents to stdout.
  //  std::cout << _msg->DebugString();
  //  std::cout << std::flush;
}

void poseCallback(ConstPosesStampedPtr &_msg) {
  // Dump the message contents to stdout.
  //  std::cout << _msg->DebugString();

  for (int i = 0; i < _msg->pose_size(); i++) {
    if (_msg->pose(i).name() == "pioneer2dx") {

      /*std::cout << std::setprecision(2) << std::fixed << std::setw(6)
                << _msg->pose(i).position().x() << std::setw(6)
                << _msg->pose(i).position().y() << std::setw(6)
                << _msg->pose(i).position().z() << std::setw(6)
                << _msg->pose(i).orientation().w() << std::setw(6)
                << _msg->pose(i).orientation().x() << std::setw(6)
                << _msg->pose(i).orientation().y() << std::setw(6)
                << _msg->pose(i).orientation().z() << std::endl;*/
    }
  }
}

void cameraCallback(ConstImageStampedPtr &msg) {

  std::size_t width = msg->image().width();
  std::size_t height = msg->image().height();
  const char *data = msg->image().data().c_str();
  cv::Mat im(int(height), int(width), CV_8UC3, const_cast<char *>(data));
  
  im = im.clone();
  cv::cvtColor(im, im, cv::COLOR_RGB2BGR);

  mutex.lock();
  cv::imshow("camera", im);
  mutex.unlock();

  // Clone camera-feed into global variable
  cam = im.clone();
}

void lidarCallback(ConstLaserScanStampedPtr &msg) {

  //  std::cout << ">> " << msg->DebugString() << std::endl;
  float angle_min = float(msg->scan().angle_min());
  //  double angle_max = msg->scan().angle_max();
  float angle_increment = float(msg->scan().angle_step());

  float range_min = float(msg->scan().range_min());
  float range_max = float(msg->scan().range_max());
  lidarMaxRange = range_max;
  lidarMinRange = range_min; //added 

  int sec = msg->time().sec();
  int nsec = msg->time().nsec();

  int nranges = msg->scan().ranges_size();
  int nintensities = msg->scan().intensities_size();

  assert(nranges == nintensities);

  int width = 400;
  int height = 400;
  float px_per_m = 200 / range_max;

  cv::Mat im(height, width, CV_8UC3);
  im.setTo(0);
  for (int i = 0; i < nranges; i++) {
    float angle = angle_min + i * angle_increment;
    float range = std::min(float(msg->scan().ranges(i)), range_max);
    //    double intensity = msg->scan().intensities(i);
    cv::Point2f startpt(200.5f + range_min * px_per_m * std::cos(angle),
                        200.5f - range_min * px_per_m * std::sin(angle));
    cv::Point2f endpt(200.5f + range * px_per_m * std::cos(angle),
                      200.5f - range * px_per_m * std::sin(angle));
    cv::line(im, startpt * 16, endpt * 16, cv::Scalar(255, 255, 255, 255), 1,
             cv::LINE_AA, 4);
    //    std::cout << angle << " " << range << " " << intensity << std::endl;
  }
  cv::circle(im, cv::Point(200, 200), 2, cv::Scalar(0, 0, 255));
  cv::putText(im, std::to_string(sec) + ":" + std::to_string(nsec),
              cv::Point(10, 20), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(255, 0, 0));


// //original
//   left_distance = std::min(float(msg->scan().ranges(nranges * 1/4)), range_max);
//   center_distance = std::min(float(msg->scan().ranges(nranges * 1/2)), range_max);
//   right_distance = std::min(float(msg->scan().ranges(nranges * 3/4)), range_max);
//   std::cout << std::endl << "Left " << left_distance << " , Center " << 
//   center_distance << " , Right " << right_distance << std::endl;
// //End original

//Altered 
  //determine closest point to the left, center and right
  left_distance = 10;
  right_distance = 10;
  center_distance = 10;
  for (int i = 0; i < nranges*1/3; i++)
  {
    if (std::min(float(msg->scan().ranges(i)), range_max) < left_distance)
    {
      left_distance = std::min(float(msg->scan().ranges(i)), range_max);
      //std::cout << "Left_distance right after assignment: " << left_distance << std::endl;
    }


    if (std::min(float(msg->scan().ranges(i*2)), range_max) < center_distance)
    {
      center_distance = std::min(float(msg->scan().ranges(i*2)), range_max);
      
    }

    if (std::min(float(msg->scan().ranges(i*3)), range_max) < right_distance)
    {
      right_distance = std::min(float(msg->scan().ranges(i*3)), range_max);
      
    }
                  //test
      //std::cout << std::min(float(msg->scan().ranges(i*3)), range_max) << std::endl; 
      //end test
    //std::cout << left_distance << std::endl;
    // std::cout << "Nranges: "<< nranges << std::endl;
    // std::cout << "1/3 Nranges: " << (nranges * 1/3) << std::endl;
    // std::cout <<  "min:(scan, nrange(1/4)): " << std::min(float(msg->scan().ranges(nranges * 1/4)), range_max);
    // std::cout << std::endl;
    // std::cout <<  "min:(scan, nrange(i)): " << std::min(float(msg->scan().ranges(i)), range_max);
    // std::cout << std::endl;
  }
  //std::cout << left_distance << std::endl;

  // left_distance = std::min(float(msg->scan().ranges(nranges * 1/4)), range_max);
  //center_distance = std::min(float(msg->scan().ranges(nranges * 1/2)), range_max);
  //right_distance = std::min(float(msg->scan().ranges(nranges * 3/4)), range_max);
  // std::cout << std::endl << "Left " << left_distance << " , Center " << 
  // center_distance << " , Right " << right_distance << std::endl;
//end altered


    // //test
    // std::cout << "after values assigned to distances" <<std::endl;

    //end test

  //Save range output
  /*
  cv::Point2f frontStart(200.5f + range_min * px_per_m * std::cos(90),
                        200.5f - range_min * px_per_m * std::sin(90));
    cv::Point2f endpt(200.5f + range * px_per_m * std::cos(90),
                      200.5f - range * px_per_m * std::sin(90));
                      */
  //distance = std::sqrt(std::pow(frontStart.x,2) + frontStart.y) - ;


  mutex.lock();
  cv::imshow("lidar", im);
  mutex.unlock();
}