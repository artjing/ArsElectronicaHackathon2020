#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
#ifdef DEBUGMODE
    ofSetLogLevel(OF_LOG_VERBOSE);
#endif
    
        
    ofSetWindowTitle("Face Emotion");
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    resImg = glm::vec2(ofGetWidth(), ofGetHeight());
    
    pythonSender.setup(HOST, PYTHONPORT);
    maxSender.setup(HOST, MAXPORT);
    emotioReceiver.setup(EMOTIONPORT);
    
    emotionState = 0;
    
    /*
     
    happy
    angry
    disgusted
    fear
    surprise
    neutra
    sad
     
    */
    arraysEmotions.push_back("happy");
    arraysEmotions.push_back("angry");
    arraysEmotions.push_back("disgusted");
    arraysEmotions.push_back("fear");
    arraysEmotions.push_back("surprise");
    arraysEmotions.push_back("neutra");
    arraysEmotions.push_back("sad");
    
    
    displayContent = true;
    hideMouse = false;
    redOnScreen = false;
    
    guiON = false;
    gui.setup("Control Panel");
    gui.add(emotionState.set("emotionState", 0, 0, arraysEmotions.size() - 1));
    gui.add(emotionIntensity.set("emotion Intensity", 0, 0, 1));
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
    

    circles.clear();
    
    
    vMesh.setup();
    
//    glitch.setup();
    
    p.setup();
    
    happyColor = ofColor(0, 255, 10);
    vQuat = 400;
    
    post.setup(resImg.x, resImg.y);
    
    recordMax = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    post.update();
    
    
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
    post.begin();
    ofEnableDepthTest();
    cam.begin();
    if(displayContent)showHead();
    cam.end();
    ofDisableDepthTest();
    post.end();
    
    
    if(guiON)gui.draw();
    
    if(redOnScreen){
        cout << "redOnScreen" << endl;
        redOnScreen = false;
        ofPushMatrix();
        ofSetColor(255, 0, 0);
        ofDrawRectangle(0, 0, resImg.x, resImg.y);
        ofPopMatrix();
    }
}


void ofApp::updateMeshFromFace(){
    face = tracker.getMesh();
    int n = face.getNumVertices();
    
    if(n > 0){
        switch(userEmotion.index){
                
           
                
             case 0 :
             // HAPPY
//                if(circles.size()<1){
//                    circles.clear();
//                    for( int i = 0; i < n; i+=3){
//                        circles.push_back(ofVec2f(face.getVertices()[i].x, face.getVertices()[i].y));
//                    }
//                }
                
                if(circles.size() < 1000){
                    
                    for( int i = 0; i < n; i += 3){
                        if(ofRandom(1)> .3){
                            circles.push_back(ofVec2f(face.getVertices()[i].x, face.getVertices()[i].y));
                            radius.push_back(ofVec3f(ofRandom(MAXRAD), pow(-1, int(ofRandom(2))) * .2, 0));
                            ofColor happyColorVariations = ofColor(happyColor.r + ofRandom(COLVAR),happyColor.g + ofRandom(COLVAR) - COLVAR / 2,happyColor.b + ofRandom(COLVAR));
                            circleColors.push_back(happyColorVariations);

                            glm::quat q = glm::vec3(ofRandom(vQuat) - vQuat / 2, ofRandom(vQuat) - vQuat / 2, ofRandom(vQuat) - vQuat / 2);
                            quaternionCircles.push_back(q);
                        }
                        
                    }
                }else{
                    for( int i = 0; i < indicesToReSpawn.size(); i++){
                        
                        circles[indicesToReSpawn[i]]  = (ofVec2f(face.getVertices()[i].x, face.getVertices()[i].y));
                        
                    }
                    indicesToReSpawn.clear();
                }
//                vMesh.pts.clear();
//                for( int i = 0; i < n; i+=3){
//                //                for( int i = 0; i < points.size(); i+=1){
//                    vMesh.pts.push_back(ofVec2f(face.getVertices()[i].x, face.getVertices()[i].y));
//                //                    vMesh.pts.push_back(ofVec2f(points[i].x, points[i].y));
//
//                }
//                vMesh.update();
                
                break;
                
            case 1 :
                // ANGER
                lines.clear();
                lines.setMode(OF_PRIMITIVE_LINES);
                for( int i = 0;i < points.size(); i++){
                    lines.addVertex(points[i]);
                    lines.addVertex(face.getVertices()[i % n]);
                }
                break;
                
            default :
               // DSIGUSTED
                   face.clearColors();
                   break;
           
        }
    }

    
}

