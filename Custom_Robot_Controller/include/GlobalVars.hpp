#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <math.h>

// Declare Camera
cv::Mat cam;

// Load map
std::string mapPath = "../../Gazebo/models/bigworld/meshes/floor_plan.png";
// std::string mapPath = "../../Gazebo/models/smallworld/meshes/floor_plan_w500.png";
cv::Mat map0 = cv::imread(mapPath);

float posex;
float posey;
