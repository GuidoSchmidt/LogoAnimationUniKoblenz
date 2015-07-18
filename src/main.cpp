#include "ofApp.h"

int main()
{
  ofGLFWWindowSettings settings;
  settings.width  = 1920/2;
  settings.height = 1080/2;

  auto window = ofCreateWindow(settings);
  auto app    = make_shared<ofApp>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}
