#include <opencv2/opencv.hpp>

std::vector<cv::Vec3f> hough(cv::Mat img);

void track(cv::Mat map, float x, float y);

float intervalMapping(float s, cv::Point from, cv::Point to);