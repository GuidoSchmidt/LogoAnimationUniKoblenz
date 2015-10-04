#include "Cloud.h"

Cloud::Cloud(ofVec2f position)
{
    this->position = position;
    for(unsigned i=0; i < 15; i++)
    {
        ofVec2f partPosition{position.x + ofRandom(-60, 60), position.y + ofRandom(-10, 10)};
        partsCoordinates.push_back(partPosition);
        float positionDifference = abs(partPosition.x - position.x) / 90.0f;
        cout << positionDifference << endl;
        partsRadii.push_back(ofRandom(15, 35) * (1.0f - positionDifference) );
    }
}

void Cloud::draw()
{
    ofSetColor(255, 255, 255, 200);
    for(unsigned i=0; i < partsCoordinates.size(); ++i)
    {
        ofCircle(partsCoordinates[i], partsRadii[i]);
        partsCoordinates[i].x += (windSpeed * ofRandom(0.5, 1.0))/10.0f;
    }
}
