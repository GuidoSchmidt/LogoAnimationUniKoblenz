project(ofLogoAnimation)
set(APP ofLogoAnimation)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include(../of/openFrameworks.cmake)

ofxaddon(ofxYahooWeather)
ofxaddon(ofxTween)
ofxaddon(ofxGui)
ofxaddon(ofxXmlSettings)
ofxaddon(ofxUI)
ofxaddon(ofxLayerMask)
ofxaddon(ofxAssimpModelLoader)

set(SOURCES
    src/Star.cpp
    src/Rain.cpp
    src/Snow.cpp
    src/Cloud.cpp
    src/main.cpp
    src/ofApp.cpp
)

add_executable(
    ${APP}
    MACOSX_BUNDLE
    ${SOURCES}
    ${OFXADDONS_SOURCES}
)

target_link_libraries(
    ${APP}
    ${OPENFRAMEWORKS_LIBRARIES}
)

cotire(${APP})
