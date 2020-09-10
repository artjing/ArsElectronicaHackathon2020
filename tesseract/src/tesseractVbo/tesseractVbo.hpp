//
//  tesseractVbo.hpp
//  tesseract
//
//  Created by Pierre Tardif on 10/09/2020.
//

#ifndef tesseractVbo_hpp
#define tesseractVbo_hpp

#include <stdio.h>
#include "ofMain.h"

class tesseractVbo{
    public :
    tesseractVbo(){
        
    }
    
    void setup();
    void update(ofVboMesh* vbo);
    void draw();
    
    void processTesseract(ofVboMesh* vbos);
    glm::vec4 TesseractPoint(glm::vec3 pOriginal, float depth, float fourthD);
    void TesseractInit(glm::vec4* projected, float size, ofVboMesh* tesseract);
    void TesseractUpdate(glm::vec4* projected, float size, ofVboMesh* tesseract);
    void addVertexToVbo(int offset, int i, int j, int k, ofVboMesh* v, glm::vec4* p);
    void setVertexToVbo(int offset, int i, int j, ofVboMesh* v, glm::vec4* p, int index);
    void selectMat(std::string name, glm::mat4x4* m);
    
    glm::mat4x4 mat;
    ofVboMesh vboT;
    
    float distance;
    float rescaleAmp;
    
    float angleRot;
    bool initTesseract;
    
};

#endif /* tesseractVbo_hpp */
