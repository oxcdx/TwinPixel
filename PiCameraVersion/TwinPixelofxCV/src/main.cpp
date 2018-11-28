#include "ofMain.h"
#include "ofApp.h"

int main() {

    //ofSetupOpenGL(1280, 720, OF_WINDOW);

    //settings if OF_FULLSCREEN causing problems on linux, or more flexibility needed across multiple monitors
//    ofGLFWWindowSettings settings;
//    settings.windowMode = OF_WINDOW;
//    settings.setSize(3840, 1080);
//    settings.setPosition({0.0f,0.0f});
//    settings.decorated = false;
//    settings.multiMonitorFullScreen = false;
//    ofCreateWindow(settings);

    //or:
    //ofAppGLFWWindow window;
    //window.setMultiDisplayFullscreen(false);
    ofSetupOpenGL(800,600,OF_FULLSCREEN);

    ofRunApp(new ofApp());
}
