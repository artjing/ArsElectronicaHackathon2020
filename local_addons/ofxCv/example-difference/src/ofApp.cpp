#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
    ofEnableAlphaBlending();

	cam.setup(W, H);
	
	// imitate() will set up previous and diff
	// so they have the same size and type as cam
	imitate(previous, cam);
    
    f.allocate(W, H, GL_RGBA);
    
    // loops over 30 frames;
    diffs.resize(20);
    for(int i = 0; i < diffs.size(); i++){
        imitate(diffs[i], cam);
    }
    indexImg = 0;
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		// take the absolute difference of prev and cam and save it inside diff
		absdiff(cam, previous , diffs[indexImg]);
		diffs[indexImg].update();
		
		// like ofSetPixels, but more concise and cross-toolkit
		copy(cam, previous);
		
		// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
		diffMean = mean(toCv(diffs[indexImg]));
		
		// you can only do math between Scalars,
		// but it's easy to make a Scalar from an int (shown here)
		diffMean *= Scalar(50);
        
        indexImg ++;
        indexImg %= diffs.size();
	}
}

void ofApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
    for( int i = diffs.size() - 1; i > -1; i--){
        int currIndex = (indexImg + i - 1) % diffs.size();
        int prop = (int)(255  * float(i)/ diffs.size());
//        cout << "prop = " << ofToString(prop) << " & i = " << ofToString(i) << " for currIndex  = " << ofToString(currIndex) << endl;
        
        ofSetColor(255, prop);
        diffs[currIndex].draw(W, 0);
    }
}
