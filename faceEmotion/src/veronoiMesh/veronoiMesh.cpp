//
//  veronoiMesh.cpp
//  veronoi2D
//
//  Created by Pierre Tardif on 12/09/2020.
//

#include "veronoiMesh.hpp"


void veronoiMesh::setup(){

    count = 0;
    dispMode = 0;
    imgRes = glm::vec2(ofGetWidth(), ofGetHeight());
}


void veronoiMesh::update(){
    
    
    // update voronoi
    if(pts.size() != count){
        voronoi.compute(pts, ofRectangle(-imgRes.x / 2, -imgRes.y / 2, imgRes.x / 2, imgRes.y / 2), 0.0f);
        count = pts.size();
        // rebuild mesh
        mesh.clear();
        voronoi.buildMesh(mesh);

//        // random face colors
        unsigned int N = mesh.getIndices().size(), M = mesh.getVertices().size();
        vector<ofFloatColor> colors;
        colors.resize(M);
        for(unsigned int i = 0; i < N; i += 3){
            ofFloatColor c(
                ofRandom(1.0f), // R
                ofRandom(1.0f), // G
                ofRandom(1.0f)  // B
            );
            colors[mesh.getIndex(i + 0)] = c; // cell center
            colors[mesh.getIndex(i + 1)] = ofFloatColor(1.0f, 1.0f, 1.0f, 0.0f); // cell border
            colors[mesh.getIndex(i + 2)] = ofFloatColor(1.0f, 1.0f, 1.0f, 0.0f); // cell border
        }
        for(unsigned int i = 0; i < M; ++i) mesh.addColor(colors[i]);
    }
}

void veronoiMesh::draw(){
    if(count < 1) return;
//
//    // draw circles
//    for(unsigned int i = 0; i < pts.size(); ++i)
//      ofCircle(pts[i].x, pts[i].y, 10);
//
//    switch(dispMode){
//      case '1':
//        glPointSize(5.0f);
//        mesh.drawVertices();
        // mesh.draw(OF_MESH_POINTS);
//        break;
//      case '2':
        mesh.drawWireframe();
        // mesh.draw(OF_MESH_WIREFRAME);
//        break;
//      case '3':
        //mesh.draw(OF_MESH_FILL);
//        mesh.drawFaces();
//        break;
//      default:
//        // draw cells
//        ofxSegmentIterator it = voronoi.edges();
//        for(; it; ++it){
//          ofxSegment e = *it;
//          ofDrawLine(e.p1, e.p2);
//        }
//    }
}
