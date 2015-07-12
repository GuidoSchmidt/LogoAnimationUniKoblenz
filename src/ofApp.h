#pragma once

#include <iostream>
#include <vector>

#include "ofMain.h"
#include "ofxTween.h"
#include "ofxUI.h"
#include "ofxYahooWeather.h"

#include "Star.h"

class ofApp : public ofBaseApp
{
  private:
    // Tweening
    ofxTween        tweenLogoBlend;
    ofxTween        tweenDaytime;
    ofxEasingBack   easingBack;
    ofxEasingLinear easingLinear;
    ofxEasingCubic  easingCubic;
    // Weather API
    ofxYahooWeather yahooWeather;
    // Fonts
    ofTrueTypeFont font;

    // Elements
    std::vector<Star*> stars;

  public:
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void keyPressed      (ofKeyEventArgs&);
    void keyReleased     (ofKeyEventArgs&);

    void mouseMoved      (ofMouseEventArgs&);
    void mouseDragged    (ofMouseEventArgs&);
    void mousePressed    (ofMouseEventArgs&);
    void mouseReleased   (ofMouseEventArgs&);
    void mouseScrolled   (ofMouseEventArgs&);
    void mouseEntered    (ofMouseEventArgs&);
    void mouseExited     (ofMouseEventArgs&);

    void touchDown       (ofTouchEventArgs&);
    void touchMoved      (ofTouchEventArgs&);
    void touchUp         (ofTouchEventArgs&);
    void touchDoubleTap  (ofTouchEventArgs&);
    void touchCancelled  (ofTouchEventArgs&);

    void windowResized   (ofResizeEventArgs&);
    void dragged         (ofDragInfo&);
    void messageReceived (ofMessage&);

    //
    void drawStars();

    // UI
    ofxUISuperCanvas *gui0;
    void guiEvent(ofxUIEventArgs &e);
    float timeFloatFromString(std::string sunrise);
};
