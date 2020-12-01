#include "deadReckoning.hpp"
#include <math.h> 

deadReckoning::deadReckoning()
{
    polar.modulus = 0;
    polar.angle = 0;

    dirGazInterval.min = 0, dirGazInterval.max = 218;
    radInterval.min = 0; radInterval.max = 2 * M_PI;
}

void deadReckoning::updateMovement(float speed, float dirG)
{
    float dir = intervalMapping(dirG, dirGazInterval, radInterval);

    polar.modulus = speed;
    polar.angle += dir;

    // std::cout << "modulus: " << polar.modulus << " angle: " << polar.angle << std::endl;

    rectVector rect;
    rect.x = abs(polar.modulus) * cos(polar.angle);
    rect.y = abs(polar.modulus) * sin(polar.angle);

    std::cout << "rectx: " << rect.x << " recty: " << rect.y << std::endl;

    x += rect.x;
    y += rect.y; 

    // printPos();
}

float deadReckoning::intervalMapping(float s, interval from, interval to)
{
    // Formula for linear mapping between intervals
    float t = to.min + ((s - from.min)*(to.max - to.min))/(from.max - from.min);
    
    return t;
}

void deadReckoning::printPos()
{
    std::cout << "x: " << x << " y: " << y << std::endl;
}