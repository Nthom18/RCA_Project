#include "trackPos.hpp"
#include <iostream>

void track(cv::Mat map, int x, int y)
{
    static const cv::Point gz_x(-5, 5); // These values are found by measuring it in the Gazebo env.
    static const cv::Point gz_y(4, -4);

    static const cv::Point map_x(0, map.cols);
    static const cv::Point map_y(0, map.rows);

    // std::cout << "GZ:  " << x << " " << y << std::endl;

    x = intervalMapping(x, gz_x, map_x);
    y = intervalMapping(y, gz_y, map_y);

    // std::cout << "MAP: " << x << " " << y << std::endl;

    cv::Point mark(x, y);
    cv::circle(map, mark, 3, cv::Vec3b(255, 0, 0), -1);
}

int intervalMapping(int s, cv::Point from, cv::Point to)
{
    // Formula for mapping number from one range to another
    int t = to.x + ((s - from.x)*(to.y - to.x))/(from.y - from.x);
    return t;
}