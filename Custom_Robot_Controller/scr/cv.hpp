#ifndef CV_H
#define CV_H

#include <opencv2/opencv.hpp>

class CV {
public:
    CV(){}
    CV(cv::Mat* img);
    ~CV(){}

    cv::Mat hough(cv::Mat img);

    float distanceToMarble(int rpx);

    void printCircles();

    std::vector<cv::Vec3f> circles;

private:

    cv::Mat calibrate(cv::Mat img);

};







void trackOnMap(cv::Mat map, float x, float y);

float intervalMapping(float s, cv::Point from, cv::Point to);

#endif