/* 
 * File:   road.cpp
 * Author: Anjoom
 * 
 */


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
#include "road.h"
#include "roadmap.h"

using namespace std;

class vertex;
class edge;
class roadmap;


road::road() {
}

//road::road(const road& orig) {
//}

road::~road() {
    vertexOnRoad.clear();
}

// Get methods
string          road::GetRoadName() const {return roadName;}
double          road::GetTimeNeededToTravelRoad() const {return timeNeededToTravelRoad;}
vector<vertex*> road::GetVertexOnRoad() const {return vertexOnRoad;}

// Set Methods
void road::SetRoadName(string roadName) {this->roadName = roadName;}
void road::SetTimeNeededToTravelRoad(double timeNeededToTravelRoad) {this->timeNeededToTravelRoad = timeNeededToTravelRoad;}


bool road::addVertexOnRoad(vertex* vertexToAdd) {
    this->vertexOnRoad.push_back(vertexToAdd);
}