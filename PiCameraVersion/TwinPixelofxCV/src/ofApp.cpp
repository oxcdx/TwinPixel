#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {

    frameRate = 24; // rate at which dominant hue and dominant hz frequency are calculated
    nextTrigger = 1000/frameRate;
    camWidth = 320;
    camHeight = 240;
    
    // oxfOMXCam ofxCV example
    
    settings.sensorWidth = camWidth;
    settings.sensorHeight = camHeight;
    settings.framerate = frameRate;
    settings.enableTexture = true;
    settings.enablePixels = true;
    //settings.saturation = 100;
    videoGrabber.setup(settings);
    
	ofSetVerticalSync(true);	
    ofSetFrameRate(frameRate);

	fft.setup();

    int bufferSize		= 512;
    sampleRate 			= 48000;
    phaseL 				= 0;
    phaseAdderL			= 0.0f;
    phaseAdderTargetL 	= 0.0f;
    phaseR 				= 0;
    phaseAdderR			= 0.0f;
    phaseAdderTargetR 	= 0.0f;
    volume				= 0.2f;

    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);

    soundStream.printDeviceList();

    //if you want to set the audio device id to be different than the default
    soundStream.setDeviceID(3);
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);

    ofBackground(0);

    freq = 0;

    //ofSetWindowShape(3840, 1080);
    
}

void ofApp::update() {

	fft.update();
	
    if(videoGrabber.isFrameNew())
    {
		                       
        pixels = videoGrabber.getPixels();
    
        int rSum = 0;
        int gSum = 0;
        int bSum = 0;

        int rSum2 = 0;
        int gSum2 = 0;
        int bSum2 = 0;
    
        for(int x = 0; x < camWidth/2; x++) {
            for(int y = 0; y < camHeight; y++) {
				
                ofColor pixelColor = pixels.getColor(x, y);
                rSum += pixelColor.r;
                gSum += pixelColor.g;
                bSum += pixelColor.b;
            }
        }
        for(int x = camWidth/2; x < camWidth; x++) {
            for(int y = 0; y < camHeight; y++) {

                ofColor pixelColor2 = pixels.getColor(x, y);
                rSum2 += pixelColor2.r;
                gSum2 += pixelColor2.g;
                bSum2 += pixelColor2.b;
            }
        }
    
        int samples = camWidth/4 * camHeight; // The number of pixels we are averaging

        // Update the average color
        averageColor.r = rSum / samples;
        averageColor.b = bSum / samples;
        averageColor.g = gSum / samples;

        averageColor2.r = rSum2 / samples;
        averageColor2.b = bSum2 / samples;
        averageColor2.g = gSum2 / samples;

        //get hue, create new colour and map to sound frequency based on hue
        float hue = (int)averageColor.getHue() * 0.7;
        float hue2 = (int)averageColor2.getHue() * 0.7;

        averageHue = ofColor::fromHsb(hue, 255, 255);
        averageHue2 = ofColor::fromHsb(hue2, 255, 255);

		//for sound related to colour
        //freqHue = ofMap(hue, 0, 255, 82.412, 1174.624);
        //freqHue2 = ofMap(hue2, 0, 255, 82.412, 1174.624);    
    
    }

}

void ofApp::draw() {
	// RPICamera
	//videoGrabber.draw(0,0,ofGetWidth(),ofGetHeight());
    //ofxCv::drawMat(edgeMat, videoGrabber.getWidth(), 0);

    // for 1 pixels
    //ofFill();
    //ofSetColor(averageHue);
    //ofRect(ofGetWidth()/2-1, ofGetHeight()/2, 1, 1);
    //ofSetColor(averageHue2);
    //ofRect(ofGetWidth()/2+1, ofGetHeight()/2, 1, 1);

    //for large squares
    ofPushMatrix();
    ofPushStyle();
    ofFill();
    ofSetColor(averageHue);
    ofRect(0, 0, ofGetWidth()/2, ofGetHeight());
    ofSetColor(averageHue2);
    ofRect((ofGetWidth()/2), 0, ofGetWidth()/2, ofGetHeight());
    ofPopMatrix();
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float* output, int bufferSize, int nChannels){

    // get frequency Hz for every frame length (nextTrigger)
    if(ofGetElapsedTimeMillis()>nextTrigger){

        //using adapted version of ofxFft addon (added stereo)
        float freqPerBin = ofMap(fft.FFTpercentage, 0, 0.23, 0, 5000)/fft.numFFTbins;

        targetFrequencyL = freqPerBin*fft.loudestBandL;
        targetFrequencyR = freqPerBin*fft.loudestBandR;

        //std::cout << "targetFrequencyL: " << targetFrequencyL << endl;
        //std::cout << "targetFrequencyR: " << targetFrequencyR << endl;

        phaseAdderTargetL = (targetFrequencyL / (float) sampleRate) * TWO_PI;
        phaseAdderTargetR = (targetFrequencyR / (float) sampleRate) * TWO_PI;

        pan = 0.5f;
        float leftScale = 1 - pan;
        float rightScale = pan;

        // sin (n) seems to have trouble when n is very large, so we
        // keep phase in the range of 0-TWO_PI like this:
        while (phaseL > TWO_PI){
            phaseL -= TWO_PI;
        }
        while (phaseR > TWO_PI){
            phaseR -= TWO_PI;
        }

        phaseAdderL = 0.95f * phaseAdderL + 0.05f * phaseAdderTargetL;
        phaseAdderR = 0.95f * phaseAdderR + 0.05f * phaseAdderTargetR;
        for (int i = 0; i < bufferSize; i++){
            phaseL += phaseAdderL;
            float sampleL = sin(phaseL);
            phaseR += phaseAdderR;
            float sampleR = sin(phaseR);
            lAudio[i] = output[i*nChannels + 0] = sampleL * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = sampleR * volume * rightScale;
        }

        nextTrigger=ofGetElapsedTimeMillis()+(1000/(frameRate/2));
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

    soundStream.close();

}
