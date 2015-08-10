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
    unsigned              dropLength;
    std::vector<int>      timeshifts;

  public:
    Rain(const unsigned _rainAmount,
         const float _rainSpeed,
         unsigned _dropLength);
    void draw(ofColor color, float alpha);
    inline void setDropLength(unsigned newDropLength)
    {
        dropLength = newDropLength;
    };
};

#endif // RAIN_H
