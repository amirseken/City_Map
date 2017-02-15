#include "m1.h"
#include "m3.h"
#include "m4.h"
#include <queue>
#include <vector>
#include <cfloat>
#include <iostream>
#include "m2.h"
#include <algorithm>
#include "limits.h"
#include "math.h"
#include <stack>
#include <time.h> 
#include <stdlib.h>

#include "Node_path.h"
#include "global_variables.h"
#include "data_structure.h"

#include <thread>
#define TIME_LIMIT 30
using namespace std;

//extern void draw_street_segment_for_path(unsigned streetSegmentID);
std::vector< std::vector<WeightedPath> > jointGraph;


// help functions

vector<WeightedPath>* traverseWeight;                      //traverse weight
vector<WeightedPath>* depotWeight;                         //depot weight
vector<unsigned> traverse_intersections;
vector<unsigned> g_depots;
int traverseSize, depotSize;

void delivery_map_generate (int start, int end, vector<aNode>& node_map);
void depot_map_generate (int start, int end, vector<aNode>& node_map);


std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots) {
    clock_t startTime = clock ();
    
    g_depots = depots;
    

    for (unsigned i=0; i < deliveries.size(); i++){
        traverse_intersections.push_back (deliveries [i].pickUp);
        traverse_intersections.push_back (deliveries [i].dropOff);
    }
    
    traverseSize =(int) traverse_intersections.size();
    depotSize =(int) depots.size();
    std::vector<unsigned> emptyVector;
    WeightedPath emptyWeight(0.0, emptyVector);
    std::vector<unsigned> finalPath;

    
    srand(time(NULL));
 
    traverseWeight = new vector<WeightedPath> [traverseSize];
    depotWeight = new vector<WeightedPath> [depotSize];
    
    thread myThread[NUM_THREADS];
    
    //------------ Djiktra search to find all paths
    for (int ithr=0; ithr<NUM_THREADS; ithr++){
        myThread[ithr] = thread(delivery_map_generate, ithr*traverseSize/NUM_THREADS, (ithr+1)*traverseSize/NUM_THREADS, ref(Node_Map [ithr]));
    }
    for (int ithr=0; ithr<NUM_THREADS; ithr++){
        myThread[ithr].join();
    }
    
    for (int ithr=0; ithr<NUM_THREADS; ithr++){
        myThread[ithr] = thread(depot_map_generate, ithr*depotSize/NUM_THREADS, (ithr+1)*depotSize/NUM_THREADS, ref(Node_Map [ithr]));
    }
    for (int ithr=0; ithr<NUM_THREADS; ithr++){
        myThread[ithr].join();
    }
    
    
    //is picked up
    bool isPickedUp [traverseSize]  = {false};
    for (int i =0; i<traverseSize; i = i + 2){
        isPickedUp [i] = true;
    }
    
    
    
    
    //------------   GREEDY TSP
    unsigned start_depot, start_pickup;
    vector <unsigned> solution;
    vector <unsigned> deliveryOrder; // container of intersection path
    vector <unsigned> packages = traverse_intersections; 
    
    //find the start depot to nearest pick up
    double min_time = DBL_MAX;
    for (unsigned i=0; i < (unsigned)depotSize; i++){ //go from any depot to nearest pickup
        for (unsigned j=0; j < (unsigned)traverseSize; j = j + 2){ //only choose pick up
            if (depotWeight [i][j].TravelTime < min_time){
                min_time = depotWeight [i][j].TravelTime;
                start_depot = i;
                start_pickup = j;
            }
        }
    }
    deliveryOrder.push_back (start_depot);
    deliveryOrder.push_back (start_pickup);    //add first pickup after start depot
    
    unsigned p = start_pickup;
    min_time = DBL_MAX;
    int remaining = traverseSize - 1;
    bool visited [traverseSize] = {false};
    
    //find delivery order
    visited [p] = true;
    isPickedUp [p+1] = true;
    while(remaining > 0){ //total number of packages - 1
        unsigned new_p;
        for (int i=0; i < traverseSize; i++){   //check nearest pick up or legal drop off from p
            if (!visited [i] && isPickedUp [i]){ 
                if(traverseWeight [p][i].TravelTime < min_time){
                    min_time = traverseWeight [p][i].TravelTime;
                    new_p = i;
                }
            }
        }
        
        p = new_p;
        visited [p] = true;
        min_time = DBL_MAX;
        if (p % 2 == 0) //if id is a pick up int
            isPickedUp [p+1] = true; //package is picked up and ready to drop off
        
        deliveryOrder.push_back (p);
        
        remaining--;
    }
    // find the nearest stop depot
    min_time = DBL_MAX;
    unsigned stop_depot;
    for (int i=0; i<depotSize; i++){
        if (traverseWeight [p][traverseSize + i].TravelTime < min_time){
            min_time = traverseWeight [p][traverseSize + i].TravelTime;
            stop_depot = i;
        }
    }
    deliveryOrder.push_back (stop_depot);
    
    //path from start depot to first pick up
    vector <unsigned> path = depotWeight [start_depot] [deliveryOrder [1]].pathIDs;
    finalPath.insert (finalPath.end(), path.begin(), path.end());
    
    //path from delivery to delivery
    for (unsigned i=1; i<=deliveryOrder.size()-3; i++){
        path = traverseWeight [deliveryOrder [i]] [deliveryOrder [i+1]].pathIDs;
        finalPath.insert (finalPath.end(), path.begin(), path.end());
    }
    
    //path from last delivery to stop depot
    unsigned last_delivery = deliveryOrder [deliveryOrder.size()-2];
    path = traverseWeight [last_delivery] [traverseSize + stop_depot].pathIDs;
    
    if (path.size() == 0){
        unsigned intersection_start = traverse_intersections [last_delivery];
        unsigned intersection_end = depots [1];
        path = find_path_between_intersections (intersection_start, intersection_end);
    }
    finalPath.insert (finalPath.end(), path.begin(), path.end());
    

    
    
    traverse_intersections.clear();
    delete [] traverseWeight;
    delete [] depotWeight;
    
    
    return finalPath;
}

