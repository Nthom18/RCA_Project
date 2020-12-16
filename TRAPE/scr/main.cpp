#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>
#include <cmath>
#include <algorithm>

#include "../include/Roadmap.hpp"


int main()
{
    cv::Mat map = cv::imread("../course.png");
    std::vector<cv::Point> points;

    // Add map corners
    //cv::circle(map, cv::Point(0,0), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
    points.push_back(cv::Point(0,0));

    //cv::circle(map, cv::Point(0, map.rows), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
    points.push_back(cv::Point(0,map.rows));

    //cv::circle(map, cv::Point(map.cols, 0), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
    points.push_back(cv::Point(map.cols, 0));

    //cv::circle(map, cv::Point(map.cols,map.rows), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
    points.push_back(cv::Point(map.cols, map.rows));

    // Find corners
    std::vector<cv::Point> corners;
    for(int i = 0; i < map.cols; i++)
    {
        for(int j = 0; j < map.rows; j++)
        {
            cv::Vec3b currentPixelColor = map.at<cv::Vec3b>(cv::Point(i,j));
            if(currentPixelColor.val[0] == 0 && currentPixelColor.val[1] == 0 && currentPixelColor.val[2] == 0)
            {
                if(checkArea(cv::Point(i,j), map))
                {
                    corners.push_back(cv::Point(i,j));
                    points.push_back(cv::Point(i,j));
                    //Scalar(100,0,0), 2, cv::LINE_8);
                }
            }
        }
    }

    cv::Vec3b blue = cv::Vec3b(255, 0, 0);
    //std::vector<cv::Point> toBeAdded;
    for(cv::Point currentCorner : corners)
    {
        // Check upwards
        for(int i = currentCorner.y - 1 ; i > 0 ; i--)
        {
            cv::Vec3b currentPixelColor = map.at<cv::Vec3b>(cv::Point(currentCorner.x, i));
            if(currentPixelColor.val[0] == 0 && currentPixelColor.val[1] == 0 && currentPixelColor.val[2] == 0)
            {
                //cv::circle(map, cv::Point(currentCorner.x, i), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
                points.push_back(cv::Point(currentCorner.x, i));
                break;
            }
            else if(i == 1)
            {
                //cv::circle(map, cv::Point(currentCorner.x, i - 1), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
                points.push_back(cv::Point(currentCorner.x, i - 1));
                break;
            }
            else
            {
                map.at<cv::Vec3b>(cv::Point(currentCorner.x, i)) = blue;
            }
        }

        // Check downwards
        for(int i = currentCorner.y + 1 ; i < map.rows ; i++)
        {
            cv::Vec3b currentPixelColor = map.at<cv::Vec3b>(cv::Point(currentCorner.x, i));
            if(currentPixelColor.val[0] == 0 && currentPixelColor.val[1] == 0 && currentPixelColor.val[2] == 0)
            {
                //cv::circle(map, cv::Point(currentCorner.x, i), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
                points.push_back(cv::Point(currentCorner.x, i));
                break;
            }
            else if(i == map.rows - 1)
            {
                //cv::circle(map, cv::Point(currentCorner.x, i + 1), 5, cv::Scalar(100,0,0), 2, cv::LINE_8);
                points.push_back(cv::Point(currentCorner.x, i + 1));
                break;
            }
            else
            {
                map.at<cv::Vec3b>(cv::Point(currentCorner.x, i)) = blue;
            }
        }
    }

    for(cv::Point dummy : points)
    {
        std::cout << dummy.x << " " << dummy.y << std::endl; 
        cv::circle(map, dummy, 5, cv::Scalar(100,0,0), 3, cv::LINE_8);
    }

    // Check for each point if there is there can exist a rectangle
    
    std::vector<cv::Point> debug = points;
    for(int i = 0; i < debug.size(); i++)
    {
        bool pointBelow = false;
        bool pointRight = false;
        bool pointBelowRight = false;

        cv::Point below;
        cv::Point right;
        cv::Point rightBelow;
        
         // Step 1, check if there is a points underneath it. 
         
        std::vector<cv::Point> pointsUnder;
        std::vector<int> pointsUnderDist;
        for(int j = 0; j < debug.size(); j++)
        {
            if(debug[i].x == debug[j].x && debug[i].y < debug[j].y)
            {
                pointsUnder.push_back(debug[j]);
                pointsUnderDist.push_back(std::sqrt(std::pow(debug[i].x - debug[j].x, 2) + std::pow(debug[i].y - debug[j].y, 2)));
                pointBelow = true;
            }
        }
        
        // Step 2, check if there exist two points right of original and below

        if(pointBelow)
        {
            ModBubbleSort(pointsUnder, pointsUnderDist);
            below = pointsUnder[0];

            // These create problems
            std::vector<cv::Point> pointsRight;
            std::vector<int> pointsRightDist;
            
            /*
            for(int j = 0; j < debug.size(); j++)
            {
                if(debug[i].y == debug[j].y && debug[i].y < debug[j].y)
                {
                    pointsRight.push_back(debug[j]);
                    pointsRightDist.push_back(std::sqrt(std::pow(debug[i].x - debug[j].x, 2) + std::pow(debug[i].y - debug[j].y, 2)));
                    pointRight = true;
                }
            }
            
            if(pointBelow == true && pointRight == true)
            {
                ModBubbleSort(pointsRight, pointsRightDist);
                right = pointsRight[0];                
            }
            
            */



        }
    }

    cv::imshow("image", map);
    cv::waitKey();

    return 0;
}