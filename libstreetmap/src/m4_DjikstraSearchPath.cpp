/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include "m3.h"
#include "m4.h"
#include "Node_path.h"
#include <queue>
#include <vector>
#include <cfloat>
#include <iostream>
#include "m2.h"
#include "graphics.h"
#include <algorithm>
#include "limits.h"
#include "math.h"
#include <stack>
#include <time.h> 
#include <stdlib.h>
#include <mutex>

#include "global_variables.h"
#include "data_structure.h"

mutex dpSync; 

struct m4_djikstra_compare  
{  
    bool operator()(WaveElem a, WaveElem b)  
    {  
        return (a.pathLen > b.pathLen);  
    }  
};

void m4_djikstraSearchPath(aNode* sourceNode, std::vector<unsigned> otherintersectID) {
    // record all modified nodes
    std::vector<unsigned> visitedNode;
    std::priority_queue<WaveElem, std::vector<WaveElem>, m4_djikstra_compare> wavefront;
    wavefront.push(WaveElem(sourceNode, NO_EDGE, 0, 0));
    bool found = false;
    
    while ((!wavefront.empty()) && (!found)) { 
        WaveElem wave = wavefront.top();
        wavefront.pop();  // Remove from wavefront
        aNode* node = wave.node;

        if (wave.pathLen < node->pathLen) {
            node->reachingEdgeID = wave.edgeID;
            node->pathLen = wave.pathLen;
            visitedNode.emplace_back(node->ID);

            for (int i = 0; i < (int) otherintersectID.size(); i ++) {
                if (node->ID == otherintersectID[i]) {
                    otherintersectID.erase(otherintersectID.begin()+i);
                }
            }
            if (otherintersectID.empty()) {
                found = true;
            }


            for (unsigned i = 0; i < (unsigned) node->adjacentNode.size(); i++) {
                aNode *toNode = &(Node_Info[(node->adjacentNode[i])]);
                int outEdgeID = node->adjacentEdge[i];
                int prevEdgeID = node->reachingEdgeID;
                double currTravelTime;
                //calculate path length (travel time)
                if (prevEdgeID == NO_EDGE) {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID);
                }
                else if (Street_Segment_Info[prevEdgeID].streetID != Street_Segment_Info[outEdgeID].streetID) {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID) + 0.25;
                }
                else {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID);
                }                
                
                wavefront.push(
                    WaveElem(toNode, outEdgeID, currTravelTime, 0));
            }
        }  // End if best path to this node     
    }   // End while wavefront might yield a better solution
    
    // initialize all modified nodes path length
    for (unsigned i = 0; i < (unsigned) visitedNode.size(); i ++) {
        Node_Info [(visitedNode[i])].pathLen = numeric_limits <double>::infinity();
    }  

}

vector<WeightedPath> m4_djikstraSearchPath_2(vector<aNode>& node_map, aNode* sourceNode, vector<unsigned>& otherintersectID, vector<int>& traverse) {
    
    // record all modified nodes
    std::vector<unsigned> visitedNode;
    std::priority_queue<WaveElem, std::vector<WaveElem>, m4_djikstra_compare> wavefront;
    wavefront.push(WaveElem(sourceNode, NO_EDGE, 0, 0));
    bool found = false;
    
    while ((!wavefront.empty()) && (!found)) { 
        WaveElem wave = wavefront.top();
        wavefront.pop();  // Remove from wavefront
        aNode* node = wave.node;

        if (wave.pathLen < node->pathLen) {
            node->reachingEdgeID = wave.edgeID;
            node->pathLen = wave.pathLen;
            visitedNode.emplace_back(node->ID);

            for (int i = 0; i < (int) otherintersectID.size(); i ++) {
                if (node->ID == otherintersectID[i]) {
                    otherintersectID.erase(otherintersectID.begin()+i);
                }
            }
            if (otherintersectID.empty()) {
                found = true;
            }


            for (unsigned i = 0; i < (unsigned) node->adjacentNode.size(); i++) {
                aNode *toNode = &(node_map[(node->adjacentNode[i])]);
                int outEdgeID = node->adjacentEdge[i];
                int prevEdgeID = node->reachingEdgeID;
                double currTravelTime;
                //calculate path length (travel time)
                if (prevEdgeID == NO_EDGE) {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID);
                }
                else if (Street_Segment_Info[prevEdgeID].streetID != Street_Segment_Info[outEdgeID].streetID) {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID) + 0.25;
                }
                else {
                    currTravelTime = node->pathLen + find_street_segment_travel_time(outEdgeID);
                }                
                
                wavefront.push(
                    WaveElem(toNode, outEdgeID, currTravelTime, 0));
            }
        }  // End if best path to this node     
    }   // End while wavefront might yield a better solution
    
    // initialize all modified nodes path length
    for (unsigned i = 0; i < (unsigned) visitedNode.size(); i ++) {
        node_map [(visitedNode[i])].pathLen = numeric_limits <double>::infinity();
    }  

    
    
    std::vector<unsigned> path;
    std::vector<WeightedPath> singleWeight;
    std::vector<unsigned> emptyVector;
    WeightedPath emptyWeight(0.0, emptyVector);
    for (unsigned i = 0; i < traverse.size(); i++) {
        if (traverse [i] != -1) {
            
            path = traceBack_m4(traverse[i], node_map);
            singleWeight.emplace_back(WeightedPath(compute_path_travel_time(path), path));
        }
        else {
            singleWeight.emplace_back(emptyWeight);
        }
    }

    return singleWeight;
}