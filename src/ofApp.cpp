#include "ofApp.h"

#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>

ofImage img_logoUniKoblenz;
ofImage img_errorBackground;
ofImage img_noiseBackground;

// Condition codes
bool    isStorm{false};
bool    isSnow{false};
bool    isRain{false};
bool    isDusty{false};
bool    isWindy{false};
bool    isCold{false};
bool    isCloudy{false};
bool    isClear{false};
bool    isSun{false};
bool    isThunderstorm{false};
bool    connectionIssue{false};

bool    animateDaytime{false};
bool    drawGui{false};
int     starsAlpha{255};
ofParameter<float> dayTimeFromGui{0.0};
float   sunsetTime, sunriseTime;

// Map for lookup of Yahoo weather codes
auto buildConditionCodeMap() -> std::map<int, tuple<string, string> >
{
    std::map<int, std::tuple<std::string, std::string> > conditionCodeMap {
        // Storm
        {  0, tuple<string, string>{"Tornado", "Storm"} },
        {  1, tuple<string, string>{"Tropical Storm", "Storm"} },
        {  2, tuple<string, string>{"Hurricane", "Storm"} },
        {  3, tuple<string, string>{"Severe thunderstorms", "Storm"} },
        {  3, tuple<string, string>{"Thunderstorms", "Storm"} },
        { 26, tuple<string, string>{"Cloudy Tornado", "Storm"} },
        // Snow & Rain
        {  5, tuple<string, string>{"Mixed Rain And Snow", "Snow & Rain"} },
        {  6, tuple<string, string>{"Mixed Rain And Sleet", "Snow & Rain"} },
        {  7, tuple<string, string>{"Mixed Snow And Sleet", "Snow & Rain"} },
        { 17, tuple<string, string>{"Hail", "Snow & Rain"} }, // Hagel
        { 18, tuple<string, string>{"Sleet", "Snow & Rain"} }, // Eisregen
        { 35, tuple<string, string>{"Mixed Rain And Hail", "Snow & Rain"} },
        { 42, tuple<string, string>{"Scattered Snow Showers", "Snow & Rain"} },
        { 46, tuple<string, string>{"Snow Showers", "Snow & Rain"} },
        // Snow
        { 13, tuple<string, string>{"Snow Flurries", "Snow"} },
        { 14, tuple<string, string>{"Light Snow Showers", "Snow"} },
        { 15, tuple<string, string>{"Blowing Snow", "Snow"} },
        { 16, tuple<string, string>{"Snow", "Snow"} },
        { 41, tuple<string, string>{"Heavy Snow", "Snow"} },
        { 43, tuple<string, string>{"Heavy Snow", "Snow"} },
        // Rain
        {  8, tuple<string, string>{"Freezing Drizzle", "Rain"} },
        {  9, tuple<string, string>{"Drizzle", "Rain"} },
        { 10, tuple<string, string>{"Freezing Rain", "Rain"} },
        { 11, tuple<string, string>{"Showers", "Rain"} },
        { 12, tuple<string, string>{"Showers", "Rain"} },
        { 40, tuple<string, string>{"Scattered Showers", "Rain"} },
        // Dusty
        { 19, tuple<string, string>{"Dusty", "Dusty"} },
        { 20, tuple<string, string>{"Foggy", "Dusty"} },
        { 21, tuple<string, string>{"Haze", "Dusty"} },
        { 22, tuple<string, string>{"Smoky", "Dusty"} },
        // Windy
        { 23, tuple<string, string>{"Blustery", "Windy"} },
        { 24, tuple<string, string>{"Windy", "Windy"} },
        // Cold
        { 25, tuple<string, string>{"Cold", "Cold"} },
        // Cloudy
        { 27, tuple<string, string>{"Mostly Cloudy (Night)", "Cloudy"} },
        { 28, tuple<string, string>{"Mostly Cloudy (Day)", "Cloudy"} },
        { 29, tuple<string, string>{"Partly Cloudy (Night)", "Cloudy"} },
        { 30, tuple<string, string>{"Partly Cloudy (Day)", "Cloudy"} },
        { 44, tuple<string, string>{"Partly Cloudy", "Cloudy"} },
        // Clear
        { 31, tuple<string, string>{"Clear (Night)", "Clear"} },
        // Sun
        { 32, tuple<string, string>{"Sunny", "Sunny"} },
        { 33, tuple<string, string>{"Fair (Night)", "Sunny"} },
        { 34, tuple<string, string>{"Fair (Day)", "Sunny"} },
        { 36, tuple<string, string>{"Hot", "Sunny"} },
        // Thunderstorms
        { 37, tuple<string, string>{"Isolated Thunderstorms", "Sunny"} },
        { 38, tuple<string, string>{"Scattered Thunderstorms", "Sunny"} },
        { 45, tuple<string, string>{"Thundershowers", "Sunny"} },
        { 47, tuple<string, string>{"Isolated Thundershowers", "Sunny"} },
        // Not Available
        { 3200, tuple<string, string>{"Not Available", "Connection Error"} },
    };
    return conditionCodeMap;
}

