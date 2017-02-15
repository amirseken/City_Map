/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   symbol.h
 * Author: kimyou
 *
 * Created on February 19, 2016, 5:05 PM
 */
#pragma once

#include "graphics.h"
#include "LatLon.h"
#include "easygl_constants.h"

#include <string>
#include <iostream>
#include <math.h>

using namespace std;

/////////////drawing basic box for symbol //////////////////////
void create_symbol_box(float x, float y);
void create_bigger_symbol_box(float x, float y);
void draw_bottom_box (float x, float y, float width);
void draw_left_text_box (t_bound_box rect, float width, float length);


/////////// drawing different symbols ///////////////////////////////
void draw_current_location_symbol (float x, float y, float size, string type);
void draw_hospital_symbol (float x, float y);
void draw_cafe_symbol (float x, float y);
void draw_restaurant_symbol (float x, float y);
void draw_subway_symbol (float x, float y);
void draw_fastfood_symbol (float x, float y);
void draw_bank_symbol (float x, float y);
void draw_arrow(LatLon middle, LatLon to);

///////////drawing text for help button /////////////
void draw_help_text(t_bound_box rect);
void invalid_input_box(float x, float y, float width);

//////////drawing text for fastest path direction//////////
void print_help_fastest_path(t_bound_box rect);

/////////////////mathmatic function to support drawing /////////////////
float get_degree(LatLon from, LatLon to);

