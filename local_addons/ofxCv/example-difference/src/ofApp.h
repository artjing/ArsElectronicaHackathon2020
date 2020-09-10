#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#define W 640
#define H 480

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofPixels previous;
    int indexImg;
	vector < ofImage >  diffs;
	
	// a scalar is like an ofVec4f but normally used for storing color information
	cv::Scalar diffMean;
    ofFbo f;
};
