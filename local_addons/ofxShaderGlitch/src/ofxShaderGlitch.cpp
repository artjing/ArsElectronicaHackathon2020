//
//  ofxShaderGlitch.cpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//

#include "ofxShaderGlitch.h"


void ofxShaderGlitch::setup(){
    
    ofAddListener(ofEvents().keyPressed, this, &ofxShaderGlitch::keyPressed);
}

//--------------------------------------------------------------
void ofxShaderGlitch::begin(){
    glitch.begin();
}

//--------------------------------------------------------------
void ofxShaderGlitch::end(){
    glitch.end();
    drawGUI();
}

//--------------------------------------------------------------
void ofxShaderGlitch::drawGUI(){
    glitch.drawGUI();
}

//--------------------------------------------------------------
void ofxShaderGlitch::keyPressed(ofKeyEventArgs &keyArgs){
    if(keyArgs.key == 'g')glitch.guiON = !glitch.guiON;
    if(keyArgs.key == 'c'){
        glitch.nonMaxAndContinuity = !glitch.nonMaxAndContinuity;
//        cout << " maxContinuity = " << ofToString(glitch.nonMaxAndContinuity) << endl;
    }
    
    if(keyArgs.key == '0'){
        cout << "doing config 0" << endl;
        glitch.gui.loadFromFile("settings_demo/0.xml");
    }
    
    if(keyArgs.key == '1')glitch.gui.loadFromFile("settings_demo/1.xml");
    if(keyArgs.key == '2')glitch.gui.loadFromFile("settings_demo/2.xml");
    if(keyArgs.key == '3')glitch.gui.loadFromFile("settings_demo/3.xml");
    if(keyArgs.key == '4')glitch.gui.loadFromFile("settings_demo/4.xml");
    if(keyArgs.key == '5')glitch.gui.loadFromFile("settings_demo/5.xml");
    if(keyArgs.key == '6')glitch.gui.loadFromFile("settings_demo/6.xml");
    if(keyArgs.key == '7')glitch.gui.loadFromFile("settings_demo/7.xml");
    if(keyArgs.key == '8')glitch.gui.loadFromFile("settings_demo/8.xml");
    if(keyArgs.key == '9')glitch.gui.loadFromFile("settings_demo/9.xml");
    
}
