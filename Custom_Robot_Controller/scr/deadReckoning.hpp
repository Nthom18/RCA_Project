#ifndef DEADRECKONING_H
#define DEADRECKONING_H

#include <opencv2/opencv.hpp>

#include <iostream>

struct polarVector {
  float modulus;
  float angle;
};

struct rectVector {
  float x;
  float y;
};

struct interval {
  int min;
  int max;
};


class deadReckoning {
public:

  deadReckoning();
  ~deadReckoning(){}

  void updateMovement(float speed, float dir);

  float x = 0;
  float y = 0;

  cv::Mat map;

private:

  interval dirGazInterval;
  interval radInterval;
  
  polarVector polar;

  float intervalMapping(float s, interval from, interval to);

  void trackOnMap(float xx, float yy);

  void printPos();

};
#endif

