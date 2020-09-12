#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    userEmotion.descriptor = "Anger";
    userEmotion.index = 0;
    
    
    grabber.setup(1280,720);
    
    // Setup tracker
    tracker.setup();
    
    // timer face update
    timer = 0;
    updateMeshFromFace();
    
    cam.setDistance(400);
    

    
    int amnt = 100;
    pTest.setMode(OF_PRIMITIVE_POINTS);
    for( int i = 0; i < amnt; i++){
        points.push_back(ofVec3f(ofRandom(CUBESIZE) - CUBESIZE / 2, ofRandom(CUBESIZE) - CUBESIZE / 2, ofRandom(CUBESIZE) - CUBESIZE / 2));
        speeds.push_back(ofVec3f(ofRandom(MAXSPEED) - MAXSPEED / 2, ofRandom(MAXSPEED) - MAXSPEED / 2, ofRandom(MAXSPEED) - MAXSPEED / 2));
        
        pTest.addVertex(points[i]);
    }
    
    angerIntensity  = 10;
    
    
    vMesh.setup();
    
    
    arraysEmotions.push_back("Angry");
    arraysEmotions.push_back("Happy");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
    // Update tracker when there are new frames
    if(grabber.isFrameNew()){
        tracker.update(grabber);
    }
    
    timer += 0.2;
    if(timer > 1){
        timer = 0;
        updateMeshFromFace();
    }
    
    
    updatePointsPositions();
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0,255);
    // Draw camera image
//    grabber.draw(0, 0);
    
    // Draw tracker landmarks
//    tracker.drawDebug();
    ofEnableDepthTest();
    cam.begin();
    
    
    
    switch(userEmotion.index){
    
    case 0 :
        // ANGER
        ofSetColor(255, 255);
        face.drawWireframe();
        ofSetColor(255, 255);
        lines.draw();
        ofSetColor(255, 0, 0, 255);
        pTest.draw();
        break;
            
            
    case 1 :
        // HAPPY
        ofSetColor(255, 255);
        face.drawWireframe();
        vMesh.draw();
        break;
    }
            
    
//
    cam.end();
    ofDisableDepthTest();
    
    // Draw estimated 3d pose
//    tracker.drawDebugPose();
    
    // Draw text UI
    ofDrawBitmapStringHighlight("Framerate : "+ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapStringHighlight("Tracker thread framerate : "+ofToString(tracker.getThreadFps()), 10, 40);
    
#ifndef __OPTIMIZE__
    ofSetColor(ofColor::red);
    ofDrawBitmapString("Warning! Run this app in release mode to get proper performance!",10,60);
    ofSetColor(ofColor::white);
#endif
}


void ofApp::updateMeshFromFace(){
    face = tracker.getMesh();
    int n = face.getNumVertices();
    
    if(n > 0){
        switch(userEmotion.index){
            
            case 0 :
                // ANGER
                lines.clear();
                lines.setMode(OF_PRIMITIVE_LINES);
                for( int i = 0;i < points.size(); i++){
                    lines.addVertex(points[i]);
                    lines.addVertex(face.getVertices()[i % n]);
                }
                break;
            
            case 1 :
                // HAPPY
                std::vector < ofVec2f > pointVeronoi;
                vMesh.pts.clear();
                for( int i = 0; i < n; i+=3){
                    vMesh.pts.push_back(ofVec2f(face.getVertices()[i].x, face.getVertices()[i].y));
                }
                vMesh.update();
                break;
        }
    }

    
}

void ofApp::updatePointsPositions(){
    
    bool updateLines = false;
    if(lines.getNumVertices() > 0)updateLines = true;
    for( int i = 0; i < points.size(); i++){
        
        points[i].x += angerIntensity * speeds[i].x * ofNoise(ofGetElapsedTimef()* 6 + i * 20);
        points[i].y += angerIntensity * speeds[i].y * ofNoise(ofGetElapsedTimef()* 6 + i * 20);
        points[i].z += angerIntensity * speeds[i].z * ofNoise(ofGetElapsedTimef()* 12 + i * 20);
        
        if(points[i].x > CUBESIZE / 2)points[i].x -= CUBESIZE;
        if(points[i].y > CUBESIZE / 2)points[i].y -= CUBESIZE;
        if(points[i].z > CUBESIZE / 2)points[i].z -= CUBESIZE;
        
        if(points[i].x < -CUBESIZE / 2)points[i].x += CUBESIZE;
        if(points[i].y < -CUBESIZE / 2)points[i].y += CUBESIZE;
        if(points[i].z < -CUBESIZE / 2)points[i].z += CUBESIZE;
        
        
        pTest.setVertex(i, points[i]);
        if(updateLines)lines.setVertex(i*2, points[i]);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == '+'){
        angerIntensity +=1;
        if(angerIntensity > 20) angerIntensity = 1;
        cout << "anger increased : " << ofToString(angerIntensity) << endl;
    }
    
    if(key == ' '){
        changeEmotion();
    }
        
}

void ofApp::changeEmotion(){
    userEmotion.index += 1;
    if(userEmotion.index > arraysEmotions.size())userEmotion.index = 0;
    userEmotion.descriptor = arraysEmotions[userEmotion.index];
    
    cout << "userEmotion = " << userEmotion.descriptor << endl;
}
