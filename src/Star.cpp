#include "Star.h"

#include <cmath>

Star::Star(const ofVec2f position, const float size, int timeOffset)
{
    this->position   = position;
    this->size       = size;
    this->timeOffset = timeOffset;
}

void Star::update()
{
    size += sin(ofGetSeconds() + timeOffset)/100.0;
}

void Star::draw(int alpha) const
{
    ofFill();
    ofSetColor(255, 255, 255, alpha * (1.0 - position.y/ofGetHeight()));
    ofCircle(position.x, position.y, size);
    ofNoFill();
}
