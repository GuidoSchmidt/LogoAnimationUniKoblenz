project(ofLogoAnimation)
set(APP ofLogoAnimation)

cmake_minimum_required(VERSION 2.8)

include(../of/openFrameworks.cmake)

ofxaddon(ofxYahooWeather)
ofxaddon(ofxTween)
ofxaddon(ofxGui)
ofxaddon(ofxLayerMask)
ofxaddon(ofxAssimpModelLoader)

set(SOURCES
    src/Star.cpp
    src/Rain.cpp
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
