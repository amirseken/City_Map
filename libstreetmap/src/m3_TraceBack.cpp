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

// declare the global MapStructure variable

std::vector<unsigned> traceBack(unsigned destID) {
    std::vector<unsigned> path;
    aNode* currNode = &Node_Info[destID];
    int prevEdgeID = currNode->reachingEdgeID;
    
    while (prevEdgeID != NO_EDGE) {
        path.push_back(prevEdgeID);
        // find previous node
        StreetSegmentInfo_2 info = Street_Segment_Info [prevEdgeID];
        if (info.from == currNode->ID) {
            currNode = &Node_Info[info.to];
        }
        else {
            currNode = &Node_Info[info.from];
        }
        prevEdgeID = currNode->reachingEdgeID;
    }
    std::reverse(path.begin(),path.end());
    return path;
}

std::vector<unsigned> traceBack_m4(unsigned destID, vector <aNode>& node_map) {
    std::vector<unsigned> path;
    aNode* currNode = &node_map[destID];
    int prevEdgeID = currNode->reachingEdgeID;
    
    while (prevEdgeID != NO_EDGE) {
        path.push_back(prevEdgeID);
        // find previous node
        StreetSegmentInfo_2 info = Street_Segment_Info [prevEdgeID];
        if (info.from == currNode->ID) {
            currNode = &node_map[info.to];
        }
        else {
            currNode = &node_map[info.from];
        }
        prevEdgeID = currNode->reachingEdgeID;
    }
    std::reverse(path.begin(),path.end());
    return path;
}