#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <math.h>

// Declare Camera
cv::Mat cam;

// Load map
std::string mapPath = "../../Gazebo/models/bigworld/meshes/floor_plan.png";
cv::Mat mapO = cv::imread(mapPath);
