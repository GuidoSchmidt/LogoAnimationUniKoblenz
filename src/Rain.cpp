#include "Rain.h"

#include <cmath>

float windSpeed{0.0f};
float alphaFactor = 0.0;

Rain::Rain(const unsigned _rainAmount,
           const float _rainSpeed,
           unsigned _dropLength)
: rainAmount{_rainAmount},
  rainSpeed{_rainSpeed},
  dropLength{_dropLength}
{
    for(unsigned i=0; i < this->rainAmount; i++)
    {
        ofVec2f* particle = new ofVec2f{ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight(), -1.0)};
        rainDrops.push_back(particle);
        ofVec2f* direction = new ofVec2f{particle->x - (particle->x + ofRandom(-5, 5) + windSpeed * 3), particle->y - (particle->y + dropLength)};
        direction->normalize();
        rainDropDirections.push_back(direction);
        lineWidths.push_back(ofRandom(0.5f, 2.5f));
        timeshifts.push_back(ofRandom(-250, 250));
    }
}


void Rain::draw(ofColor color, float alpha)
{
  for(unsigned i = 0; i < rainDrops.size(); i++)
  {
      if(rainDrops[i]->y > ofGetHeight())
      {
          rainDrops[i]->y -= ofGetHeight() + dropLength;
      }
      if(rainDrops[i]->x > ofGetWidth())
      {
          rainDrops[i]->x = 0;
      }
      if(rainDrops[i]->x < 0)
      {
          rainDrops[i]->x = ofGetWidth();
      }

      *rainDrops[i] -= *rainDropDirections[i] * rainSpeed;
      *rainDrops[i] += ofVec2f{windSpeed + windSpeed * sin((rainDropDirections[i]->x * 200)-100 + ofGetFrameNum()/10.0f) * 3.0f, 1.0};

      // Draw
      alphaFactor = rainDrops[i]->y/ofGetHeight();
      ofSetColor(color, alpha * (1.0 - alphaFactor));
      ofSetLineWidth(lineWidths[i]);
      ofPushMatrix();
      ofTranslate(rainDrops[i]->x, rainDrops[i]->y);
      ofRotateZ(sin(ofGetFrameNum()/10.0f) * windSpeed);
      ofTranslate(-rainDrops[i]->x, -rainDrops[i]->y);
      ofLine(rainDrops[i]->x,
             rainDrops[i]->y,
             rainDrops[i]->x + rainDropDirections[i]->x * dropLength,
             rainDrops[i]->y + rainDropDirections[i]->y * dropLength);
      ofPopMatrix();
  }
}

