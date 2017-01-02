/* 
 * File:   edge.cpp
 * Author: Anjoom
 * 
 */

#include <iostream>
#include <stdlib.h>
#include <limits>
#include <float.h>
#include <cmath>
#include <string>
#include <vector>
#include <stack>

#include "vertex.h"
#include "edge.h"
#include "road.h"
#include "roadmap.h"

using namespace std;

edge::edge(){}

edge::edge(string nameInput){
    this->edgeName = nameInput;
}

edge::edge(vertex* vertex1, vertex* vertex2, int directional, double speed, double length) {
    this->edgeName                  = vertex1->GetVertexName() + "-" + vertex2->GetVertexName();
    this->edgePostedSpeedLimit      = speed;
    this->edgeEffectiveSpeedLimit   = speed;
    this->edgeLength                = length;
    this->edgeDirection             = directional;
    this->edgeEvent                 = 0;
    this->sourceVertex              = vertex1;
    this->destinationVertex         = vertex2;
        
    this->sourceVertex->addEdgeToAvailableEdgeList(this);
    
    if (speed != 0)
        this->timeToTravel = length / speed;
    else
        this->timeToTravel = DBL_MAX;
    
//    if (this->edgeDirection == 1){
//        this->sourceVertex->addEdgeToAvailableEdgeList(this);
//    }
//    else if (this->edgeDirection == 2){
//        this->sourceVertex->addEdgeToAvailableEdgeList(this);
//        this->destinationVertex->addEdgeToAvailableEdgeList(this);
//    }
}

edge::~edge() {
}


string  edge::GetEdgeName()                 const {return edgeName;}
double  edge::GetEdgeLength()               const {return edgeLength;}
double  edge::GetEdgePostedSpeedLimit()     const {return edgePostedSpeedLimit;}
double  edge::GetEdgeEffectiveSpeedLimit()  const {return edgeEffectiveSpeedLimit;}
int     edge::GetEdgeDirection()            const {return edgeDirection;}
int     edge::GetEdgeEvent()                const {return edgeEvent;}
vertex* edge::GetSourceVertex()             const {return sourceVertex;}
vertex* edge::GetDestinationVertex()        const {return destinationVertex;}
double  edge::GetTimeToTravel() const               {return timeToTravel;}

void    edge::SetTimeToTravel(double timeToTravel){
    this->timeToTravel = timeToTravel;
}

void edge::SetEdgeEffectiveSpeedLimit(double edgeEffectiveSpeedLimit) {
    this->edgeEffectiveSpeedLimit = edgeEffectiveSpeedLimit;
}

void edge::SetEdgeEvent(int edgeEvent) {
    this->edgeEvent = edgeEvent;
}

// 0 = EVENT_NORMAL, 
// 1 = EVENT_ACCIDENT_1_WAY, 
// 2 = EVENT_ACCIDENT_2_WAY, 
// 3 = EVENT_ROADBLOCK_1_WAY, 
// 4 = EVENT_ROADBLOCK_2_WAY, 
// 5 = EVENT_CONSTRUCTION_1_WAY, 
// 6 = EVENT_CONSTRUCTION_2_WAY,
