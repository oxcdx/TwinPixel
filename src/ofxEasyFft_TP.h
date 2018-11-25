#pragma once

#include "ofMain.h"
#include "ofxFft.h"

class ofxEasyFft_TP : public ofBaseSoundInput{
public:
    ofxEasyFft_TP();
    ~ofxEasyFft_TP();
	void setup(int bufferSize = 512,
						 fftWindowType windowType = OF_FFT_WINDOW_HAMMING,
						 fftImplementation implementation = OF_FFT_BASIC,
						 int audioBufferSize = 256,
                         int audioSampleRate = 48000);
	void setUseNormalization(bool useNormalization);
	void update();
	vector<float>& getAudio();
    vector<float>& getBinsL();
    vector<float>& getBinsR();
	
    void audioIn(float* input, int bufferSize, int nChannels);
	
    ofxFft* fftL;
    ofxFft* fftR;

	ofSoundStream stream;
private:
	bool useNormalization;
	ofMutex soundMutex;
    vector<float> audioFrontL, audioFrontR, audioMiddleL, audioMiddleR, audioBackL, audioBackR, audioRawL, audioRawR;
    vector<float> binsL;
    vector<float> binsR;
	
	void normalize(vector<float>& data);

};