void delivery_map_generate (int start, int end, vector<aNode>& node_map){
    for (int i = start; i < end; i++) {
        std::vector<unsigned> otherintersectID;
        //initialize other intersect id
        for (int j = 0; j < traverseSize; j++) {
            if (i != j)
                otherintersectID.emplace_back(traverse_intersections[j]);
        }
        for (int j = 0; j < depotSize; j++) {
            otherintersectID.emplace_back(g_depots[j]);
        }
        
        
        //initialize traverse
        vector <int> traverse;
        for (int j=0; j<traverseSize; j++){
            if(i!=j)
                traverse.push_back(traverse_intersections[j]);
            else
                traverse.push_back(-1);
        }
        for (int j=0; j<depotSize; j++){
            traverse.push_back(g_depots[j]);
        }
        aNode* sourceNode = &(node_map[(traverse_intersections[i])]);
        traverseWeight[i] = m4_djikstraSearchPath_2(node_map, sourceNode, otherintersectID, traverse); 

    }
}

void depot_map_generate (int start, int end, vector<aNode>& node_map){
    for (int i = start; i < end; i++) {
        std::vector<unsigned> otherintersectID;
        //initialize other intersect id
        for (int j = 0; j < traverseSize; j++) {
            otherintersectID.emplace_back(traverse_intersections[j]);
        }
        //initialize traverse
        vector <int> traverse;
        for (int j=0; j<traverseSize; j++){
                traverse.push_back(traverse_intersections[j]);
        }
        
        aNode* sourceNode = &(node_map[(g_depots[i])]);
        depotWeight[i] = m4_djikstraSearchPath_2(node_map, sourceNode, otherintersectID, traverse); 
    }  
}
