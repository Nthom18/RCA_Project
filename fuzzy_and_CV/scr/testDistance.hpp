#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>

#include "cv.hpp"

#define TEST_NR 100

void testDistance(cv::Mat cam)
{
    srand(time(0));

    static bool newSession = true;
    int signal = 0;
    FILE* coms = fopen("../TEST/server_signal.txt", "r");
    fscanf(coms, "%d", &signal);

    
    if( (signal == 1) & newSession )
    {     
        newSession = false;

        std::ofstream distanceData("distanceData.csv", std::ios::app);
    
        CV cv;
        float sumx = 0, sumy = 0, sumr = 0;
        float av_sumx = 0, av_sumy = 0, av_sumr = 0;

        for(int i = 0; i < TEST_NR; i++)
        {
            cv.hough(cam);

            for( auto j : cv.circles )
            {
                sumr += j[2];
            }
        }
        
        av_sumr = sumr / TEST_NR;

        float distance = cv.distanceToMarble(av_sumr * 2);

        // std::cout << distance << std::endl;
        distanceData << distance << '\n';
        distanceData.close();
    }

    if( signal == 0 )
        newSession = true;   

    fclose(coms);
}