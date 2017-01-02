/* 
 * File:   vertex.cpp
 * Author: Anjoom
 * 
 */

#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string>
#include <vector>
#include <stack>

#include "vertex.h"
#include "edge.h"
#include "road.h"
#include "roadmap.h"

using namespace std;

vertex::vertex(){}

// for null objects
vertex::vertex(string nameInput){
    this->vertexName = nameInput;
}

vertex::vertex(int vertexTypeInput, string nameInput, int vertexNum){
    this->vertexNumber   = vertexNum;
    this->vertexName     = nameInput;
    this->vertexType     = vertexTypeInput;
//    this->availableEdges = NULL;
//    totalVertexCount++;
}
        
vertex::~vertex(){
    this->availableEdges.clear();
//    totalVertexCount--;
}

int           vertex::GetVertexNumber()   const {return vertexNumber;}
string        vertex::GetVertexName()     const {return vertexName;}
int           vertex::GetVertexType()     const {return vertexType;}
vector<edge*> vertex::GetAvailableEdges() const {return availableEdges;}

bool vertex::addEdgeToAvailableEdgeList(edge* edgeToAdd) {
    this->availableEdges.push_back(edgeToAdd);
}




//bool isEqualIqgnoreCase(const string& a, const string& b){
//    unsigned int sz = a.size();
//    if (b.size() != sz)
//        return false;
//    for (unsigned int i = 0; i < sz; ++i)
//        if (tolower(a[i]) != tolower(b[i]))
//            return false;
//    return true;
//}