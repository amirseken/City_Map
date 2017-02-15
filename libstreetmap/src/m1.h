#pragma once //protects against multiple inclusions of this header file

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "data_structure.h"
#include "math_helpers.h"
#include "load_helpers.h"
#include "Node_path.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <assert.h>

//use these defines if you need earth radius or conversion from degrees to radians
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define DEG_TO_RAD 0.017453292519943295769236907684886

using namespace std;

//function to load map streets.bin file
bool load_map(string map_name);

//close the loaded map
void close_map();

//function to return street id(s) for a street name
//return a 0-length vector if no street with this name exists.
vector<unsigned> find_street_ids_from_name(string street_name);

//function to return the street segments for a given intersection 
vector<unsigned> find_intersection_street_segments(unsigned intersection_id);

//function to return street names at an intersection (include duplicate street names in returned vector)
vector<string> find_intersection_street_names(unsigned intersection_id);

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2);

//find all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
vector<unsigned> find_adjacent_intersections(unsigned intersection_id);

//for a given street, return all the street segments
vector<unsigned> find_street_street_segments(unsigned street_id);

//for a given street, find all the intersections
vector<unsigned> find_all_street_intersections(unsigned street_id);

//function to return all intersection ids for two intersecting streets
//this function will typically return one intersection id between two street names
//but duplicate street names are allowed, so more than 1 intersection id may exist for 2 street names
vector<unsigned> find_intersection_ids_from_street_names(string street_name1, string street_name2);

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2);

//find the length of a given street segment
double find_street_segment_length(unsigned street_segment_id);

//find the length of a whole street
double find_street_length(unsigned street_id);

//find the travel time to drive a street segment (time(minutes) = distance(km)/speed_limit(km/hr) * 60
double find_street_segment_travel_time(unsigned street_segment_id);

//find the nearest point of interest to a given position
unsigned find_closest_point_of_interest(LatLon my_position);

//find the nearest intersection (by ID) to a given position
unsigned find_closest_intersection(LatLon my_position);

double find_distance_between_two_points_2(LatLon point1, LatLon point2);
