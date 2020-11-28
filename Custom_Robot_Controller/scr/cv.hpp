#include <opencv2/opencv.hpp>

cv::Mat calibrate(cv::Mat img);

cv::Mat hough(cv::Mat img);

float distanceToMarble(int rdx);

void trackOnMap(cv::Mat map, float x, float y);

float intervalMapping(float s, cv::Point from, cv::Point to);