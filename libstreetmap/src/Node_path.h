

#ifndef NODE_PATH_H
#define	NODE_PATH_H

#define NO_EDGE -1  // Illegal edge ID

#include <vector>
#include <cfloat> // for DBL_MAX

#include <limits>

using namespace std;
struct aNode {
    unsigned ID;
    int reachingEdgeID;
    std::vector<unsigned> adjacentNode;
    std::vector<unsigned> adjacentEdge;
    double pathLen;
    double depth;
    
    aNode(unsigned id, std::vector<unsigned> nodes, std::vector<unsigned> edges) {
        ID = id;
        reachingEdgeID = NO_EDGE;
        adjacentNode = nodes;
        adjacentEdge = edges;
        pathLen = numeric_limits <double>::infinity();
    }
    aNode(){}
};

struct WeightedPath  
{  
    std::vector<unsigned> pathIDs;
    double TravelTime;
    
    WeightedPath(double t_time, std::vector<unsigned> path_ids) {
        pathIDs = path_ids;
        TravelTime = t_time;
    }
    WeightedPath(){}
};

struct WaveElem {
    aNode* node;
    int edgeID;
    double pathLen;
    double prediction;
    
    WaveElem(aNode* inputNode, int inputEdgeID, double len, double pred) {
        node = inputNode;
        edgeID = inputEdgeID;
        pathLen = len;
        prediction = pred;
    }
};

#endif	/* NODE_PATH_H */

