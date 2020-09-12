// ofxFaceTrack2 example: SIMPLE EXAMPLE
//
// This example shows minimal usage of the ofxFaceTracker2 addon,
// doing simple landmark detection and pose estimation
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#include "ofxFaceTracker2.h"
#include "veronoiMesh/veronoiMesh.hpp"
#include "constants.hpp"

#include "ofxShaderGlitch.h"
#include "ofxPlasticTexture.hpp"


struct Emotion{
    // descriptor
    std::string descriptor;
    // index in the array
    int index;
    float intensityEmotion;
};



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    
    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;
    
    
    ofEasyCam cam;
    // timer face
    float timer;
    void updateMeshFromFace();
    ofMesh face;
    ofMesh lines;
    std::vector< ofVec3f > points;
    std::vector< ofVec3f > speeds;
    ofMesh pTest;
    
    void updatePointsPositions();
    
    veronoiMesh vMesh;
    
    Emotion userEmotion;
    
    void changeEmotion();

    std::vector < std::string > arraysEmotions;
    
    bool guiON;
    ofxPanel gui;
    ofParameter < int > emotionState;
    ofParameter < float > emotionIntensity;
    void emotionCallback(int& nEmotion);


    ofTrueTypeFont font;
    ofxOscSender maxSender, pythonSender, maxAutoRecord;
    ofxOscReceiver emotioReceiver;
    
    glm::vec2 resImg;
    
    void stateUpdate();
    void emotionReceivingOSC();
    
    // utils
    void logPrint(std::string logPrint){
#ifdef DEBUGMODE
        cout << logPrint << endl;
#endif
    }
    
    
    // postProcessing
//    ofxShaderGlitch glitch;
    
    ofxPlasticTexture p;
    ofTexture t;
    
    void drawCircles();
    
    bool hideMouse;
    
    bool initRad;
    std::vector < ofVec3f > radius;
    std::vector < ofVec2f > circles;
    std::vector < ofColor > circleColors;
    
    float vQuat;
    std::vector < glm::quat > quaternionCircles;
    
    void initRadius(int size);
    void radiusEvolution();
    
    std::vector < int > indicesToReSpawn;
    
    bool displayContent;
    void showHead();
    ofColor happyColor;
    
    void startRecording();
    bool redOnScreen;
};
