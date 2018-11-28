/**********************************************************************

  ofxProcessFFT.cpp

  
  These classes and functions (in ofxEasyFft_TP.cpp, and 
  ofxProcessFFT.cpp) are based on files from the ofxFft addon: 

    https://github.com/kylemcdonald/ofxFft

  written by Kyle McDonald last updated in 2016

  These files have been adapted to work with two channel (stereo) input, 
  and some functions not needed for this application have been removed.
  The purpose is to get the dominiant hz frequency from an audio input 
  source. 


**********************************************************************/

#include "ofxProcessFFT_TP.h"

void ProcessFFT_TP::setup(){
    
    scaleFactor = 10000;
    numBins = 16384;
    
    fft.setup(numBins); //default
    fft.setUseNormalization(false);
    
    exponent = 1.0;
    
    numFFTbins = 128;
    FFTpercentage = 0.14;
    
    delta = loudestBandL = loudestBandR = maxSoundL = maxSoundR = 0;
    
    normalize = false;
    volumeRange = 400; //only used if normalize is false

}

//---------------------------------------------
void ProcessFFT_TP::update(){
    fft.update();
    
    calculateFFT(fft.getBinsL(),fft.getBinsR(), FFTpercentage, numFFTbins);

}

//---------------------------------------------
void ProcessFFT_TP::calculateFFT(vector<float>&buffer, vector<float>&bufferR, float _FFTpercentage, int _numFFTbins){
    
    this->numFFTbins = _numFFTbins;
    this->FFTpercentage = _FFTpercentage;
    
    fftSpectrumL.clear(); //empty it all
    fftSpectrumR.clear(); //empty it all
    
    float loudBand = 0;
    maxSoundL = 0;
    maxSoundR = 0;
    float freqDelta = 0;
    
    for(int i = 0; i<numFFTbins; i++){
        fftSpectrumL.push_back(0); //init the vector for each pass
        fftSpectrumR.push_back(0); //init the vector for each pass
    }
    
    //sort through and find the loudest sound
    //use the loudest sound to normalize it to the proper range of 0-1
    //drop all those values into the fftSpectrum
    
    //average first (Left Channel)
    for(int i=0; i<fftSpectrumL.size(); i++){ //for the number of columns
        float bin_size = buffer.size()*FFTpercentage;

        for (int j=(bin_size*((float)i/numFFTbins)); j<bin_size*((float)1/numFFTbins)+(bin_size*((float)i/numFFTbins)) ; j++) { //for each i position, average the values in i's+offset
            fftSpectrumL[i] = fftSpectrumL[i] + buffer[j]*10000; //sum values in each section of buffers. Multiply by 10000 so you're not dealing with tiny numbers.
        }
        
        fftSpectrumL[i] = abs((fftSpectrumL[i]/(bin_size*(float)1/numFFTbins))*(1+pow(i, exponent)/numFFTbins));//Then make low frequency values weighted lower than high frequency with pow
        
        //find maximum band
        if (maxSoundL<fftSpectrumL[i]) {
            maxSoundL = fftSpectrumL[i];
            loudestBandL = i;
        }
    }

    //average first (Right Channel)
    for(int i=0; i<fftSpectrumR.size(); i++){ //for the number of columns
        float bin_sizeR = bufferR.size()*FFTpercentage;

        for (int j=(bin_sizeR*((float)i/numFFTbins)); j<bin_sizeR*((float)1/numFFTbins)+(bin_sizeR*((float)i/numFFTbins)) ; j++) { //for each i position, average the values in i's+offset
            fftSpectrumR[i] = fftSpectrumR[i] + bufferR[j]*10000; //sum values in each section of buffers. Multiply by 10000 so you're not dealing with tiny numbers.
        }

        fftSpectrumR[i] = abs((fftSpectrumR[i]/(bin_sizeR*(float)1/numFFTbins))*(1+pow(i, exponent)/numFFTbins));//Then make low frequency values weighted lower than high frequency with pow

        //find maximum band
        if (maxSoundR<fftSpectrumR[i]) {
            maxSoundR = fftSpectrumR[i];
            loudestBandR = i;
        }
    }
    
    freqDelta = freqDelta/numFFTbins;
        
}


float ProcessFFT_TP::getUnScaledLoudestValue(){
    //This returns the unnormalized value of the current loudest sound - useful for detecting whether the volume input is low or really high
    return maxSoundL;
}

vector<float> ProcessFFT_TP::getSpectrum(){
    return fftSpectrumL;
}

bool ProcessFFT_TP::getNormalized(){
    return normalize;
}

float ProcessFFT_TP::getLoudBand(){
    return loudestBandL; //Todo: this needs to be an average
}

float ProcessFFT_TP::getFFTpercentage(){
    return FFTpercentage;
}

float ProcessFFT_TP::getExponent(){
    return FFTpercentage;
}

int ProcessFFT_TP::getNumFFTbins(){
    return numFFTbins;
}

//SETTERS
void ProcessFFT_TP::setFFTpercentage(float _FFTpercentage){
    this->FFTpercentage=_FFTpercentage;
}

void ProcessFFT_TP::setExponent(float _exponent){
    this->exponent=_exponent;
}

void ProcessFFT_TP::setNumFFTBins(int _numFFTBins){
    this->numFFTbins = _numFFTBins;
}

void ProcessFFT_TP::setNormalize(bool _normalize){
    this->normalize = _normalize;
}

void ProcessFFT_TP::setVolumeRange(int _volumeRange){
    this->volumeRange = _volumeRange;
}
