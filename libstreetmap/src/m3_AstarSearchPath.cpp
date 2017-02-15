#include "Node_path.h"
#include <cfloat>
#include "m3.h"
#include "global_variables.h"
#include "data_structure.h"
#include "m2.h"
#include "m1.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <list>
#include <queue>

#include "m3.h"

// Highest Speed limit for one map
double maxSpeedLimit;
struct astar_compare  
{  
    bool operator()(WaveElem a, WaveElem b)  
    {  
        return ((a.pathLen + a.prediction) > (b.pathLen + b.prediction));  
    }  
};

bool astarSearchPath(aNode* sourceNode, unsigned destID) {
    maxSpeedLimit = 110;
    // record all modified nodes
    vector<unsigned> visitedNode;
    priority_queue<WaveElem, vector<WaveElem>, astar_compare> wavefront;
    wavefront.push(WaveElem(sourceNode, NO_EDGE, 0, 0));
    bool found = false;
    LatLon destPoint = Intersection_Info [destID].pos;
    
    while ((!wavefront.empty()) && (!found)) {
        WaveElem wave = wavefront.top();
        wavefront.pop();  // Remove from wavefront
        aNode* node = wave.node;

        if (wave.pathLen < node->pathLen) {
            node->reachingEdgeID = wave.edgeID;
            node->pathLen = wave.pathLen;
            visitedNode.emplace_back(node->ID);


            if (node->ID == destID) {
               found = true;
               continue;
            }


            for (unsigned i = 0; i < (unsigned) node->adjacentNode.size(); i++) {
                aNode *toNode = &(Node_Info[(node->adjacentNode[i])]);
                int outEdgeID = node->adjacentEdge[i];
                int prevEdgeID = node->reachingEdgeID;
                double currTravelTime;
                
                //calculate path length (travel time)
                if (prevEdgeID == NO_EDGE) {
                    currTravelTime = node->pathLen + find_street_segment_travel_time (outEdgeID);
                }
                else if (Street_Segment_Info[prevEdgeID].streetID != Street_Segment_Info[outEdgeID].streetID){
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID) + 0.25;
                    
                }
                else {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID);
                }
                
                // add prediction
                LatLon nodePoint = Intersection_Info[toNode->ID].pos;               
                double twoNodeDistance = find_distance_between_two_points(destPoint, nodePoint) / 1000;
                double prdeictedTime = twoNodeDistance / maxSpeedLimit * 60;
                
                wavefront.push(
                    WaveElem(toNode, outEdgeID, currTravelTime, prdeictedTime));
            }
        }  // End if best path to this node     
    }   // End while wavefront might yield a better solution
    
    // initialize all modified nodes path length
    for (unsigned i = 0; i < (unsigned) visitedNode.size(); i ++) {
        Node_Info [(visitedNode[i])].pathLen = numeric_limits <double>::infinity();
    }  

   return found; 
}


