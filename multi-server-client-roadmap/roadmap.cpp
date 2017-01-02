/* 
 * File:   roadmap.cpp
 * Author: Anjoom
 * 
 */

#include <iostream>
#include <cstddef>
#include <stdlib.h>
#include <limits>
#include <float.h>
#include <string>
#include <vector>
#include <stack>

#include "vertex.h"
#include "edge.h"
#include "road.h"
#include "roadmap.h"

using namespace std;

roadmap::roadmap() {
    this->totalVertexCount = 0;
    
    class vertex v("VERTEX NOT FOUND");
    this->nullVertex.push_back(v);
    
    class edge e("EDGE NOT FOUND");
    this->nullEdge.push_back(e);
}


roadmap::~roadmap() {
    this->allVertexInRoadmap.clear(); 
    this->allPOIInRoadmap.clear(); 
    this->allEdgeInRoadmap.clear();
    this->allRoadsInRoadmap.clear();
    this->nullEdge.clear();
    this->nullVertex.clear();
//    this->allKnownPaths.clear();   
}

vector<vertex> roadmap::getAllVertexInRoadmap() const  {return allVertexInRoadmap;}
vector<edge>   roadmap::getAllEdgeInRoadmap() const    {return allEdgeInRoadmap;}
int            roadmap::getTotalVertexCount() const    {return totalVertexCount;}
int            roadmap::getTotalEdgeCount() const      {return totalEdgeCount;}
vector<vertex> roadmap::getNullVertex() const          {return nullVertex;}
vector<edge>   roadmap::getNullEdge() const            {return nullEdge;}
vector<road>   roadmap::getAllRoadsInRoadmap() const   {return allRoadsInRoadmap;}


bool roadmap::addVertex(string vertexType, string label){
    string vName = toUpper(label);
    for (int i = 0; i < (int)allVertexInRoadmap.size(); i++) {
        if ( allVertexInRoadmap[i].GetVertexName().compare(vName) == 0){
            cerr << "Failed: addVertex: Vertex already exists at i = " << allVertexInRoadmap[i].GetVertexNumber()  << endl;
            return false;
        }
    }
    
    int vType = this->vertexTypeStringToInt(vertexType);
    if (vType == -1){
        cerr << "Failed: addVertex: VertexType should be POI or INTERSECTION" << endl;
        return false;
    }
    
//    vertex v = vertex(vType, label, this->totalVertexCount);
//    vertex;
    class vertex v(vType, vName, ++totalVertexCount);
    allVertexInRoadmap.push_back(v);
    
    // for keeping track of only the POI and not the intersections
    if (vType == 1)
        allPOIInRoadmap.push_back(v);
    cerr << "Success: addVertex: Vertex added to the map   = " << vName << endl;
//    cerr << "Success: addVertex: allVertexInRoadmap.size() = " << allVertexInRoadmap.size() << endl;
//    cerr << "Success: addVertex: totalVertexCount          = " << totalVertexCount << endl;
    
//    this->totalVertexCount++;
    return true;
}



bool roadmap::addEdge(string vertex1, string vertex2, string directional, double speed, double length){
    string v1 = toUpper(vertex1);
    string v2 = toUpper(vertex2);
    
    // when vertex exists
    int indexVertex1 = getVertexIndex(v1);
    if (indexVertex1 == -1){
        cerr << "Failed: addEdge: There is no vertex in the map named : " << v1 << endl;
        return false;
    }
    int indexVertex2 = getVertexIndex(v2);
    if (indexVertex2 == -1){
        cerr << "Failed: addEdge: There is no vertex in the map named : " << v2 << endl;
        return false;
    }
//    cerr << "indexVertex1 = " << indexVertex1 << endl;
//    cerr << "indexVertex2 = " << indexVertex2 << endl;
    
    // when edge exists
    string edgeName = v1+"-"+v2;
    int indexEdge = getEdgeIndex(edgeName);
//    cerr << "edgeName  = " << edgeName << endl;
//    cerr << "indexEdge = " << indexEdge << endl;
    
    if (indexEdge != -1){
        cerr << "Failed: addEdge: Edge already exists   : " << v1 << "-" << v2 << endl;
        return false;
    }
    
    // when direction input is incorrect
    int dir = edgeDirectionStringToInt(toUpper(directional));
    if (dir == -1){
        cerr << "Failed: addEdge: Invalid directional input         : " 
             << directional << "(Only Valid inputs are : ONE-WAY or TWO-WAY)" << endl;
        return false;
    }
    
    // assuming user enters only double values for speed and length
    // for both ONE-WAY and TWO-WAY
    if (dir == 1 || dir == 2){
        class edge onewayEdge(&allVertexInRoadmap.at(indexVertex1), &allVertexInRoadmap.at(indexVertex2), dir, speed, length);
        allEdgeInRoadmap.push_back(onewayEdge);
        cerr << "Success: addEdge: Edge added to the map     = " << onewayEdge.GetEdgeName() << endl;
//        cerr << "Success: addEdge: allEdgeInRoadmap.size()   = " << allEdgeInRoadmap.size() << endl;
        totalEdgeCount++;
    }
    // for only TWO-WAY
    if (dir == 2){
        class edge twowayEdge(&allVertexInRoadmap.at(indexVertex2), &allVertexInRoadmap.at(indexVertex1), dir, speed, length);
        allEdgeInRoadmap.push_back(twowayEdge);
        cerr << "Success: addEdge: Edge added to the map     = " << twowayEdge.GetEdgeName() << endl;
//        cerr << "Success: addEdge: allEdgeInRoadmap.size()   = " << allEdgeInRoadmap.size() << endl;
        totalEdgeCount++;
    }
    return true;
}



