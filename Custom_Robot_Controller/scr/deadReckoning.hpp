#ifndef DEADRECKONING_H
#define DEADRECKONING_H

#include <iostream>

struct polarVector {
  float modulus;
  float angle;
};

struct rectVector {
  float x;
  float y;
};

class deadReckoning {
public:

    deadReckoning(){}
    ~deadReckoning(){}

    void updateMovement(float speed, float dir);

    float x = 0;
    float y = 0;

private:

    void printPos();

};
#endif

