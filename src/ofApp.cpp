#include "ofApp.h"

#include <map>
#include <string>
#include <chrono>

ofImage img_logoUniKoblenz;
ofImage img_errorBackground;
bool    connectionIssue = false;
bool    animateDaytime  = false;
bool    night           = false;
float   dayTimeFromGui  = 0.0;
float   sunsetTime, sunriseTime;

void buildConditionCodeMap(std::map<int, std::string>& conditionCodeMap)
{
    conditionCodeMap[ 0]   = "Tornado";
    conditionCodeMap[ 1]   = "Tropical Storm";
    conditionCodeMap[ 2]   = "Hurricane";
    conditionCodeMap[ 4]   = "Thunderstorms";
    conditionCodeMap[ 5]   = "Mixed Rain And Snow";
    conditionCodeMap[ 6]   = "Mixed Rain And Sleet";
    conditionCodeMap[ 7]   = "Mixed Snow And Sleet";
    conditionCodeMap[ 8]   = "Freezing Drizzle";
    conditionCodeMap[ 9]   = "Drizzle";
    conditionCodeMap[10]   = "Freezing Rain";
    conditionCodeMap[11]   = "Showers";
    conditionCodeMap[12]   = "Showers";
    conditionCodeMap[13]   = "Snow Flurries";
    conditionCodeMap[14]   = "Light Snow Showers";
    conditionCodeMap[15]   = "Blowing Snow";
    conditionCodeMap[16]   = "Snow";
    conditionCodeMap[17]   = "Hail";
    conditionCodeMap[18]   = "Sleet";
    conditionCodeMap[19]   =  "Dust";
    conditionCodeMap[20]   =  "Foggy";
    conditionCodeMap[21]   =  "Haze";
    conditionCodeMap[22]   =  "Smoky";
    conditionCodeMap[23]   =  "Blustery";
    conditionCodeMap[24]   =  "Windy";
    conditionCodeMap[25]   =  "Cold";
    conditionCodeMap[26]   =  "Cloudy0 Tornado";
    conditionCodeMap[27]   =  "Mostly Cloudy (Night)";
    conditionCodeMap[28]   =  "Mostly Cloudy (Day)";
    conditionCodeMap[29]   =  "Partly Cloudy (Night)";
    conditionCodeMap[30]   =  "Partly Cloudy (Day)";
    conditionCodeMap[31]   =  "Clear (Night)";
    conditionCodeMap[32]   =  "Sunny";
    conditionCodeMap[33]   =  "Fair (Night)";
    conditionCodeMap[34]   =  "Fair (Day)";
    conditionCodeMap[35]   =  "Mixed Rain And Hail";
    conditionCodeMap[36]   =  "Hot";
    conditionCodeMap[37]   =  "Isolated Thunderstorms";
    conditionCodeMap[38]   =  "Scattered Thunderstorms";
    conditionCodeMap[39]   =  "Scattered Thunderstorms";
    conditionCodeMap[40]   =  "Scattered Showers";
    conditionCodeMap[41]   =  "Heavy Snow";
    conditionCodeMap[42]   =  "Scattered Snow Showers";
    conditionCodeMap[43]   =  "Heavy Snow";
    conditionCodeMap[44]   =  "Partly Cloudy";
    conditionCodeMap[45]   =  "Thundershowers";
    conditionCodeMap[46]   =  "Snow Showers";
    conditionCodeMap[47]   =  "Isolated Thundershowers";
    conditionCodeMap[3200] =  "Not Available";
}

ofColor getColorBasedOnTime(float sunriseTime, float sunsetTime)
{
    ofColor sunrise(233, 172, 156);
    ofColor daytime(91, 220, 255);
    ofColor sunset(233, 130, 103);
    ofColor nighttime(8, 6, 20);

    float delta = 3.0;

    if(dayTimeFromGui < sunriseTime)
    {
        night = true;
        return nighttime;
    }
    else if(dayTimeFromGui > sunriseTime && dayTimeFromGui < sunriseTime + delta)
    {
        night = false;
        float driver = ofClamp((dayTimeFromGui - sunriseTime), 0.0, 1.0);
        return nighttime.getLerped(sunrise, driver);
    }
    else if(dayTimeFromGui > sunriseTime + delta && dayTimeFromGui < sunsetTime - delta)
    {
        float driver = ofClamp((dayTimeFromGui - (sunriseTime + delta)), 0.0, 1.0);
        return sunrise.getLerped(daytime, driver);
    }
    else if(dayTimeFromGui > sunsetTime - delta && dayTimeFromGui < sunsetTime)
    {
        float driver = ofClamp((dayTimeFromGui - (sunsetTime - delta)), 0.0, 1.0);
        return daytime.getLerped(sunset, driver);
    }
    else if(dayTimeFromGui > sunsetTime)
    {
        float driver = ofClamp((dayTimeFromGui - sunsetTime), 0.0, 1.0);
        return sunset.getLerped(nighttime, driver);
    }
    else
    {
        night = true;
        return nighttime;
    }
}

float ofApp::timeFloatFromString(std::string sunrise)
{
    size_t delimiter = sunrise.find(':');
    int hour = std::atoi(sunrise.substr(0, delimiter).c_str());
    size_t twelveToken = sunrise.find('m');
    std::string twelveString = sunrise.substr(twelveToken-1);
    if(twelveString == "pm")
    {
        hour += 12;
    }
    int minutes = std::atoi(sunrise.substr(delimiter+1, twelveToken-3).c_str());
    float time = (float)hour + ((float)minutes/60);

    return time;
}

