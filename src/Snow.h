#ifndef SNOW_H
#define SNOW_H

#include <vector>

#include <ofVec2f.h>
#include <ofMain.h>

class Snow
{
  private:
    std::vector<ofVec2f*> snowFlakes;
    std::vector<ofVec2f*> snowFlakeDirections;
    std::vector<float>    flakeSizes;
    unsigned              snowAmount;
    float                 snowSpeed;
    std::vector<int>      timeshifts;

  public:
    Snow(const unsigned _amount,
         const float _speed);
    void draw(ofColor color, float alpha) const;
};

#endif // SNOW_H