// Color maps based on time
auto getColorBasedOnTime(float sunriseTime, float sunsetTime) -> ofColor
{
    ofColor sunrise{23, 172, 156};
    ofColor daytime{91, 220, 255};
    ofColor sunset{233, 130, 103};
    ofColor nighttime{8, 6, 20};

    float delta = 3.0;

    if(dayTimeFromGui < sunriseTime)
    {
        starsAlpha = 255;
        return nighttime;
    }
    else if(dayTimeFromGui > sunriseTime && dayTimeFromGui < sunriseTime + delta)
    {
        float driver = ofClamp((dayTimeFromGui - sunriseTime), 0.0, 1.0);
        starsAlpha = 255 - (driver * 255);
        return nighttime.getLerped(sunrise, driver);
    }
    else if(dayTimeFromGui > sunriseTime + delta && dayTimeFromGui < sunsetTime - delta)
    {
        float driver = ofClamp((dayTimeFromGui - (sunriseTime + delta)), 0.0, 1.0);
        starsAlpha = 0;
        return sunrise.getLerped(daytime, driver);
    }
    else if(dayTimeFromGui > sunsetTime - delta && dayTimeFromGui < sunsetTime)
    {
        float driver = ofClamp((dayTimeFromGui - (sunsetTime - delta)), 0.0, 1.0);
        starsAlpha = driver * 120;
        return daytime.getLerped(sunset, driver);
    }
    else if(dayTimeFromGui > sunsetTime)
    {
        float driver = ofClamp((dayTimeFromGui - sunsetTime), 0.0, 1.0);
        starsAlpha = 120 + (driver * (255-120));
        return sunset.getLerped(nighttime, driver);
    }
    else
    {
        starsAlpha = 255;
        return nighttime;
    }
}

