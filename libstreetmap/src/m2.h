#pragma once

#include "easygl_constants.h"
#include "graphics.h"
#include "global_variables.h"
#include "m1.h"
#include "m3.h"
#include "parser.h"
#include "symbol.h"

#include <string>
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cstddef> 

// Draws the map. You can assume your load_map (string map_name)
// function is called before this function in the unit tests.
// Your main () program should do the same.
void draw_map();

//load all drawing functions
void load();

// By clicking one point function will find all point of interest in area of circle.
// Scale can be changed
void find_point_of_interest_around_one_point ();

// By clicking botton or symbol of interest, same symbols will be disabled.
void disable_certain_interset ();

// if user found intersection or street, if automatically zoom in
void draw_map_found_point(float x, float y);

int m2();

//Your program must be able to load and visualize any map without recompilation 
//(i.e.you must get the map name from user input in some way).

//draw street
void draw_street();
void draw_street_name();
void draw_map_name();
void draw_one_way();
void draw_main_street();
void draw_highways();

//draw point of interest
void draw_POI();
void draw_POI_info();

//draw features
void draw_feature();
void draw_feature_info();

void draw_natural();

//The user must be able to click on an intersection; the intersection should be highlighted
//and information about it should be either displayed in the graphics or printed to the
//command (terminal) window.
void hilight_intersection(float x, float y);
void hilight_street();

void draw_highlited_intersection();
void draw_interest_symbol();
void draw_intersection_text();
void draw_outside_map(t_bound_box initial);
void draw_highlighted_symbol();
void draw_help();
void draw_invalid_input();

//The user must be able to click on a Find button in the user interface, type in the names
//of two streets (at the command prompt is fine), and have all intersections between those
//street names be highlighted in the graphics and information about the intersections be
//shown in the graphics or printed to the terminal window.
void hilight_intersections_from_street_names();
void hilight_symbol(float x,float y);

/***********************    NEW BUTTON      ***********************/
void change_map_button (void (*load_ptr) (void));
void show_interest_button (void(*load_ptr)(void));
void disable_interest_button (void(*load_ptr)(void));
void find_intersection_button( void(*load_ptr)(void));
void find_street_button( void(*load_ptr)(void));
void fastest_path_type_button( void(*load_ptr)(void));
void fastest_path_click_button( void(*load_ptr)(void));
void help_button( void(*load_ptr)(void));
/***********************      MATHEMATICS SUPPORT       ***********************/
//map projection
float get_cosine();     //return cosine from ymin and ymax of the map
void get_min_max(float&, float&, float&, float&);

void initialize_world();
void act_on_button_press (float x, float y, t_event_buttonPressed event);
void act_on_key_press(char c, int key);
void find_interest (float x, float y, float distance);
// drawing symbols
void draw_street(float to_lon, float to_lat, float from_lon, float from_lat, string type);

/*********************** Find Fastest Path helper function ********************/
void draw_fastest_path(vector<unsigned> result);
void draw_fastest_path_in_load_map();
void print_fastest_path (vector<unsigned> result, unsigned start, unsigned end);
string get_direction(float degree);