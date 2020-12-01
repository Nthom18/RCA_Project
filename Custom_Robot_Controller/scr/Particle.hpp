#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

class Particle
{
public:

    Particle()
    {

    };

    Particle(cv::Point _position, cv::Point _lookDirection)
    {
        Position = _position;
        LookDirection = _lookDirection;
    }

    cv::Point Position;         
    cv::Point LookDirection;    

    float weight;

private:

};