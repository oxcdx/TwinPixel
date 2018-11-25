#pragma once

#include "ofApp.h"
#include "ofxProcessFFT_TP.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void exit();

    void audioOut(float* output, int bufferSize, int nChannels);

    //adapted from ofxFft
    ProcessFFT_TP fft;

    // sound stuff
    ofSoundStream soundStream;

    float 	pan;
    int		sampleRate;
    float 	volume;

    vector <float> lAudio;
    vector <float> rAudio;

    //for the simple sine wave synthesis
    float targetFrequencyL, targetFrequencyR, phaseL, phaseR, phaseAdderL, phaseAdderR, phaseAdderTargetL, phaseAdderTargetR;

    //colour / hue stuff (ofxOpenCV)
    float freq;
    float freqHue, freqHue2;

    ofPixels pixels;
    ofVideoGrabber grabber;
    ofColor averageColor, averageColor2;
    ofColor averageHue, averageHue2;

    //frame rate
    float frameRate, nextTrigger;

};
