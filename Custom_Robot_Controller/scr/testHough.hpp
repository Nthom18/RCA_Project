#include <opencv2/opencv.hpp>

#include <iostream>

#include "cv.hpp"

void testHough(cv::Mat cam)
{
    int signal;
    FILE* coms = fopen("../TEST/server_signal.txt", "r");
    fscanf(coms, "%d", &signal);

    if( signal == 1 )
    {
        
    }    

    fclose(coms);
}