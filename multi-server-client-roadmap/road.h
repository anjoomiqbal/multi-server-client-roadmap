/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   road.h
 * Author: Anjoom
 *
 * Created on January 2, 2017, 7:37 AM
 */

#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cstddef>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string>
#include <vector>
#include <stack>

#include "vertex.h"
#include "edge.h"
#include "roadmap.h"

using namespace std;

class vertex;
class edge;
class roadmap;

class road {
    private:
        string          roadName;       // to store the name of the road
        double          timeNeededToTravelRoad; // keeps track of the effective weight
        vector<vertex*> vertexOnRoad;   // to store all the vertices on the road
    public:
        road();
        road(const road& orig);
        virtual ~road();

    string          GetRoadName() const;
    double          GetTimeNeededToTravelRoad() const;
    vector<vertex*> GetVertexOnRoad() const;
    
    void            SetRoadName(string roadName);
    void            SetTimeNeededToTravelRoad(double timeNeededToTravelRoad);
   
    bool            addVertexOnRoad(vertex* vertexToAdd);



};

#endif /* ROAD_H */

