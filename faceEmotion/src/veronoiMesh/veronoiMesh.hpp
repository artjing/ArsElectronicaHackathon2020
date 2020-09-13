//
//  veronoiMesh.hpp
//  veronoi2D
//
//  Created by Pierre Tardif on 12/09/2020.
//

#ifndef veronoiMesh_hpp
#define veronoiMesh_hpp

#include <stdio.h>
#include "ofMain.h"

#include "ofxVoronoi2D.h"


#define CSIZE 1000

class veronoiMesh{
    public :
    veronoiMesh(){
        
    }
    
    void setup();
    void update();
    void draw();
    
    ofxVoronoi2D voronoi;
    
    
    std::vector<ofVec2f> pts;
    glm::vec2 imgRes;
    ofMesh mesh;
    unsigned int count;
    int dispMode;
};

#endif /* veronoiMesh_hpp */