// Convert the time string into a float to work with
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
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(50);

    // Images
    img_logoUniKoblenz.load("logo_uni-koblenz.jpg");
    img_errorBackground.load("error.png");
    img_noiseBackground.load("noise.png");

    // Tweens
    const unsigned delay    = 2000;
    const unsigned duration = 5000;
    tweenLogoBlend.setParameters(1, easingCubic, ofxTween::easeOut, 0, 255, duration, delay);
    tweenDaytime.setParameters(0, easingLinear, ofxTween::easeInOut, 0, 24.0, 24000, 0.0);

    // Fonts
    font.loadFont("InputMono-Regular.ttf", 20);

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
    // Wind
    auto windSpeedString = yahooWeather.getWindSpeed();
    auto windDirectionString   = yahooWeather.getWindDirection();
    std::cout << "Wind speed: " << windSpeedString << std::endl;
    std::cout << "Wind direction: " << windDirectionString << std::endl;
    extern float windSpeed;
    float windDirection = 1.0;
    if((int)windDirectionString < 180)
        windDirection = 1.0;
    if((int)windDirectionString >= 180)
        windDirection = -1.0;

    windSpeed = 1.5f; // (float)windSpeedString/2.0f * windDirection;
    // Weather condition
    int condititonCode = yahooWeather.getWeatherConditionCode();
    // @TODO Testing
    condititonCode     = 40;
    auto conditionCodeMap = buildConditionCodeMap();
    std::cout << std::get<1>( conditionCodeMap[condititonCode] ) << std::endl;
    std::string conditionCodeString = std::get<1>( conditionCodeMap[condititonCode] );

    // UI
    daytimeSlide.addListener(this, &ofApp::daytimeChanged);
    daytimeAnimate.addListener(this, &ofApp::daytimeAnimated);
    gui.setup();
    ofColor guiBackgroundColor(255, 255, 255);
    gui.setBackgroundColor(guiBackgroundColor);
    gui.add(dayTimeFromGui.set("Daytime", 0.0, 0.0, 24.0));
    gui.add(daytimeAnimate.setup("Animate", 30, 30));

    // UI2
    guiCanvas = new ofxUISuperCanvas{"Weather Codes"};
    set<string> weatherConditionNames;
    vector<string> weatherConditions;
    for(auto it : conditionCodeMap)
    {
        weatherConditionNames.insert(std::get<1>(it.second));
    }
    copy(weatherConditionNames.begin(), weatherConditionNames.end(), back_inserter(weatherConditions));
    dropDownList = guiCanvas->addDropDownList("Select Weatherw", weatherConditions );
    dropDownList->setAllowMultiple(false);
    ofAddListener(guiCanvas->newGUIEvent, this, &ofApp::guiEvent);
    guiCanvas->setVisible(drawGui);
    guiCanvas->setColorBack(ofColor{255, 255, 255, 128});
    guiCanvas->setColorFill(ofColor{0, 0, 0, 200});
    guiCanvas->addSpacer("Daytime", 2);
    guiCanvas->addLabel("Daytime");
    guiCanvas->addButton("Animate", 1.0);

    // Weather conditions
    if(conditionCodeString ==  "Storm")
    {
        isStorm = true;
    }
    if(conditionCodeString ==  "Snow & Rain")
    {
        isSnow = true;
        isRain = true;
    }
    if(conditionCodeString ==  "Snow")
    {
        isSnow = true;
    }
    if(conditionCodeString ==  "Rain")
    {
        isRain = true;
    }
    if(conditionCodeString ==  "Dusty")
    {
        isDusty = true;
    }
    if(conditionCodeString ==  "Windy")
    {
        isWindy = true;
    }
    if(conditionCodeString ==  "Cold")
    {
        isCold = true;
    }
    if(conditionCodeString ==  "Cloudy")
    {
        isCloudy = true;
    }
    if(conditionCodeString ==  "Clear")
    {
        isClear = true;
    }
    if(conditionCodeString ==  "Sun")
    {
        isSun = true;
    }
    if(conditionCodeString ==  "Thunderstorms")
    {
        isThunderstorm = true;
    }
    if(conditionCodeString ==  "Connection Error")
    {
        connectionIssue = true;
    }

    // Objects
    // Stars
    for(unsigned i = 0; i < 200; i++)
    {
        stars.push_back(new Star{ ofVec2f(ofRandom(0.0, ofGetWidth()), ofRandom(0.0, ofGetHeight())),
                                  ofRandom(0.5, 1.0),
                                  ofRandom(0.0, 200.0)});
    }

    model.loadModel("uni.obj");
    model.setScale(0.85, 0.85, 0.85);
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);

    // Rain
    rain = new Rain{200, 9.8f, 35};

    // Snow
    snow = new Snow{200, 3.8f};
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    if(name == "Select Weather")
    {
        ofxUIDropDownList *dropdown = (ofxUIDropDownList *) e.widget;
    }
}

// UI Callbacks
void ofApp::daytimeChanged(float& daytime)
{
    dayTimeFromGui = daytime;
}

void ofApp::daytimeAnimated()
{
    animateDaytime = true;
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
        stars.at(i)->draw(starsAlpha);
    }
}

