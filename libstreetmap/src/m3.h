#pragma once

#include "m1.h"
#include "m2.h"
#include "global_variables.h"
#include "data_structure.h"
#include "Node_path.h"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <list>
#include <string>
#include <vector>
#include <Node_path.h>
#include <queue>
#include <cfloat>

#define UNDEFINED -2
#define INVALID_NUMBER 9999999

using namespace std;

//Returns a path (route) between the start intersection and the end 
// intersection, if one exists.
vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end);

// Returns the time required to travel along the path specified.
double compute_path_travel_time(const vector<unsigned>& path);

// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, std::string point_of_interest_name);

vector<unsigned> get_path(int* prev, unsigned start, unsigned end);

double seg_time(int u, int v);

unsigned find_segment_between_2_intersections(int u, int v);

unsigned get_u(bool* Q, double* time);

bool astarSearchPath(aNode* sourceNode, unsigned destID);

vector<unsigned> traceBack(unsigned destID);
