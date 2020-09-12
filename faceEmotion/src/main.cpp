#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    

    ofGLWindowSettings settings;
//    settings.setSize(1080, 1350);
    settings.setSize(WIDTH, HEIGHT);
    settings.setGLVersion(3,2);
    ofCreateWindow(settings);
    
    
	ofRunApp(new ofApp());

}
