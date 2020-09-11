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
    
	pythonSender.setup(HOST, PYTHONPORT);
    maxSender.setup(HOST, MAXPORT);
    emotioReceiver.setup(EMOTIONPORT);
    
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
    while(emotioReceiver.hasWaitingMessages()){

        // get the next message
        ofxOscMessage m;
        emotioReceiver.getNextMessage(m);
        
        if(m.getAddress() == "/happy"){
            cout << "happiness val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/angry"){
            cout << "angriness val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/disgusted"){
            cout << "disgusted val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/fear"){
            cout << "fear val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/surprise"){
            cout << "surprise val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/neutra"){
            cout << "neutra val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/sad"){
            cout << "sad val is : " << ofToString(m.getArgAsFloat(0)) << endl;
        }
        if(m.getAddress() == "/mainEmotion"){
            cout << "mainEmotion : " << ofToString(m.getArgAsString(0)) << " = " << ofToString(m.getArgAsFloat(1))<< endl;
        }
    }
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
}


void ofApp::stateUpdate(){
    if(emotionState >= states.size())emotionState = 0;
    logPrint("new state is : " + states[emotionState]);
}



void ofApp::emotionCallback(int& nEmotion){

    emotionState = nEmotion;
    
    ofxOscMessage mPython, mMax;
    mPython.setAddress("/emotion");
    mPython.addStringArg(states[emotionState]);
    mMax.setAddress("/emotion");
    mMax.addStringArg(states[emotionState]);
    
    maxSender.sendMessage(mMax, false);
    pythonSender.sendMessage(mPython, false);
    
    logPrint("sending new emotionState : " + ofToString(states[emotionState]));
}
