#include "Rain.h"

#include <cmath>

Rain::Rain(const unsigned rainAmount, const float rainSpeed)
{
    this->rainSpeed  = rainSpeed;
    this->rainAmount = rainAmount;
            for(unsigned i=0; i < this->rainAmount; i++)
    {
        ofVec2f* particle = new ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight(), -1.0));
        rainDrops.push_back(particle);
        ofVec2f* direction = new ofVec2f(particle->x - (particle->x + ofRandom(-1.5, 1.5)), particle->y - (particle->y + 10));
        direction->normalize();
        rainDropDirections.push_back(direction);
        lineWidths.push_back(ofRandom(0.5, 1.5f));
    }
}


void Rain::draw(float alpha)
{
  for(unsigned i = 0; i < rainDrops.size(); i++)
  {
      if(rainDrops[i]->y >= ofGetHeight())
      {
          rainDrops[i]->y -= ofGetHeight() + 20;
      }
      float alphaFactor = 0.0;
      if(rainDrops[i]->y <= 0.0)
      {
          alphaFactor = 1.0;
      }
      else
      {
          alphaFactor = rainDrops[i]->y/ofGetHeight();
      }
      ofSetColor(120, 120, 120, alpha * -log(alphaFactor) );
      ofSetLineWidth(lineWidths[i]);
      ofLine(rainDrops[i]->x, rainDrops[i]->y, rainDrops[i]->x + rainDropDirections[i]->x, rainDrops[i]->y + rainDropDirections[i]->y * 20);
      *rainDrops[i] -= *rainDropDirections[i] * rainSpeed;
  }
}

