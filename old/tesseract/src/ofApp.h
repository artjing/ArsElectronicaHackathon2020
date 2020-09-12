#pragma once

#include "ofMain.h"

#define WIDTH 1080
#define HEIGHT 1080

#include "tesseractVbo/tesseractVbo.hpp"



class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
    
    ofEasyCam cam;
    ofVboMesh vbo;
    std::vector < float > depths;
    glm::vec2 sGrid;
    void initTerrain();
    
    glm::vec2 imgRes;
    void setVertexNoiseZ(ofVboMesh* vbo, int* index, float i, float j);
    
    tesseractVbo tesseractor;
};
