/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   edge.h
 * Author: Anjoom
 *
 * Created on December 30, 2016, 9:54 AM
 */

#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string>
#include <vector>
#include <stack>

#include "vertex.h"
#include "road.h"
#include "roadmap.h"

using namespace std;

class vertex;

class edge {
    private:
        string  edgeName;       // name of the edge
        double  edgeLength;     // length of the edge
        double  edgePostedSpeedLimit;    // originally posted speed limit in the edge 
        double  edgeEffectiveSpeedLimit; // to keep track of speed limit after edge events
        double  timeToTravel;   // to keep track of time needed to tavel this edge
        int     edgeDirection;  // 1 = one way, 
                                // 2 = two way, 
                                // other val = invalid
        
        int     edgeEvent;      // 0 = EVENT_NORMAL, 
                                // 1 = EVENT_ACCIDENT, 
                                // 2 = EVENT_ROADBLOCK, 
                                // 3 = EVENT_CONSTRUCTION, 
                                // other val = invalid
        vertex* sourceVertex;       // the pointed of the vertex from where the edge started
        vertex* destinationVertex;  // the pointed of the vertex to where the edge ended
        
        
    public:
        edge();
        edge(string nameInput);
        edge(vertex* vertex1, vertex* vertex2, int directional, double speed, double length);
        virtual ~edge();
        
        string  GetEdgeName() const;
        double  GetEdgeLength() const;
        double  GetEdgePostedSpeedLimit() const;
        double  GetEdgeEffectiveSpeedLimit() const;
        int     GetEdgeDirection() const;
        int     GetEdgeEvent() const;
        vertex* GetSourceVertex() const;
        vertex* GetDestinationVertex() const;
        double  GetTimeToTravel() const;
        void    SetTimeToTravel(double timeToTravel);
    
        void SetEdgeEffectiveSpeedLimit(double edgeEffectiveSpeedLimit);
        void SetEdgeEvent(int edgeEvent);



    

        
};

#endif /* EDGE_H */

