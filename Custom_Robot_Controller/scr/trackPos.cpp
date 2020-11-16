#include "trackPos.hpp"
#include <iostream>

void track(cv::Mat map, int x, int y)
{
    cv::Point mark(x, y);
    cv::circle(map, mark, 3, cv::Vec3b(255, 0, 0), -1);
}