bool roadmap::edgeEvent(string edgeName, string edgeEventType){
    string eName = toUpper(edgeName);
    int edgeEventNumber = edgeEventStringToInt(edgeEventType);
//    cerr << "edgeEventNumber = " << edgeEventNumber << endl;
    if (edgeEventNumber == -1){
        cerr << "Failed: edgeEvent: Invalid Edge Event Type : " << edgeEventType << endl;
        cerr << "   (Valid Edge Events are: " << endl;
        cerr << "   'EVENT_NORMAL'        which sets edge speed to 100% of initial normal speed" << endl;
        cerr << "   'EVENT_ACCIDENT'      which sets edge speed to  25% of initial normal speed" << endl;
        cerr << "   'EVENT_ROAD_CLOSED'   which sets edge speed to   0% of initial normal speed" << endl;
        cerr << "   'EVENT_CONSTRUCTION'  which sets edge speed to  50% of initial normal speed" << endl;
        return false;
    }
    int i;
    for (i = 0; i < (int)allEdgeInRoadmap.size(); i++) {
        if ( allEdgeInRoadmap[i].GetEdgeName().compare(eName) == 0){
            if (edgeEventNumber == 0){
                allEdgeInRoadmap[i].SetEdgeEvent(0);
                allEdgeInRoadmap[i].SetEdgeEffectiveSpeedLimit(allEdgeInRoadmap[i].GetEdgePostedSpeedLimit());
                allEdgeInRoadmap[i].SetTimeToTravel(allEdgeInRoadmap[i].GetEdgeLength()/allEdgeInRoadmap[i].GetEdgeEffectiveSpeedLimit());
            }
            else if (edgeEventNumber == 1){
                allEdgeInRoadmap[i].SetEdgeEvent(1);
                allEdgeInRoadmap[i].SetEdgeEffectiveSpeedLimit(allEdgeInRoadmap[i].GetEdgePostedSpeedLimit()*0.25);
                allEdgeInRoadmap[i].SetTimeToTravel(allEdgeInRoadmap[i].GetEdgeLength()/allEdgeInRoadmap[i].GetEdgeEffectiveSpeedLimit());
            }
            else if (edgeEventNumber == 2){
                allEdgeInRoadmap[i].SetEdgeEvent(2);
                allEdgeInRoadmap[i].SetEdgeEffectiveSpeedLimit(0.0);
                allEdgeInRoadmap[i].SetTimeToTravel(DBL_MAX);
            }
            else if (edgeEventNumber == 3){
                allEdgeInRoadmap[i].SetEdgeEvent(3);
                allEdgeInRoadmap[i].SetEdgeEffectiveSpeedLimit(allEdgeInRoadmap[i].GetEdgePostedSpeedLimit()*0.50);
                allEdgeInRoadmap[i].SetTimeToTravel(allEdgeInRoadmap[i].GetEdgeLength()/allEdgeInRoadmap[i].GetEdgeEffectiveSpeedLimit());
            }
            cerr << "Success: edgeEvent: edge                   : " << allEdgeInRoadmap[i].GetEdgeName() << endl;
            cerr << "Success: edgeEvent: edge Event Type        : " << allEdgeInRoadmap[i].GetEdgeEvent() << endl;
            cerr << "Success: edgeEvent: edge Posted Speed      : " << allEdgeInRoadmap[i].GetEdgePostedSpeedLimit() << endl;
            cerr << "Success: edgeEvent: edge Effective Speed   : " << allEdgeInRoadmap[i].GetEdgeEffectiveSpeedLimit() << endl;
            return true;
        }
    }
    cerr << "Failed: edgeEvent: There is no edge with name: " << eName << endl;
    return false;
}



