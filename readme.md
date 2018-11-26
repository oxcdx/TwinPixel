TwinPixel 
================

https://github.com/oxcdx/TwinPixel 

Twin Pixel is an openFrameworks application that captures and 
displays/plays the dominant hue from each side of the frame of a video 
input source and the dominant frequency from left and right channels of 
an audio input source. The rate at which this is calcluated can be 
changed using the 'frameRate' variable in ofApp.cpp.

It uses ofxOpenCV and ofxFft addons and is based on code from the 
following examples:

https://github.com/openframeworks/openFrameworks/tree/patch-release/examples/sound/audioOutputExample
https://github.com/kylemcdonald/ofxFft/tree/master/example-process-input
https://github.com/twobeers83/hueVideo

It was developed to create a real time input version of what is achieved 
by Simon Crowe here:

https://github.com/simoncrowe/dominant_frequencies_video

Various art related applications of this project are detailed at:

https://oc-d.co.uk/portfolio/twin-pixel/ 
https://compiler.zone/
   
<br><br>
<b>Notes for use on Raspberry Pi (or FTTW in general)</b>

Could not get it to work using Kiss, only using FTTW. 

ofxFft is set to use (FTTW by default on linuxarmv6l and linuxarmv7l). However the following steps were needed to make it work:

First install the FTTW library using:

    sudo apt-get install libfftw3-dev
    
Then add: 

     PROJECT_EXTERNAL_SOURCE_PATHS = ../../../addons/ofxFft/src     

To config.make

Do NOT use 'ofxFft' in addons.make

Compile using make