void ofApp::draw()
{
    // Background
    ofColor dynamicBackgroundColor          = getColorBasedOnTime(sunriseTime, sunsetTime);
    ofColor dynamicBackgroundColorSaturated = dynamicBackgroundColor;
    dynamicBackgroundColorSaturated.setSaturation(dynamicBackgroundColorSaturated.getSaturation() - 80);
    dynamicBackgroundColorSaturated.setBrightness(dynamicBackgroundColorSaturated.getBrightness() + 30);
    dynamicBackgroundColorSaturated.setHue(dynamicBackgroundColorSaturated.getHue() + 15);
    ofBackgroundGradient(dynamicBackgroundColor, dynamicBackgroundColorSaturated, OF_GRADIENT_LINEAR);

    ofEnableAlphaBlending();
    float dust = 60;
    if(isDusty)
    {
        dust = 180;
    }
    ofSetColor(128, 128, 128, dust);
    img_noiseBackground.draw(0, 0, ofGetWidth(), ofGetHeight());

    drawStars();

    // Connection issues
    if(connectionIssue)
    {
        ofNoFill();
        ofDisableAlphaBlending();
        img_errorBackground.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(234, 67, 132);
        font.drawString("CONNECTION ERROR!", ofGetWidth()/2 - 100, ofGetHeight()/2 - 240);
        ofSetColor(250, 250, 250);
        font.drawString("Please check your internet connection\nand restart application.", ofGetWidth()/2 - 220, ofGetHeight()/2 - 200 + 24);
    }
    // Rendering
    else
    {
        // Sun
        ofSetCircleResolution(100);
        ofFill();
        ofColor sunColor = ofColor{255, 252, 218};
        ofSetColor(dynamicBackgroundColor);
        float sunSize = (0.6f * sin((dayTimeFromGui/24.0f) * M_PI) + 1.0f) * 60.0f;
        float sunPosY = (ofGetHeight() *  2.0f/3.0f) * sin(dayTimeFromGui/24.0f * M_PI);
        ofCircle(ofGetWidth()/2 + 200, ofGetHeight() + sunSize - sunPosY, sunSize);

        // Moon
        float moonSize = (sin(+M_PI/2.0f + (dayTimeFromGui/24.0f) * 2.0f * M_PI) + 1.0f) * 45.0f;
        float moonPosY = (ofGetHeight() *  2.0f/3.0f) * sin(+M_PI/2.0f + (dayTimeFromGui/24.0f) * 2.0f * M_PI);
        float moonAlpha = 255.0f * sin(+M_PI/2.0f + (dayTimeFromGui/24.0f) * 2.0f * M_PI);
        ofSetColor(220, 220, 220, moonAlpha);
        ofCircle(ofGetWidth()/2 - 200, ofGetHeight() + moonSize - moonPosY, moonSize);

        /*
        unsigned transparency = 0; //tweenLinear.update();
        ofSetColor(transparency, transparency, transparency);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        img_logoUniKoblenz.draw(0, 0, ofGetWidth(), ofGetHeight());

        ofPushMatrix();
        ofTranslate(model.getPosition());
        ofRotate(ofGetMouseX(), 0, 1, 0);
        ofTranslate(-model.getPosition());
        model.drawVertices();
        ofPopMatrix();
        */

        //--- Weather conditions
        if(isStorm)
        {
            //@TODO
        }
        if(isRain)
        {
            ofColor dynamicBackgroundColorRain = dynamicBackgroundColorSaturated;
            dynamicBackgroundColorRain.setBrightness(dynamicBackgroundColorRain.getBrightness() + 150);
            dynamicBackgroundColorRain.setSaturation(dynamicBackgroundColorRain.getSaturation() / 2);
            rain->draw(dynamicBackgroundColorRain, 255);
        }
        if(isSnow)
        {
            snow->draw(255, 255);
        }
        if(isDusty)
        {
            ofFill();
            ofSetColor(255, 255, 255, 128);
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
        }
        if(isWindy)
        {
            //@TODO
        }
        if(isCold)
        {
            //@TODO
        }
        if(isSun)
        {
            //@TODO
        }
        if(isThunderstorm)
        {
            //@TODO
        }

        //--- UI
        if(drawGui)
        {
            gui.draw();
        }
    }
}

void ofApp::exit()
{
    delete guiCanvas;
}

void ofApp::keyPressed(ofKeyEventArgs& key)
{
}

void ofApp::keyReleased(ofKeyEventArgs& key)
{
    std::cout << key.keycode << std::endl;
    switch (key.keycode) {
    case 71:
        drawGui = !drawGui;
        guiCanvas->setVisible(drawGui);
        break;
    default:
        break;
    }
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
