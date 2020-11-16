#include "trackPos.hpp"
#include <iostream>

#define COLOR_COUNT 25

void track(cv::Mat map, float x, float y)
{
    static int color = 240;
    static int i = COLOR_COUNT;
    

    static const cv::Point gz_x(-5.5, 5.5); // These values are found by measuring it in the Gazebo env.
    static const cv::Point gz_y(4.5, -4.5);

    static const cv::Point map_x(0, map.cols);
    static const cv::Point map_y(0, map.rows);

    x = intervalMapping(x, gz_x, map_x);
    y = intervalMapping(y, gz_y, map_y);

    if(!i & color < 0)
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
    // Formula for mapping number from one range to another
    float t = to.x + ((s - from.x)*(to.y - to.x))/(from.y - from.x);
    // std::cout << t << std::endl;
    return t;
}