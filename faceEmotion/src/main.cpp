#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    

    ofGLWindowSettings settings;
//    settings.setSize(1080, 1350);
    settings.setSize(WIDTH, HEIGHT);
    settings.setGLVersion(VERSION,SUB);
    ofCreateWindow(settings);
    
    
	ofRunApp(new ofApp());

}
