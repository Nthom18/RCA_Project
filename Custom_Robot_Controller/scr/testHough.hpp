#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>

#include "cv.hpp"

#define TEST_NR 100

void testHough(cv::Mat cam)
{
    srand(time(0));

    static bool newSession = true;
    int signal = 0;
    FILE* coms = fopen("../TEST/server_signal.txt", "r");
    fscanf(coms, "%d", &signal);

    
    if( (signal == 1) & newSession )
    {
        // Save image of cam
        std::string imgPath = "../TEST/cam_snapshots/snapshot" + std::to_string(rand()) + ".png";
        // cv::imwrite(imgPath, cam);
        
        newSession = false;

        std::ofstream houghData("houghData.csv", std::ios::app);
    
        CV cv;
        float sumx = 0, sumy = 0, sumr = 0;
        float av_sumx = 0, av_sumy = 0, av_sumr = 0;

        for(int i = 0; i < TEST_NR; i++)
        {
            cv.hough(cam);
            for( auto j : cv.circles )
            {
                sumx += j[0];
                sumy += j[1];
                sumr += j[2];
            }
        }
        
        av_sumx = sumx / TEST_NR;
        av_sumy = sumy / TEST_NR;
        av_sumr = sumr / TEST_NR;

        // std::cout << av_sumx << " " << av_sumy << " " << av_sumr << std::endl;
        houghData << av_sumx << ", " << av_sumy << ", " << av_sumr << '\n';
        houghData.close();
    }

    if( signal == 0 )
        newSession = true;   

    fclose(coms);
}