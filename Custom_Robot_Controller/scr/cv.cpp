#include "cv.hpp"
#include <iostream>

#define COLOR_COUNT 25
#define BORDER_WIDTH 25

#define FOCAL_LENGTH 277.1914

cv::Mat calibrate(cv::Mat img)
{
    static const float f = FOCAL_LENGTH;
    static const float cx = 320/2, cy = 240/2;
    static const float k1 = -0.25, k2 = 0.12, k3 = 0.0;
    static const float p1 = -0.00028, p2 = -0.00005;

    static const cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << f, 0, cx, 0, f, cy, 0, 0, 1);
    static const cv::Mat distCoeffs = (cv::Mat1d(1, 5) << k1, k2, p1, p2, k3);

    static cv::Mat img_cal;
    cv::undistort(img, img_cal, cameraMatrix, distCoeffs);

    return img_cal;
}

cv::Mat hough(cv::Mat img)
{
    int radius;
    static std::vector<cv::Vec3f> circles;

    circles.clear();
    if( img.empty() )
        return img;


    cv::Mat img_hough = calibrate(img);

    static cv::Mat gray;
    cvtColor(img_hough, gray, cv::COLOR_BGR2GRAY);
    
    medianBlur(gray, gray, 5);
    
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                 gray.rows/4,  // change this value to detect circles with different distances to each other
                 350, 20, gray.rows/32, gray.rows // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
    );
    
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        cv::circle( img_hough, center, 1, cv::Scalar(255,0,255), 3, cv::LINE_AA);
        // circle outline
        radius = c[2];
        cv::circle( img_hough, center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    }

    distanceToMarble(radius * 2);

    return img_hough;
}

float distanceToMarble(int dpx)
{
    static const int d = 1;
    float a = FOCAL_LENGTH/(dpx * d);

    // std::cout << "HOUGH: " << a << std::endl;
    
    return a;
}

void trackOnMap(cv::Mat map, float x, float y)
{
    static int color = 240;
    static int i = COLOR_COUNT;
    
    static const cv::Point gz_x(-6, 6); // These values are found by measuring it in the Gazebo env.
    static const cv::Point gz_y(4.33, -4.33);

    static const cv::Point map_x(BORDER_WIDTH, map.cols - BORDER_WIDTH);
    static const cv::Point map_y(BORDER_WIDTH, map.rows - BORDER_WIDTH);

    // std::cout << x << " " << y << std::endl;
    
    x = intervalMapping(x, gz_x, map_x);
    y = intervalMapping(y, gz_y, map_y);

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
    return t;
}
