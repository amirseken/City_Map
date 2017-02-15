/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   global_variables.h
 * Author: tokhoi
 *
 * Created on February 8, 2016, 6:43 PM
 */
#include "m1.h"
#include <mutex>
#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

//************************* GLOBAL VARIBLES  ***************************

extern struct Number number;                                                           //numbers of street, street segment, and intersection
extern unordered_multimap<string, unsigned> Map_street;                                //street name -> street id
extern multimap<unsigned long long, NodeInfo> OSMNodes;

extern unordered_map<string, unsigned> Middle_Street_Segment;                   // M3

//Street - Street Segment - Intersection Info Containers
extern vector<StreetSegmentInfo_2> Street_Segment_Info; 
extern vector<IntersectionInfo> Intersection_Info;                                    
extern vector<StreetInfo> Street_Info;
extern vector<Natural> Natural_Info;
extern vector<POI> POI_Info;
extern vector<NodeInfo> Subway_Stations;
extern vector<NodeInfo> Secondary_Road_Nodes;
extern vector<NodeInfo> Highway_Nodes; //Primary Roads
extern unsigned Destination_Id;

extern mutex dpSync;

//m2
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
extern Window Search;
extern XftDraw* Draw;
extern string Search_Text;


//m3
#include "Node_path.h"
using namespace std;
extern unordered_multimap <string, unsigned> POI_Name_Map;
extern vector <aNode> Node_Info;
#define NUM_THREADS 8
extern vector <aNode> Node_Map [NUM_THREADS];
extern vector <unsigned> Found_Path;
extern double maxSpeedLimit;
#endif /* GLOBAL_VARIABLES_H */

