#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#include "constants.hpp"


class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
        bool guiON;
        ofxPanel gui;
        ofParameter < int > emotionState;
        void emotionCallback(int& nEmotion);
    
    
		ofTrueTypeFont font;
		ofxOscSender maxSender, pythonSender;
    
    glm::vec2 resImg;
    
    std::vector < std::string > states;
    void stateUpdate();
    
    // utils
    void logPrint(std::string logPrint){
#ifdef DEBUGMODE
        cout << logPrint << endl;
#endif
    }
    
};
