/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   data_structure.h
 * Author: tokhoi
 *
 * Created on February 22, 2016, 4:47 PM
 */

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <string>
#include <vector>

using namespace std;

struct StreetSegmentInfo_2{ // new Streetsegmentinfo because old one contains extra info.
    unsigned from, to; // intersection ID this segment runs from/to
    bool oneWay; // if true, then can only travel in from->to direction

    unsigned curvePointCount; // number of curve points between the ends
    float speedLimit;
    unsigned streetID; // index of street this segment belongs to
    vector<LatLon> curvePointPos;
    double travelTime;
};

struct IntersectionInfo{ // structure of intersection info
    string name;
    LatLon pos;
    vector<unsigned> Street_Segments;
    vector<unsigned> outEdge;
    vector <unsigned> adjacentNode;
};

struct StreetInfo{ // structure of street info
    string Name;
    vector<unsigned> Intersections;
    vector<unsigned> Street_Segments;
};

struct Number{ // structure that contains total number of street, segment intersection...
    unsigned Streets;
    unsigned Street_Segments;
    unsigned Intersections;
    unsigned Points_Of_Interest;
    unsigned Natural_Feature;
    unsigned long long OSM_Node;
    unsigned long long OSM_Way;
    unsigned long long OSM_Relation;
};

struct Natural{
  FeatureType Natural_type;
  string Natural_name;
  unsigned Point_Count;
  vector<LatLon> Natural_Position;
  OSMID Natural_OSM_Id;
  OSMEntityType Natural_OSM_Type;
};

struct POI{
    string type;
    string name;
    LatLon position;
};

struct NodeInfo {
    unsigned long long id;
    LatLon pos;
    int tag;
    string roadName;
    
    NodeInfo ();
    NodeInfo (unsigned long long _id , LatLon _pos, int _tag, string _roadName ) { id = _id ; pos = _pos; tag = _tag; roadName = _roadName;}
};


//m3
struct poi_str{
    unsigned id;
    double dist;
};

struct poiCompare  
{  
    bool operator()(poi_str a, poi_str b)  
    {  
        return (a.dist > b.dist);  
    }  
};
#endif /* DATA_STRUCTURE_H */

