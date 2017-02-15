/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   parser.h
 * Author: tokhoi
 *
 * Created on February 12, 2016, 5:05 PM
 */

#ifndef PARSER_H
#define PARSER_H

#include "m1.h"
#include "global_variables.h"

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

/********************   LOAD NEW MAP    ****************/
string new_map_parser();
void load_new_map(int map_id);
string map_name_2(int map_id);

/********************   FIND INTERSECTIONS   **************/
vector<LatLon> find_intersection_parser();
vector<unsigned> find_street_parser();

/********************   AUTO-COMPLETE         ***************/
string look_for_match(string);
string complete_search_box_input (string text, int length, string type);
string fix_blank_spaces(string);
string match_POI(string);

#endif /* PARSER_H */

