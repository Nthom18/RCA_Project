#include "deadReckoning.hpp"
#include <math.h> 


void deadReckoning::updateMovement(float speed, float dir)
{
    polarVector polar;
    polar.modulus = speed;
    polar.angle = dir;

    rectVector rect;
    rect.x = abs(polar.modulus) * cos(polar.angle);
    rect.y = abs(polar.modulus) * sin(polar.angle);

    x += rect.x;
    y += rect.y; 

    printPos();
}

void deadReckoning::printPos()
{
    std::cout << "x: " << x << " y: " << y << std::endl;
}