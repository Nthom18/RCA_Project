#include <opencv2/opencv.hpp>

cv::Mat calibrate(cv::Mat img);

cv::Mat hough(cv::Mat img);

void track(cv::Mat map, float x, float y);

float intervalMapping(float s, cv::Point from, cv::Point to);