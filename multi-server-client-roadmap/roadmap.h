/* 
 * File:   roadmap.h
 * Author: Anjoom
 *
 */
//
#ifndef ROADMAP_H
#define ROADMAP_H

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

using namespace std;

class vertex;
class edge;
class road;

class roadmap {
private:
    vector<vertex>  allVertexInRoadmap; // to store all the vertex objects
    vector<vertex>  allPOIInRoadmap;    // to store all the vertex objects that are POI
    vector<edge>    allEdgeInRoadmap;   // to store all the edge objects
    vector<road>    allRoadsInRoadmap;  // to store all the road objects
    int             totalVertexCount;   // to store all the total number of vertex objects
    int             totalEdgeCount;     // to store all the total number of edge objects
    vector<vertex>  nullVertex;         // to store 1 null vertex object
    vector<edge>    nullEdge;           // to store 1 null edge object

    
    int             vertexTypeStringToInt(string vertexTypeString);
    int             getVertexIndex(string vertexName);
    int             getEdgeIndex(string edgeName);
    int             edgeDirectionStringToInt (string directional);
    int             edgeEventStringToInt (string eventName);
    string          toUpper(string str);
    
public:
                    roadmap();
    virtual         ~roadmap();
    vector<vertex>  getAllVertexInRoadmap() const;
    vector<edge>    getAllEdgeInRoadmap() const;
    vector<road>    getAllRoadsInRoadmap() const;
    int             getTotalVertexCount() const;
    int             getTotalEdgeCount() const;
    vector<vertex>  getNullVertex() const;
    vector<edge>    getNullEdge() const;
    
    bool            addVertex(string vertexType, string label);
    bool            addEdge(string vertex1, string vertex2, string directional, double speed, double length);
    bool            edgeEvent(string edgeName, string edgeEventType);
    vertex*         vertex(string vertexName);
    vector<edge*>   trip(string fromVertex, string toVertex);
    bool            road(edge edges[]);
    bool            store(string filename);
    bool            retrieve(string filename);
    


};



#endif /* ROADMAP_H */

