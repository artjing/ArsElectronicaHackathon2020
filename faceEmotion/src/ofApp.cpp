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
    
//    glitch.setup();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    emotionReceivingOSC();
    
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
    
    
//    glitch.begin();
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
//        face.drawWireframe();
        vMesh.draw();
        break;
    }
            
    
//
    cam.end();
    ofDisableDepthTest();
//    glitch.end();
    
    
    if(guiON)gui.draw();
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
                vMesh.pts.clear();
                for( int i = 0; i < n; i+=3){
//                for( int i = 0; i < points.size(); i+=1){
                    vMesh.pts.push_back(ofVec2f(face.getVertices()[i].x, face.getVertices()[i].y));
//                    vMesh.pts.push_back(ofVec2f(points[i].x, points[i].y));
                    
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
    
    if(key == 'g')guiON = !guiON;
        
}

void ofApp::changeEmotion(){
    userEmotion.index += 1;
    if(userEmotion.index > arraysEmotions.size()-1)userEmotion.index = 0;
    userEmotion.descriptor = arraysEmotions[userEmotion.index];
    
    cout << "userEmotion = " << userEmotion.descriptor << endl;
}


void ofApp::emotionReceivingOSC(){
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