void ofApp::setup()
{
    // ofx Setup
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofSetCircleResolution(100);
    // Images
    img_logoUniKoblenz.load("logo_uni-koblenz.jpg");
    img_errorBackground.load("error.jpg");
    // Tween
    const unsigned delay    = 2000;
    const unsigned duration = 5000;
    tweenLogoBlend.setParameters(1, easingCubic, ofxTween::easeOut, 0, 255, duration, delay);
    tweenDaytime.setParameters(0, easingLinear, ofxTween::easeInOut, 0, 24.0, 24000, 0.0);
    // Font
    font.loadFont("InputMono-Regular.ttf", 16);
    // Weather API
    unsigned locationCode_Koblenz = 667675;
    yahooWeather.setup(locationCode_Koblenz, 'c');
    yahooWeather.refresh();
    // Temperature
    std::cout << "Temperature: " << yahooWeather.getTemperature() << std::endl;
    // Sunrise
    std::string sunrise = yahooWeather.getSunrise();
    sunriseTime = timeFloatFromString(sunrise);
    std::cout << "Sunrise @ " << sunriseTime << std::endl;
    // Sunset
    std::string sunset = yahooWeather.getSunset();
    sunsetTime = timeFloatFromString(sunset);
    std::cout << "Sunset @ " << sunsetTime << std::endl;
    // Daytime
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "Daytime: " << std::ctime(&now_t);
    // Weather condition
    int condititonCode = yahooWeather.getWeatherConditionCode();
    std::map<int, std::string> conditionCodeMap;
    buildConditionCodeMap(conditionCodeMap);
    std::cout << conditionCodeMap[condititonCode] << std::endl;
    if(condititonCode == 3200)
    {
        connectionIssue = true;
    }

    // UI
    gui0 = new ofxUISuperCanvas("Slider");
    gui0->addMinimalSlider("Daytime", 0.0, 24.0, &dayTimeFromGui);
    gui0->addButton("Animate", animateDaytime);
    ofAddListener(gui0->newGUIEvent, this, &ofApp::guiEvent);

    // Objects
    for(unsigned i = 0; i < 200; i++)
    {
        stars.push_back(new Star(ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())), ofRandom(0.5, 1.5), ofRandom(-100, 100)));
    }
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.getName() == "Animate")
    {
        std::cout << "Animate" << std::endl;
        animateDaytime = true;
    }
}

void ofApp::update()
{
    if(animateDaytime)
    {
        dayTimeFromGui = tweenDaytime.update();
    }

    if(tweenDaytime.isCompleted())
    {
        animateDaytime = false;
        tweenDaytime.setParameters(0, easingLinear, ofxTween::easeInOut, 0, 24.0, 24000, 0.0);
    }
}

void ofApp::drawStars()
{
    for(unsigned i = 0; i < stars.size(); i++)
    {
        stars.at(i)->update();
        stars.at(i)->draw();
    }
}

void ofApp::draw()
{
    // Background
    ofColor dynamicBackgroundColor          = getColorBasedOnTime(sunriseTime, sunsetTime);
    ofColor dynamicBackgroundColorSaturated = dynamicBackgroundColor;
    dynamicBackgroundColorSaturated.setSaturation(0);
    ofBackgroundGradient(dynamicBackgroundColor, dynamicBackgroundColorSaturated, OF_GRADIENT_LINEAR);

    if(night)
    {
        drawStars();
    }


    // Connection issues
    if(connectionIssue)
    {
        ofNoFill();
        img_errorBackground.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(234, 67, 132);
        font.drawString("Connection Error!", ofGetWidth()/2 - 100, ofGetHeight()/2 - 200);
        ofSetColor(250, 250, 250);
        font.drawString("Please check your internet connection\nand restart application.", ofGetWidth()/2 - 220, ofGetHeight()/2 - 200 + 24);
    }
    // Rendering
    else
    {
        ofSetCircleResolution(100);
        ofFill();
        ofSetColor(255, 252, 218);
        ofCircle(ofGetWidth()/2 + 200, ofGetHeight()/2, 200);

        ofSetColor(128, 128, 128);
        ofCircle(ofGetWidth()/2- 200, ofGetHeight()/2, 120);

        /*
        unsigned transparency = 0; //tweenLinear.update();
        ofSetColor(transparency, transparency, transparency);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        img_logoUniKoblenz.draw(0, 0, ofGetWidth(), ofGetHeight());
        */
    }
}

void ofApp::exit()
{
    delete gui0;
}

void ofApp::keyPressed(ofKeyEventArgs& key)
{
}

void ofApp::keyReleased(ofKeyEventArgs& key)
{
}

void ofApp::mouseMoved(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseDragged(ofMouseEventArgs& mouse)
{
}

void ofApp::mousePressed(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseReleased(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseScrolled(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseEntered(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseExited(ofMouseEventArgs& mouse)
{
}

void ofApp::touchDown(ofTouchEventArgs& touch)
{
}

void ofApp::touchMoved(ofTouchEventArgs& touch)
{
}

void ofApp::touchUp(ofTouchEventArgs& touch)
{
}

void ofApp::touchDoubleTap(ofTouchEventArgs& touch)
{
}

void ofApp::touchCancelled(ofTouchEventArgs& touch)
{
}

void ofApp::windowResized(ofResizeEventArgs& window)
{
}

void ofApp::dragged(ofDragInfo& dragged)
{
}

void ofApp::messageReceived(ofMessage& message)
{
}
