#pragma once

#include "ofMain.h"
#include "ofxOMXVideoGrabber.h"
#include "ofxCv.h"
#include "ofxProcessFFT_TP.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
    void exit();

    void audioOut(float* output, int bufferSize, int nChannels);

	// adapted from ofxOMXCameraGrabber ofxCV example  
    ofxOMXVideoGrabber videoGrabber;
    ofxOMXCameraSettings settings;
    
    int threshold1;
    int threshold2;
    int apertureSize;
    bool L2gradient;
    
    cv::Mat edgeMat;
    cv::Mat grayMat;
    cv::Mat cameraMat;
	
    // adapted from ofxFft
    ProcessFFT_TP fft;

    // sound stuff
    ofSoundStream soundStream;

    float 	pan;
    int		sampleRate;
    float 	volume;

    vector <float> lAudio;
    vector <float> rAudio;

    // for the simple sine wave synthesis
    float targetFrequencyL, targetFrequencyR, phaseL, phaseR, phaseAdderL, phaseAdderR, phaseAdderTargetL, phaseAdderTargetR;

    // colour / hue stuff (ofxOpenCV)
    float freq;
    float freqHue, freqHue2;

    ofPixels pixels;
    //ofVideoGrabber grabber;
    ofColor averageColor, averageColor2;
    ofColor averageHue, averageHue2;

    // frame rate
    float frameRate, nextTrigger;
    
    int camWidth, camHeight;

};