void ofApp::updatePointsPositions(){
    
    bool updateLines = false;
    if(lines.getNumVertices() > 0)updateLines = true;
    for( int i = 0; i < points.size(); i++){
        
        points[i].x += 10 * emotionIntensity * speeds[i].x * ofNoise(ofGetElapsedTimef()* 6 + i * 20);
        points[i].y += 10 * emotionIntensity * speeds[i].y * ofNoise(ofGetElapsedTimef()* 6 + i * 20);
        points[i].z += 10 * emotionIntensity * speeds[i].z * ofNoise(ofGetElapsedTimef()* 6 + i * 20);
        
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
    
    if(key == 'g')guiON = !guiON;
    if(key == OF_KEY_LEFT || key == OF_KEY_RIGHT){
        displayContent = !displayContent;
    }
    
    if(key == 'h'){
        hideMouse = !hideMouse;
        if(hideMouse){
            ofHideCursor();
        }else{
            ofShowCursor();
        }
    }
    
    if(key == 'r')startRecording();
    
    if(key == '0')emotionState = 0;
    if(key == '1')emotionState = 1;
    if(key == '2')emotionState = 2;
    if(key == '3')emotionState = 3;
        
}

void ofApp::changeEmotion(){
    userEmotion.index += 1;
    if(userEmotion.index > arraysEmotions.size()-1)userEmotion.index = 0;
    userEmotion.descriptor = arraysEmotions[userEmotion.index];
    
    cout << "userEmotion = " << userEmotion.descriptor << endl;
}


void ofApp::emotionReceivingOSC(){
    while(emotioReceiver.hasWaitingMessages()){
        
        cout << "got a message " << endl;
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
            
            if(m.getArgAsString(0) == "happy")emotionState = 0;
            if(m.getArgAsString(0) == "angry")emotionState = 1;
            if(m.getArgAsString(0) == "disgusted")emotionState = 2;
            if(m.getArgAsString(0) == "fear")emotionState = 3;
            if(m.getArgAsString(0) == "surprise")emotionState = 4;
            if(m.getArgAsString(0) == "neutra")emotionState = 5;
            if(m.getArgAsString(0) == "sad")emotionState = 5;
            cout << "m.getArgAsFloat(1) = " << ofToString(m.getArgAsFloat(1) ) << endl;
            emotionIntensity = m.getArgAsFloat(1);
            
            
        }
    }
}

void ofApp::emotionCallback(int& nEmotion){

    userEmotion.index = nEmotion;
    userEmotion.descriptor = arraysEmotions[emotionState];
    userEmotion.intensityEmotion = emotionIntensity;
    
    ofxOscMessage mPython, mMax;
    mPython.setAddress("/emotion");
    mPython.addStringArg(userEmotion.descriptor);
    mPython.setAddress("/intensity");
    mPython.addFloatArg(userEmotion.intensityEmotion);
    
    mMax.setAddress("/emotion");
    mMax.addStringArg(userEmotion.descriptor);
    maxSender.sendMessage(mMax, false);
    mMax.setAddress("/intensity");
    mMax.addFloatArg(userEmotion.intensityEmotion);
    maxSender.sendMessage(mMax, false);
    
    pythonSender.sendMessage(mPython, false);
    
    logPrint("sending new emotionState : " + ofToString(userEmotion.descriptor));
    
    
    
    if(userEmotion.index != 0)initRad = false;
    
    

    std::string effectName = "./Effects/emotion_" + ofToString(emotionState) + ".xml";
    cout << effectName << endl;
    post.guiPanel.gui.loadFromFile(effectName);
}


void ofApp::drawCircles(){
    
    int s = circles.size();
    if(!initRad)initRadius(s);
    
    int r = radius.size();
    
    
    for(int i = 0; i < s; i++){
        
        auto axis = glm::axis(quaternionCircles[i%r]);
                                                  
        ofVec3f rad = radius[i%r];

        ofPushMatrix();
        ofNoFill();
        ofRotateDeg(10, axis.x, axis.y, axis.z);
        ofTranslate(0, 0, rad.z);
        
        ofPushStyle();
        ofSetColor(circleColors[i%r]);
        ofDrawCircle(circles[i].x, circles[i].y, rad.x);
        ofPopStyle();
        ofPopMatrix();
    }
    
    radiusEvolution();
    
    
    
}


void ofApp::initRadius(int size){
    initRad = true;
    radius.clear();
    
    for(int i = 0; i < size; i++){
        radius.push_back(ofVec3f(ofRandom(MAXRAD), pow(-1, int(ofRandom(2))) * .2, 0));
        ofColor happyColorVariations = ofColor(happyColor.r + ofRandom(COLVAR),happyColor.g + ofRandom(COLVAR) - COLVAR / 2,happyColor.b + ofRandom(COLVAR));
        circleColors.push_back(happyColorVariations);
        glm::quat q = glm::vec3(ofRandom(vQuat) - vQuat / 2, ofRandom(vQuat) - vQuat / 2, ofRandom(vQuat) - vQuat / 2);
        quaternionCircles.push_back(q);
    }
}

void ofApp::radiusEvolution(){
    
    
    indicesToReSpawn.clear();
    for( int i = 0; i < radius.size(); i++){
        radius[i].x += emotionIntensity;
        radius[i].z += emotionIntensity * radius[i].y;
        
        if(radius[i].x > MAXRAD){
//            cout << "need respawning for index = " << ofToString(i) << endl;
            indicesToReSpawn.push_back(i);
            radius[i].x = 0;
            radius[i].z = 0;
            radius[i].y = pow(-1, int(ofRandom(2))) * ofRandom(MAXMOVECIRCLE);

            
            glm::quat q = glm::vec3(ofRandom(vQuat) - vQuat / 2, ofRandom(vQuat) - vQuat / 2, ofRandom(vQuat) - vQuat / 2);
            quaternionCircles[i] = q;
        }
    }
}



void ofApp::showHead(){
    switch(userEmotion.index){
                     

                 
        
            case 0 :
            {
                // HAPPY
                drawCircles();
                
                ofSetColor(255, 255);
                face.drawWireframe();
                break;
            }
        
         
         case 1 :
         {
             // ANGER
             ofSetColor(255, 255);
             face.drawWireframe();
             ofSetColor(255, 255);
             lines.draw();
             ofSetColor(255, 0, 0, 255);
             pTest.draw();
             break;
         }


         
        default :
        {
            // DSIGUST
            ofSetColor(255, 255);

            p.begin();
            face.draw();
            p.end();


            face.drawWireframe();
            break;
        }
                 
     }
}


void ofApp::startRecording(){
    
    redOnScreen = true;
    recordMax = !recordMax;
    
    
    ofxOscMessage mSend;
    
    mSend.setAddress("/autoRecord");
    mSend.addBoolArg(recordMax);
    
    maxSender.sendMessage(mSend, false);
}
