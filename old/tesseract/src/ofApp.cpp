#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setDistance(400);
    
//    ofHideCursor();
    
    initTerrain();
    
    tesseractor.setup();
    
    imgRes = glm::vec2(ofGetWidth(), ofGetHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int index = 0;
    float freqNoise = .1;
    for( int i = -sGrid.x; i < sGrid.x; i++){
        for( int j = -sGrid.y; j < sGrid.y; j++){
            
            float speed = ofGetElapsedTimef() * .1;
            
            setVertexNoiseZ(&vbo, &index, float(i * freqNoise + speed), float(j * freqNoise + speed) );
            index++;
            setVertexNoiseZ(&vbo, &index, float((i+1) * freqNoise + speed), float(j * freqNoise + speed) );
            index++;
            setVertexNoiseZ(&vbo, &index, float((i+1) * freqNoise + speed), float((j+1) * freqNoise + speed) );
            index++;
            
            setVertexNoiseZ(&vbo, &index, float(i * freqNoise + speed), float(j * freqNoise + speed) );
            index++;
            setVertexNoiseZ(&vbo, &index, float(i * freqNoise + speed), float((j+1) * freqNoise + speed) );
            index++;
            setVertexNoiseZ(&vbo, &index, float((i+1) * freqNoise + speed), float((j+1) * freqNoise + speed) );
            index++;
            
        }
    }
    
    tesseractor.update(&vbo);
}
void ofApp::setVertexNoiseZ(ofVboMesh* vbo, int* index, float i, float j){

    ofVec3f p = vbo->getVertex(*index);
    p.z = 35 * ofNoise(i, j);
    vbo->setVertex(*index, p);
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(ofColor::black);
    
    cam.begin();
    
    ofRotateX( 90 );
    ofRotateZ( 30 + ofGetElapsedTimef() * 30);
    
    ofSetColor(ofColor::white);
    tesseractor.draw();
    
    cam.end();
    
    ofPushMatrix();
    ofTranslate(imgRes.x - 200, imgRes.y - 100);
    
    ofRotateX( 90 );
    ofRotateZ( 30 + ofGetElapsedTimef() * 30);
    ofSetColor(ofColor::white);
    vbo.drawWireframe();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}


void ofApp::initTerrain(){
    
    float scale = 20;
    vbo.setMode(OF_PRIMITIVE_LINES);
    
    sGrid = glm::vec2(8, 4);
    for(int i = -sGrid.x; i < sGrid.x; i++){
        for(int j = -sGrid.y; j < sGrid.y; j++){
            
            ofVec3f p = ofVec3f(i * scale, j * scale, 0);
            vbo.addVertex(p);
            p = ofVec3f((i+1) * scale, j * scale, 0);
            vbo.addVertex(p);
            p = ofVec3f((i+1) * scale, (j+1) * scale, 0);
            vbo.addVertex(p);
            
            p = ofVec3f(i * scale, j * scale, 0);
            vbo.addVertex(p);
            p = ofVec3f(i * scale, (j+1) * scale, 0);
            vbo.addVertex(p);
            p = ofVec3f((i+1) * scale, (j+1) * scale, 0);
            vbo.addVertex(p);
        }
    }
    
    depths.resize(vbo.getNumVertices());
}
