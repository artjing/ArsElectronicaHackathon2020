// ofxFaceTrack2 example: SIMPLE EXAMPLE
//
// This example shows minimal usage of the ofxFaceTracker2 addon,
// doing simple landmark detection and pose estimation
//

#pragma once

#include "ofMain.h"
#include "ofxFaceTracker2.h"
#include "veronoiMesh/veronoiMesh.hpp"

#define CUBESIZE 1000
#define MAXSPEED 5

struct Emotion{
    // descriptor
    std::string descriptor;
    // index in the array
    int index;
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
    
    // emotions
    float angerIntensity;
    
    veronoiMesh vMesh;
    
    Emotion userEmotion;
    void changeEmotion();

    std::vector < std::string > arraysEmotions;
};
