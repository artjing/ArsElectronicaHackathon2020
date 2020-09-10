//
//  tesseractVbo.cpp
//  tesseract
//
//  Created by Pierre Tardif on 10/09/2020.
//

#include "tesseractVbo.hpp"

// tesseract
void tesseractVbo::setup(){
    rescaleAmp = 100;
    angleRot = 0;
    initTesseract = false;
}

void tesseractVbo::update(ofVboMesh* vbo){
    angleRot += .01;
    processTesseract(vbo);
}

void tesseractVbo::draw(){
    vboT.drawWireframe();
}
//--------------------------------------------------------------
void tesseractVbo::processTesseract(ofVboMesh* vbos){
    std::vector < glm::vec4 > projected;
    float depth = 20;
    float fourthD = 20;
    
    for(int i = 0; i < vbos->getNumVertices(); i+=6){
        projected.push_back(TesseractPoint(vbos->getVertex(i), depth, fourthD));
    }
    for(int i = 0; i < vbos->getNumVertices(); i+=6){
        projected.push_back(TesseractPoint(vbos->getVertex(i), -depth, -fourthD));
    }
    
    if(!initTesseract){
        TesseractInit(&projected[0], projected.size(), &vboT);
    }else{
        TesseractUpdate(&projected[0], projected.size(), &vboT);
    }
}

glm::vec4 tesseractVbo::TesseractPoint(glm::vec3 pOriginal, float depth, float fourthD){
    
    glm::vec4 p = glm::vec4(pOriginal, fourthD);
    p.z += depth;

    distance = 40;
    selectMat("ZW", &mat);
    glm::vec4 rotated =  mat * p;
//    selectMat("ZW", &mat);
//    rotated = mat * rotated;

    float w = 1 / ( distance - rotated.w );

    glm::mat4x4 proj = glm::mat4x4(  w, 0, 0, 0,
                                     0, w, 0, 0,
                                     0, 0, w, 0,
                                     0, 0, 0, 0);

    return proj * rotated;
}

void tesseractVbo::TesseractInit(glm::vec4* projected, float size, ofVboMesh* tesseract){
    
    initTesseract = true;
    tesseract->clear();
    tesseract->setMode(OF_PRIMITIVE_LINES);
    int s_4 = int(size / 4.0);
    int s_2 = int(size / 2.0);
    for( int i = 0; i < s_2; i++){
        
        addVertexToVbo(0, i,  (i+1) % s_2 , tesseract, projected);
        addVertexToVbo(0, i + s_2, (i + 1) % s_2 + s_2, tesseract, projected);
        addVertexToVbo(0, i, i + s_2, tesseract, projected);
    }

//    for( int i = 0; i < s_2; i++){
//
//       addVertexToVbo(s_2, i,  (i+1) % s_4, tesseract, projected);
//       addVertexToVbo(s_2, i + s_4, (i + 1) % s_4 + s_4, tesseract, projected);
//       addVertexToVbo(s_2, i, i + s_4, tesseract, projected);
//    }

//    for( int i = 0; i < s_2; i++){
//
//       addVertexToVbo(0, i,  i + s_2 , tesseract, projected);
//    }
    
}

void tesseractVbo::addVertexToVbo(int offset, int i, int j, ofVboMesh* v, glm::vec4* p){
        
    glm::vec4 a = *(p + i + offset);
    glm::vec4 b = *(p + j + offset);
    
    v->addVertex(ofVec3f(a.x * rescaleAmp, a.y * rescaleAmp, a.z * rescaleAmp));
    v->addVertex(ofVec3f(b.x * rescaleAmp, b.y * rescaleAmp, b.z * rescaleAmp));
    
}



void tesseractVbo::TesseractUpdate(glm::vec4* projected, float size, ofVboMesh* tesseract){
//
//    int s_4 = int(size / 4.0);
//    int s_2 = int(size / 2.0);
//    for( int i = 0; i < s_2; i++){
//
//        addVertexToVbo(0, i,  (i+1) % s_2 , tesseract, projected);
//        addVertexToVbo(0, i + s_2, (i + 1) % s_2 + s_2, tesseract, projected);
//        addVertexToVbo(0, i, i + s_2, tesseract, projected);
//    }
//
    
    int s_4 = int(size / 4.0);
    int s_2 = int(size / 2.0);
    int index = 0;
    for( int i = 0; i < s_2; i++){
        
        setVertexToVbo(0, i,  (i+1) % s_2 , tesseract, projected, index);
        setVertexToVbo(0, i + s_2, (i + 1) % s_2 + s_2, tesseract, projected, index + 2);
        setVertexToVbo(0, i, i + s_2, tesseract, projected, index + 4);
        
        index += 6;
    }

//    for( int i = 0; i < s_4; i++){
//
//       setVertexToVbo(s_2, i,  (i+1) % s_4, tesseract, projected, index);
//       setVertexToVbo(s_2, i + s_4, (i + 1) % s_4 + s_4, tesseract, projected, index + 1);
//       setVertexToVbo(s_2, i, i + s_4, tesseract, projected, index + 2);
//
//        index += 3;
//    }
//
//    for( int i = 0; i < s_2; i++){
//
//        setVertexToVbo(0, i,  i + s_2 , tesseract, projected, index);
//        index++;
//    }
    
}

void tesseractVbo::setVertexToVbo(int offset, int i, int j, ofVboMesh* v, glm::vec4* p, int index){
        
    glm::vec4 a = *(p + i + offset);
    glm::vec4 b = *(p + j + offset);
    
    v->setVertex(index, ofVec3f(a.x * rescaleAmp, a.y * rescaleAmp, a.z * rescaleAmp));
    v->setVertex(index+1, ofVec3f(b.x * rescaleAmp, b.y * rescaleAmp, b.z * rescaleAmp));
    
}

void tesseractVbo::selectMat(std::string name, glm::mat4x4* m){
    float c = cos(angleRot);
    float s = sin(angleRot);
    if( name == "XY" ) *m = glm::mat4x4( c, -s, 0, 0,
                                    s, c, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1);
    if( name == "XZ" ) *m = glm::mat4x4( c, 0, -s, 0,
                                    0, 1, 0, 0,
                                    s, 0, c, 0,
                                    0, 0, 0, 1);
    if( name == "XW" ) *m = glm::mat4x4( c, 0, 0, -s,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    s, 0, 0, c);
        
     if( name == "YZ") *m = glm::mat4x4( 1, 0, 0, 0,
                                    0, c, -s, 0,
                                    0, s, c, 0,
                                    0, 0, 0, 1);
    if( name == "YW") *m = glm::mat4x4(  1, 0, 0, 0,
                                    0, c, 0, -s,
                                    0, 0, 1, 0,
                                    0, s, 0, c);
    if( name == "ZW") *m = glm::mat4x4(  1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, c, -s,
                                    0, 0, s, c);

}
