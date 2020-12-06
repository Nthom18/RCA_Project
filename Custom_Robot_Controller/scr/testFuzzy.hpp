#include <opencv2/opencv.hpp>
#include "fl/Headers.h"

#include <iostream>

#define COLOR_COUNT 200
#define BORDER_WIDTH 5

float intervalMappingF(float s, cv::Point from, cv::Point to)
{
    // Formula for linear mapping between intervals
    float t = to.x + ((s - from.x)*(to.y - to.x))/(from.y - from.x);
    return t;
}

void trackFuzzy(cv::Mat* map, float x, float y)
{
    static int color = 240;
    static int i = COLOR_COUNT;
    
    static const cv::Point gz_x(-41.1531, 41.3775); // These values are found by measuring it in the Gazebo env. (bigworld)
    static const cv::Point gz_y(27.2285, -27.2662);

    static const cv::Point map_x(BORDER_WIDTH, map->cols - BORDER_WIDTH);
    static const cv::Point map_y(BORDER_WIDTH, map->rows - BORDER_WIDTH);

    // std::cout << x << " " << y << std::endl;
    
    x = intervalMappingF(x, gz_x, map_x);
    y = intervalMappingF(y, gz_y, map_y);

    if(!i & color > 0)
    {
        color--;
        i = COLOR_COUNT;
    }

    cv::Point mark(x, y);
    cv::circle(*map, mark, 3, cv::Vec3b(color, color, 255), -1);
    
    i--;
}
