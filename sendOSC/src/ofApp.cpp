#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

#ifdef DEBUGMODE
    ofSetLogLevel(OF_LOG_VERBOSE);
#endif
        
	ofSetWindowTitle("send OSC with GUI pannel");
    ofSetFrameRate(60);
	ofSetVerticalSync(true);
    
    resImg = glm::vec2(ofGetWidth(), ofGetHeight());
    
	sender.setup(HOST, MAXPORT);
    
    emotionState = 0;
    states.push_back("sad");
    states.push_back("angry");
    states.push_back("happy");
    states.push_back("hopeless");
    
    
    
    
    guiON = false;
    gui.setup("Control Panel");
    gui.add(emotionState.set("emotionState", 0, 0, states.size() - 1));
    gui.setPosition(resImg.x - 220, 20);
    
    emotionState.addListener(this, &ofApp::emotionCallback);

    
}

//--------------------------------------------------------------
void ofApp::update(){
//	ofxOscMessage m;
//	m.setAddress("/mouse/position");
//	m.addFloatArg(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.f, 1.f, true));
//	m.addFloatArg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.f, 1.f, true));
//	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(255, 100);
    
    
    if(guiON)gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' '){
        emotionState +=1;
        stateUpdate();
    }
    
    if(key == 'g')guiON = !guiON;
    
	// send a test message
	if(key == 'a' || key == 'A'){
		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(1);
		m.addFloatArg(3.5f);
		m.addStringArg("hello");
		m.addFloatArg(ofGetElapsedTimef());
		sender.sendMessage(m, false);
	}

	// send an image
	// note: the size of the image depends greatly on your network buffer sizes,
	// if an image is too big the message won't come through and you may need
	// to break it up into multiple blobs
//	if(key == 'i' || key == 'I'){
//
//		// load image from buffer
//		img.load(imgAsBuffer);
//
//		// send as a binary blob
//		ofxOscMessage m;
//		m.setAddress("/image");
//		m.addBlobArg(imgAsBuffer);
//		sender.sendMessage(m);
//		ofLog() << "sending image with size: " << imgAsBuffer.size();
//	}
}


void ofApp::stateUpdate(){
    if(emotionState >= states.size())emotionState = 0;
    logPrint("new state is : " + states[emotionState]);
}



void ofApp::emotionCallback(int& nEmotion){

    emotionState = nEmotion;
    
    ofxOscMessage m;
    m.setAddress("/emotion");
    m.addStringArg(states[emotionState]);
    sender.sendMessage(m, false);
    logPrint("sending new emotionState : " + ofToString(states[emotionState]));
}
