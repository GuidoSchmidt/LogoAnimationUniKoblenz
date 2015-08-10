#pragma once

#include <iostream>
#include <vector>

#include "ofMain.h"
#include "ofxTween.h"
#include "ofxYahooWeather.h"
#include "ofxGui.h"
#include "ofxUI.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
#include "ofEasyCam.h"

#include "Star.h"
#include "Rain.h"
#include "Snow.h"

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
    std::vector<Star*>   stars;
    Rain*                rain;
    Snow*                snow;
    ofxAssimpModelLoader model;
    ofMesh               mesh;

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

    float timeFloatFromString(std::string sunrise);

    // UI
    ofxPanel       gui;
    ofxFloatSlider daytimeSlide;
    ofxButton      daytimeAnimate;

    // UI2
    ofxUISuperCanvas*  guiCanvas;
    ofxUIDropDownList* dropDownList;
    ofxUIButton*       animateButton;
    void guiEvent(ofxUIEventArgs &event);

    void daytimeChanged(float& daytime);
    void daytimeAnimated();
};
