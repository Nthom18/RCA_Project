#include "trackPos.hpp"
#include <iostream>

#define COLOR_COUNT 25
#define BORDER_WIDTH 25

void track(cv::Mat map, float x, float y)
{
    static int color = 240;
    static int i = COLOR_COUNT;
    
    static const cv::Point gz_x(-5, 5); // These values are found by measuring it in the Gazebo env.
    static const cv::Point gz_y(4, -4);

    static const cv::Point map_x(BORDER_WIDTH, map.cols - BORDER_WIDTH);
    static const cv::Point map_y(BORDER_WIDTH, map.rows - BORDER_WIDTH);

    x = intervalMapping(x, gz_x, map_x);
    y = intervalMapping(y, gz_y, map_y);

    // std::cout << x << " " << y << std::endl;

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
    // std::cout << t << std::endl;
    return t;
}