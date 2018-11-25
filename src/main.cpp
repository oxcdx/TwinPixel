#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
    //ofAppGlutWindow window;

    ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);

    //settings if OF_FULLSCREEN causing problems on linux, or more flexibility needed across multiple monitors
//    ofGLFWWindowSettings settings;
//    settings.windowMode = OF_WINDOW;
//    settings.setSize(3840, 1080);
//    settings.setPosition({0.0f,0.0f});
//    settings.decorated = false;
//    settings.multiMonitorFullScreen = false;
//    ofCreateWindow(settings);

    //or:
//    ofAppGLFWWindow window;
//    window.setMultiDisplayFullscreen(true);
//    ofSetupOpenGL(&window,3840,1080,OF_FULLSCREEN);

    ofRunApp(new ofApp());
}
