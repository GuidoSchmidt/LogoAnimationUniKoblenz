#ifndef CLOUD_H
#define CLOUD_H

#include "ofMain.h"

extern float windSpeed;

class Cloud
{
  private:
    ofVec2f position;
    std::vector<ofVec2f> partsCoordinates;
    std::vector<float> partsRadii;

  public:
    Cloud(const ofVec2f position);
    void draw();
};

#endif // CLOUD_H
