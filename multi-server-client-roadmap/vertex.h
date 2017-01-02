/* 
 * File:   vertex.h
 * Author: Anjoom
 * 
 */

#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string>
#include <vector>
#include <stack>

#include "edge.h"
#include "road.h"
#include "roadmap.h"

using namespace std;

class edge;

class vertex {
    private:
        int           vertexNumber; // to track the n'th entry of the vertex
        string        vertexName;   // name of the vertex
        int           vertexType;   // type of the vertex  
        vector<edge*> availableEdges;   // to keep all the edges that starts from this vertex
        
    public:
        vertex();
        vertex(string nameInput);
        vertex(int vertexTypeInput, string nameInput, int vertexNum);
        virtual ~vertex();
        
        int           GetVertexNumber() const;
        string        GetVertexName() const;
        int           GetVertexType() const;
        vector<edge*> GetAvailableEdges() const;
        
        bool          addEdgeToAvailableEdgeList(edge* edgeToAdd);
};


#endif /* VERTEX_H */
