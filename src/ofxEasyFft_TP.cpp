/**********************************************************************

  ofxEasyFft_TP.cpp

  
  These classes and functions (in ofxEasyFft_TP.cpp, and ofxProcessFFT)
  are based on files from the ofxFft addon: 

    https://github.com/kylemcdonald/ofxFft

  written by Kyle McDonald last updated in 2016

  These files have been adapted to work with two channel (stereo) input, 
  and some functions not needed for this application have been removed.
  The purpose is to get the dominiant hz frequency from an audio input 
  source. 


**********************************************************************/

#include "ofxEasyFft_TP.h"

ofxEasyFft_TP::ofxEasyFft_TP()
:useNormalization(true) {
}
ofxEasyFft_TP::~ofxEasyFft_TP(){
    stream.close();
}

void ofxEasyFft_TP::setup(int bufferSize, fftWindowType windowType, fftImplementation implementation, int audioBufferSize, int audioSampleRate) {
	if(bufferSize < audioBufferSize) {
		ofLogWarning("ofxEasyFft") << "bufferSize (" << bufferSize << ") less than audioBufferSize (" << audioBufferSize << "), using " << audioBufferSize;
		bufferSize = audioBufferSize;
	}
    fftL = ofxFft::create(bufferSize, windowType, implementation);
    fftR = ofxFft::create(bufferSize, windowType, implementation);
	
    binsL.resize(fftL->getBinSize());
    binsR.resize(fftR->getBinSize());
	
    audioFrontL.resize(bufferSize);
    audioMiddleL.resize(bufferSize);
    audioBackL.resize(bufferSize);
    audioRawL.resize(bufferSize);
    audioFrontR.resize(bufferSize);
    audioMiddleR.resize(bufferSize);
    audioBackR.resize(bufferSize);
    audioRawR.resize(bufferSize);
	
    stream.getDeviceList();
    stream.setup(0, 2, audioSampleRate, audioBufferSize, 2);
    stream.setInput(this);
}

void ofxEasyFft_TP::setUseNormalization(bool useNormalization) {
	this->useNormalization = useNormalization;
}

void ofxEasyFft_TP::update() {
	soundMutex.lock();
    audioFrontL = audioMiddleL;
    audioFrontR = audioMiddleR;
	soundMutex.unlock();
	
    fftL->setSignal(&audioFrontL[0]);
    float* curFft = fftL->getAmplitude();
    copy(curFft, curFft + fftL->getBinSize(), binsL.begin());
    normalize(binsL);

    fftR->setSignal(&audioFrontR[0]);
    float* curFftR = fftR->getAmplitude();
    copy(curFftR, curFftR + fftR->getBinSize(), binsR.begin());
    normalize(binsR);

}

vector<float>& ofxEasyFft_TP::getAudio() {
    return audioFrontL;
    return audioFrontR;
}

vector<float>& ofxEasyFft_TP::getBinsL() {
    return binsL;
}
vector<float>& ofxEasyFft_TP::getBinsR() {
    return binsR;
}

void ofxEasyFft_TP::audioIn(float* input, int bufferSize, int nChannels) {

    for (size_t i = 0; i < bufferSize; i++){
        audioRawL[i] = input[i*2 + 0];
        audioRawR[i] = input[i*2 + 1];
    }

//    if(audioRawL.size() > bufferSize) {
//        copy(audioRawL.begin() + bufferSize, audioRawL.end(), audioRawL.begin()); // shift old
//    }
//    copy(input, input + bufferSize, audioRawL.end() - bufferSize); // push new
    copy(audioRawL.begin(), audioRawL.end(), audioBackL.begin());
    normalize(audioBackR);
    copy(audioRawR.begin(), audioRawR.end(), audioBackR.begin());
    normalize(audioBackL);

    soundMutex.lock();
    audioMiddleL = audioBackL;
    audioMiddleR = audioBackR;
    soundMutex.unlock();

}



void ofxEasyFft_TP::normalize(vector<float>& data) {
	if(useNormalization) {
		float maxValue = 0;
		for(int i = 0; i < data.size(); i++) {
			if(abs(data[i]) > maxValue) {
				maxValue = abs(data[i]);
			}
		}
		for(int i = 0; i < data.size(); i++) {
			data[i] /= maxValue;
		}
	}
}
