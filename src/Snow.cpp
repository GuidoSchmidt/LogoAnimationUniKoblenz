#include "Snow.h"

#include <cmath>

extern float windSpeed;
extern float alphaFactor;

Snow::Snow(const unsigned _amount,
           const float _speed)
: snowAmount{_amount},
  snowSpeed{_speed}
{
    for(unsigned i=0; i < this->snowAmount; i++)
    {
        ofVec2f* particle = new ofVec2f{ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight(), -1.0)};
        snowFlakes.push_back(particle);
        ofVec2f* direction = new ofVec2f{particle->x - (particle->x + ofRandom(-5, 5) + windSpeed * 3), particle->y - (particle->y + 10)};
        direction->normalize();
        snowFlakeDirections.push_back(direction);
        flakeSizes.push_back(ofRandom(1.5f, 5.5f));
        timeshifts.push_back(ofRandom(-250, 250));
    }
}

void Snow::draw(ofColor color, float alpha) const
{
  for(unsigned i = 0; i < snowFlakes.size(); i++)
  {
      if(snowFlakes[i]->y > ofGetHeight())
      {
          snowFlakes[i]->y -= ofGetHeight();
      }
      if(snowFlakes[i]->x > ofGetWidth())
      {
          snowFlakes[i]->x = 0;
      }
      if(snowFlakes[i]->x < 0)
      {
          snowFlakes[i]->x = ofGetWidth();
      }

      *snowFlakes[i] -= *snowFlakeDirections[i] * snowSpeed;
      *snowFlakes[i] += ofVec2f{windSpeed, 1.0} * sin((ofGetFrameNum() + ofRandom(0, 1000)) /1000);

      // Draw
      ofSetColor(255, 255, 255);
      ofFill();
      ofCircle(snowFlakes[i]->x, snowFlakes[i]->y, flakeSizes[i]);
  }
}
