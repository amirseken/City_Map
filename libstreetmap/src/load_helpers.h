/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   load_helpers.h
 * Author: coralcar
 *
 * Created on February 20, 2016, 10:30 PM
 */

#ifndef LOAD_HELPERS_H
#define LOAD_HELPERS_H

/*All the structures loaded can be found as global variables under the file "global_variables.h*/

//Load Number of intersections, streets, street segments, points of interest, Features, OSM Nodes, OSM Ways and OSM relations
void load_number ();
//Load street structures
void load_street();
//Load intersection structures
void load_intersection();
//Load street segment structures
void load_street_segments();
//Load natural features structures
void load_natural();
//Load points of interest structures
void load_POI();
//Load OSMNodes multimap
void load_OSM_Nodes();
//Load OSM Ways, specifically secondary roads
void load_roads();
//Load subway stations and bus stops
void load_transit();
//m3
void load_street_segment_travel_time();
void load_node ();
#endif /* LOAD_HELPERS_H */