vertex* roadmap::vertex(string vertexName){
    string vName = toUpper(vertexName);
    for (int i = 0; i < (int)allPOIInRoadmap.size(); i++) {
        if ( allPOIInRoadmap[i].GetVertexName().compare(vName) == 0){
            return &allPOIInRoadmap[i];
        }
    }
    // when vertex not found
    cerr << "Failed: vertex: No point-of-interest found with name : " << vName << endl;
    return &getNullVertex().at(0);
}



//vector<edge*>  roadmap::trip(string fromVertex, string toVertex){
//    string v1 = toUpper(fromVertex);
//    string v2 = toUpper(toVertex);
//    
//    // when vertex exists
//    int indexVertex1 = getVertexIndex(v1);
//    if (indexVertex1 == -1){
//        cerr << "Failed: trip: There is no vertex in the map named : " << v1 << endl;
//        return NULL;
//    }
//    int indexVertex2 = getVertexIndex(v2);
//    if (indexVertex2 == -1){
//        cerr << "Failed: trip: There is no vertex in the map named : " << v2 << endl;
//        return NULL;
//    }
//}

//bool     roadmap::road(edge edges[]){}
//
//bool     roadmap::store(string filename){}
//
//bool     roadmap::retrieve(string filename){}










// PRIVATE METHODS

int roadmap::vertexTypeStringToInt(string vertexTypeString){
    int vertexTypeInt = -1;
    if (!vertexTypeString.compare("POI") ||
        !vertexTypeString.compare("POINT_OF_INTEREST") ){
        vertexTypeInt = 1;
    }
    if (!vertexTypeString.compare("INTERSECTION")){
        vertexTypeInt = 2;
    }
    return vertexTypeInt;
}


int roadmap::getVertexIndex(string vertexName){
    for (int i=0; i<this->allVertexInRoadmap.size(); i++){
        if (vertexName.compare(allVertexInRoadmap.at(i).GetVertexName()) == 0)
            return i;
    }
    return -1;
}


int roadmap::getEdgeIndex(string edgeName){
    for (int i=0; i<this->allEdgeInRoadmap.size(); i++){
        if (edgeName.compare(allEdgeInRoadmap.at(i).GetEdgeName()) == 0)
            return i;
    }
    return -1;
//    if (this->allEdgeInRoadmap.size() == 0) // for the first edge
//        return 0;
//    else 
//        return -1;
}


int roadmap::edgeEventStringToInt (string eventName){
    string e = toUpper(eventName);
    // 0 = EVENT_NORMAL, 
    // 1 = EVENT_ACCIDENT,      Speed = 25% of original
    // 2 = EVENT_ROADBLOCK,     Speed = 0% of original
    // 3 = EVENT_CONSTRUCTION,  Speed = 50% of original
    
    if (e.compare("EVENT_NORMAL") == 0){
        return 0;
    }
    else if (e.compare("EVENT_ACCIDENT") == 0){
        return 1;
    }
    else if (e.compare("EVENT_ROAD_CLOSED") == 0){
        return 2;
    }
    else if (e.compare("EVENT_CONSTRUCTION") == 0){
        return 3;
    }
    else {
        return -1;
    }
    
}

// checking these cases for user typo
int roadmap::edgeDirectionStringToInt (string edgeDirectionString){
    if (edgeDirectionString.compare("ONE-WAY") == 0 || 
        edgeDirectionString.compare("ONEWAY") == 0){
        return 1;
    }
    else if (edgeDirectionString.compare("TWO-WAY") == 0 ||
        edgeDirectionString.compare("TWOWAY") == 0){
        return 2;
    }
    else{
        return -1;
    }
}


string roadmap::toUpper(string str){
    string upper_str;
    locale loc;
    for (int i=0; i<str.length(); i++)
        upper_str += toupper(str[i],loc);
    return upper_str;
}