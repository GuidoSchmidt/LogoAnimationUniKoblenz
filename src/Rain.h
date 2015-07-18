#ifndef RAIN_H
#define RAIN_H

#include <vector>

#include <ofVec2f.h>
#include <ofMain.h>

class Rain
{
  private:
    std::vector<ofVec2f*> rainDrops;
    std::vector<ofVec2f*> rainDropDirections;
    std::vector<float>    lineWidths;
    unsigned              rainAmount;
    float                 rainSpeed;

  public:
    Rain(const unsigned rainAmount, const float rainSpeed);
    void draw(float alpha);
};

#endif // RAIN_H
