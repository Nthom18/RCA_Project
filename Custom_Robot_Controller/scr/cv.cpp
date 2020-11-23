#include "cv.hpp"
#include <iostream>

#define COLOR_COUNT 25
#define BORDER_WIDTH 25


std::vector<cv::Vec3f> hough(cv::Mat img)
{
    std::vector<cv::Vec3f> circles;

    circles.clear();
    if( img.empty() )
        return circles;


    cv::Mat gray;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    
    medianBlur(gray, gray, 5);
    
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


void track(cv::Mat map, float x, float y)
{
    static int color = 240;
    static int i = COLOR_COUNT;
    
    static const cv::Point gz_x(-6, 6); // These values are found by measuring it in the Gazebo env.
    static const cv::Point gz_y(4.33, -4.33);

    static const cv::Point map_x(BORDER_WIDTH, map.cols - BORDER_WIDTH);
    static const cv::Point map_y(BORDER_WIDTH, map.rows - BORDER_WIDTH);

    // std::cout << x << " " << y << std::endl;
    
    x = intervalMapping(x, gz_x, map_x);
    y = intervalMapping(y, gz_y, map_y);

    if(!i & color > 0)
    {
        color--;
        i = COLOR_COUNT;
    }

    cv::Point mark(x, y);
    cv::circle(map, mark, 3, cv::Vec3b(color, color, 255), -1);
    
    i--;
}

float intervalMapping(float s, cv::Point from, cv::Point to)
{
    // Formula for linear mapping between intervals
    float t = to.x + ((s - from.x)*(to.y - to.x))/(from.y - from.x);
    return t;
}
