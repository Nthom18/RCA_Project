#include "cv.hpp"
#include <cassert>
#include <iostream>

cv::Mat edges;
std::vector<cv::Vec3f> circles;

std::vector<cv::Vec3f> hough(cv::Mat img)
{
    // cv::Canny(img, edges, 180, 200);

    circles.clear();
    if( img.empty() )
        return circles;


    cv::Mat gray;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    // medianBlur(gray, gray, 5);
    
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                 gray.rows/4,  // change this value to detect circles with different distances to each other
                 350, 20, gray.rows/32, gray.rows // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
    );
    // for( size_t i = 0; i < circles.size(); i++ )
    // {
    //     cv::Vec3i c = circles[i];
    //     cv::Point center = cv::Point(c[0], c[1]);
    //     // circle center
    //     circle( img, center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
    //     // circle outline
    //     int radius = c[2];
    //     circle( img, center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);
    // }

    return circles;
}
