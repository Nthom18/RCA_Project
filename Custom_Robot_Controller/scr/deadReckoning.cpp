#include "deadReckoning.hpp"
#include <math.h> 

#define COLOR_COUNT 25
#define BORDER_WIDTH 25
#define SPEED_SCALING_FACTOR 19.970143

deadReckoning::deadReckoning()
{
    polar.modulus = 0;
    polar.angle = 0;

    dirGazInterval.min = 0, dirGazInterval.max = 390;
    radInterval.min = 0; radInterval.max = 2 * M_PI;

    std::string mapPath = "../../Gazebo/models/smallworld/meshes/floor_plan_w500.png";
    map = cv::imread(mapPath);
}

void deadReckoning::updateMovement(float speed, float dirG)
{
    float dir = intervalMapping(dirG, dirGazInterval, radInterval);

    polar.modulus = speed/SPEED_SCALING_FACTOR;
    polar.angle += dir;

    std::cout << "modulus: " << polar.modulus << " angle: " << polar.angle << std::endl;

    rectVector rect;
    rect.x = abs(polar.modulus) * cos(polar.angle);
    rect.y = abs(polar.modulus) * sin(polar.angle);

    // std::cout << "rectx: " << rect.x << " recty: " << rect.y << std::endl;

    x += rect.x;
    y += rect.y;

    trackOnMap(x, y);

    // printPos();
}

float deadReckoning::intervalMapping(float s, interval from, interval to)
{
    // Formula for linear mapping between intervals
    float t = to.min + ((s - from.min)*(to.max - to.min))/(from.max - from.min);
    
    return t;
}

void deadReckoning::trackOnMap(float xx, float yy)
{
    static int color = 240;
    static int i = COLOR_COUNT;
    
    static interval gz_x;
    gz_x.min = -12.3985, gz_x.max = 13.067;
    static interval gz_y;
    gz_y.min = -3.4046, gz_y.max = 15.0243;
    
    static interval map_x;
    map_x.min = BORDER_WIDTH, map_x.max = map.cols - BORDER_WIDTH;
    static interval map_y;
    map_y.min = BORDER_WIDTH, map_y.max = map.rows - BORDER_WIDTH;

    // std::cout << xx << " " << yy << std::endl;
    
    xx = intervalMapping(xx, gz_x, map_x);
    yy = intervalMapping(yy, gz_y, map_y);

    if(!i & color > 0)
    {
        color--;
        i = COLOR_COUNT;
    }

    cv::Point mark(xx, yy);
    cv::circle(map, mark, 3, cv::Vec3b(color, color, 255), -1);
    
    i--;
}

void deadReckoning::printPos()
{
    std::cout << "x: " << x << " y: " << y << std::endl;
}