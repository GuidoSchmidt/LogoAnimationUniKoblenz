#ifndef STAR_H
#define STAR_H

#include "ofVec2f.h"
#include "ofMain.h"

class Star
{
  private:
    ofVec2f position;
    float   size;
    int     timeOffset;

  public:
    Star(ofVec2f position, float size, int timeOffset);
    void draw(int alpha) const;
    void update();
    inline ofVec2f getPosition() const { return position; } ;
    inline float   getSize() const { return size;  };
};

#endif // STAR_H
