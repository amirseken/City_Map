//Reference: Example.cpp [Online]. Available: http://www.eecg.utoronto.ca/~vaughn/ece297/ECE297/assignments/easygl/code.zip

#include "m2.h"
#include "m3.h"
#include <chrono> 

//error range that is used to display map
#define MINIMUM_ERROR_RANGE 0.00005
#define MAXIMUM_ERROR_RANGE 0.0005


//screen area used to show certain elements in certain zoom level
#define FIRST_SCREEN_AREA 0.259645
#define HIGHWAY_SCREEN_AREA 0.0447331
#define STREET_SCREEN_AREA 0.00763326
#define BUILDING_SCREEN_AREA 0.00158052
#define STREETNAME_SCREEN_AREA 0.000213855
#define ARROW_SCREEN_AREA 7.42585e-05
#define LOCATION_SYMBOL_SCREEN_AREA 2.66424e-05
#define POI_SCREEN_AREA 9.70398e-06

#define INVALID_NUMBER 9999999

//ratio that convert real distance to map scale distance
#define REAL_DISTANCE_TO_MAP 0.0000035
#define PI 3.14159265

using namespace std;

//global variables for m2.cpp
vector<LatLon> highlighted_pos;

vector<unsigned> highlighted_street_id;
vector<LatLon> Pos;

LatLon highlighted_symbol;
IntersectionInfo highlighted_info;
t_bound_box initial_coords;
t_bound_box current_screen;
t_point max_screen_cordin, min_screen_cordin;

string Map_Name;
float cosine;
float get_distance;
bool interest_button, highlight_interesction, symbol_click;
bool help_button_click, help_draw_click, invalid_input;


//global variables used to find fastest path
bool fastest_path, fastest_click_button, fastest_type_button, fastest_type_found;
bool fastest_path_type, got_all_info, passed, closest_poi;
bool intersection1_street1, intersection1_street2, intersection2_street1;
string type, name_begin, name_end;
unsigned Destination_Id, Start_Id;


//search box
string Search_Text;

/***************************    INITIALIZATION      ****************************/

void draw_map() {

    highlighted_pos.clear();
    highlighted_street_id.clear();
    //initialize world
    Map_Name = "Toronto";
    initialize_world();
    //drawing loopdraw_street_name
    set_mouse_move_input(true);
    set_keypress_input(true);
    event_loop(act_on_button_press, NULL, act_on_key_press, load);   
   
    close_graphics ();
    
}

void load(){
    
    clearscreen();
    
    //draw outside of map with white space for user's visibility
    draw_outside_map(initial_coords); 
    
    //draw basic features of map
    draw_natural();
    draw_street();
    draw_street_name();
    //draw_POI();  
    draw_map_name();
    
    //draw extra features of map including button event.
    draw_interest_symbol(); 
    draw_highlighted_symbol(); 
    draw_highlited_intersection();
    draw_intersection_text();
    draw_help();
    
    // draw fastest path
    draw_fastest_path_in_load_map();
    hilight_street();
    draw_invalid_input();
    //m4
    draw_fastest_path(Found_Path);
}


void initialize_world(){
    float xmin, xmax, ymin, ymax;
    cosine = get_cosine();
    init_graphics("ECE 297 - " + Map_Name + " Map", NUM_COLOR); 
    get_min_max(xmin, xmax, ymin, ymax);
    initial_coords = t_bound_box(xmin, ymin , xmax, ymax );
    set_visible_world(initial_coords);
    
    //create buttons
    create_button("Window", "Help", help_button);
    create_button("Window", "---1", change_map_button);
    create_button("Window", "Find Closest POI", find_intersection_button);
    create_button("Window", "Find Street", find_street_button);
    create_button("Window", "Fastest Path(Type)", fastest_path_type_button);
    create_button("Window", "Fastest Path(Click)", fastest_path_click_button);
    create_button("Window", "Disable All", disable_interest_button);
    create_button("Window", "Find POI", show_interest_button);
    create_button("Window", "Change Map", change_map_button);
    create_button("Window", "---1", change_map_button);
    
    //set bools as false   
    get_distance = INVALID_NUMBER;
    interest_button = false;
    highlight_interesction = false;
    symbol_click = false;
    help_button_click = false;
    help_draw_click = false;
    fastest_path = false;
    fastest_path_type = false;
    got_all_info = false;
    fastest_click_button = false;
    fastest_type_button = false;
    fastest_type_found = false;
    intersection1_street1 = false;
    intersection1_street2 = false;
    intersection2_street1 = false;
    invalid_input = false; 
    closest_poi = false;
}
/****************************   DRAWING FUNCTIONS   ***************************/
void draw_street(){
    
    set_draw_mode (DRAW_NORMAL);
    setlinestyle (SOLID);
    setlinewidth(2);
    string type = "normal";

    if (current_screen.area() < STREET_SCREEN_AREA){ //if current screen is smaller than certain area it draws street
        for(unsigned i=0; i < number.Street_Segments; i++){
            
            StreetSegmentInfo_2 info;
            LatLon from, to;
            
            info = Street_Segment_Info[i];
            from = Intersection_Info[info.from].pos;
            to = Intersection_Info[info.to].pos;
            
            // it doesn't draw street outside of window.
            if (max_screen_cordin.x > from.p_lon() || max_screen_cordin.x > to.p_lon()
                || min_screen_cordin.x < from.p_lon() || min_screen_cordin.x < to.p_lon()
                || max_screen_cordin.y > from.lat || max_screen_cordin.y > to.lat
                || min_screen_cordin.y < from.lat || min_screen_cordin.y < to.lat){
                
                if(info.oneWay == true)
                    type = "oneway";
                else
                    type = "normal";
                
                if (info.curvePointCount != 0){//if street segment has curve points
                    vector<LatLon> curve;
                    int size;
                    size = info.curvePointCount;
                    curve = info.curvePointPos;

                    //draw from street_segment.from to first curve point
                    draw_street(from.p_lon(), from.lat, curve[0].p_lon(), curve[0].lat, type);
                    //draw curve from point to point
                    for(unsigned j = 0; j < info.curvePointCount - 1; j++){
                        draw_street(curve[j].p_lon(), curve[j].lat, curve[j+1].p_lon(), curve[j+1].lat, type);
                    }
                    //draw from last curve point to street_segment.to
                    draw_street(to.p_lon(), to.lat, curve[size-1].p_lon(), curve[size-1].lat, type);
                } 
                else{            
                    draw_street(from.p_lon(), from.lat, to.p_lon(), to.lat, type);  
                }
                setlinestyle(SOLID);
            }
        }
    }
    
    // draw highway and main street over street.
    if (current_screen.area() < HIGHWAY_SCREEN_AREA){
        draw_main_street();
        draw_highways();
    }
        //draw arrow that shows way of one-way street segments
        if (current_screen.area() < ARROW_SCREEN_AREA){
            for(unsigned i=0; i < number.Street_Segments; i++){           
                StreetSegmentInfo_2 info;
                LatLon from, to;

                info = Street_Segment_Info[i];
                from = Intersection_Info[info.from].pos;
                to = Intersection_Info[info.to].pos;

                    if (info.oneWay == true){
                        LatLon median;            
                        setlinewidth(2);
                        if(Street_Segment_Info[i].curvePointCount > 2){ // draw arrow that shows direction of one way road
                            int median = round(Street_Segment_Info[i].curvePointCount/2);
                            from = Street_Segment_Info[i].curvePointPos[median-1];
                            to = Street_Segment_Info[i].curvePointPos[median];
                           // draw_arrow(from,to);
                        }
                        else{
                            median.lon = (from.p_lon() + to.p_lon())/2;
                            median.lat = (from.lat + to.lat)/2;
                           // draw_arrow(median, to);
                        }
                    }
            }
    }   
}

void draw_map_name(){ // draw map name in the middle of map such as TORONTO
    if (current_screen.area() > FIRST_SCREEN_AREA){
        setfontsize(40);
        setcolor(100,100,100);
        drawtext(initial_coords.get_center().x, initial_coords.get_center().y, Map_Name, initial_coords.get_width(), initial_coords.get_height());
    }
}

void draw_street_name(){
    string street_name_compare = "INVALID";
    int count_name = 0;
    if (current_screen.area() < STREETNAME_SCREEN_AREA){
        for (unsigned i =0; i < number.Street_Segments; i++){
            LatLon from = Intersection_Info[Street_Segment_Info[i].from].pos;
            LatLon to = Intersection_Info[Street_Segment_Info[i].to].pos;        
            string street_name = Street_Info[Street_Segment_Info[i].streetID].Name;
            
            float width = fabs(from.lat-to.lat);
            
            if (width < fabs(from.lon - to.lon)) // find bigger differnece of lat and lon and set as width.
                width = fabs(from.lon - to.lon);
        
        //if there are curve point draw street name in the middle of curvepoint
        if(Street_Segment_Info[i].curvePointCount > 0){
            int median = round(Street_Segment_Info[i].curvePointCount/2);
            from = Street_Segment_Info[i].curvePointPos[median-1];
            to = Street_Segment_Info[i].curvePointPos[median];
        }
            if (count_name == 5){
                street_name_compare = "INVALID";
                count_name = 0;
            }
            //eliminate all street name <unknown> , and duplicate street names, if street name is equal to last street name it doesn't draw. 
            if (street_name != "<unknown>" && street_name != street_name_compare){
            setfontsize(8);
            setcolor(BLACK); 
            
            from.lon = from.p_lon();
            to.lon = to.p_lon();
            float degree = get_degree(from, to);
            
            if (degree>100 && degree<260){ //convert fliped text into original text.
                degree = degree + 180;
                if(degree>=360)
                    degree = degree - 360;   
            }
            
            else
                count_name = count_name + 1;
            settextrotation(degree);
            drawtext ((from.lon+to.lon)/2, (to.lat+from.lat)/2, street_name, width, width);   
            street_name_compare = street_name;
            }
            
        }
    }    
    settextrotation(0); //set text rotate to origin
}

void draw_main_street()
{
    string type = "main";    
    for (vector<NodeInfo>::iterator it = Secondary_Road_Nodes.begin(); it < Secondary_Road_Nodes.end() ; it++)
    {
        vector<NodeInfo>::iterator it_next = it;
        it_next ++;
        int tag1 = it->tag;
        int tag2 = it_next->tag;
        //As long as the nodes have the same tag (Road index to which they belong), draw a highway between them
        if (tag1 == tag2)
        {
            setcolor(255,236,122);
            LatLon posFrom = it->pos;
            LatLon posTo = it_next->pos;
            draw_street(posFrom.p_lon(), posFrom.lat, posTo.p_lon(), posTo.lat, type ); 
        }               
    }
}

void draw_highways()
{   
    string type = "highway";
    for (vector<NodeInfo>::iterator it = Highway_Nodes.begin(); it < Highway_Nodes.end() ; it++)
    {
        vector<NodeInfo>::iterator it_next = it;
        it_next ++;
        int tag1 = it->tag;
        int tag2 = it_next->tag;
        //As long as the nodes have the same tag (Road index to which they belong), draw a highway between them
        if (tag1 == tag2)
        {
            setcolor(255,236,122);
            LatLon posFrom = it->pos;
            LatLon posTo = it_next->pos;
            draw_street(posFrom.p_lon(), posFrom.lat, posTo.p_lon(), posTo.lat, type ); 
        }               
    }    
}

void draw_natural(){
    
    //get current screen information here, since draw_nature is first layer of the map.
    current_screen= get_visible_world();
    max_screen_cordin = current_screen.top_right();
    min_screen_cordin = current_screen.bottom_left();
    
    set_draw_mode (DRAW_NORMAL);
    setlinestyle (SOLID);
    setlinewidth(3);
    
    //draw island first because it cover all other Nature of features
    for (unsigned i=0; i < number.Natural_Feature; i++){ // 
        unsigned int p_tot = Natural_Info[i].Point_Count;
        t_point points[p_tot];
        FeatureType type = (Natural_Info[i].Natural_type);
        
        for(unsigned j=0; j < (p_tot)-1 ; j++){
            points[j].x = Natural_Info[i].Natural_Position[j].p_lon();
            points[j].y = Natural_Info[i].Natural_Position[j].lat;
            }
             if (type == Island){
                setcolor (208,199,170); 
                fillpoly(points, p_tot-1);
            }
             else if (type == Lake){
                 setcolor (135,202,215); 
                 fillpoly(points, p_tot-1);
             }
    }
    
    //draw rest of nature features.
    for (unsigned i=0; i < number.Natural_Feature; i++){
        bool building = false;
        
        unsigned int p_tot = Natural_Info[i].Point_Count;
        t_point points[p_tot];
        FeatureType type = (Natural_Info[i].Natural_type);
        switch (type)
        {
            case Park: 
                setcolor (173,243,152);  break;
            case Greenspace:
                setcolor (159,226,75); break;
            case Beach: 
                setcolor (216,199,151);  break;
            case Stream:
                setcolor (96,165,215); break;
            case Island:
                setcolor (208,199,170); break;
            case Lake: 
                setcolor (135,202,215);  break;
            case River:
                setcolor (156,199,255);  break;
            case Golfcourse:
                setcolor (202,223,170);  break;
            case Shoreline: 
                setcolor (216,199,151);  break;
            case Building: 
                setcolor (194,205,207);  break;
            case Unknown:
                setcolor (BLACK); break;
        }
            if (type == Building && current_screen.area() > BUILDING_SCREEN_AREA)
                 building = true;
        
            //store position of each nature to draw polygon
            for(unsigned j=0; j < (p_tot)-1 ; j++){
            points[j].x = Natural_Info[i].Natural_Position[j].p_lon();
            points[j].y = Natural_Info[i].Natural_Position[j].lat;
            }

            //draw nature that consists of only line
            if (Natural_Info[i].Natural_Position[0].lon != Natural_Info[i].Natural_Position[p_tot-1].lon
                    && Natural_Info[i].Natural_Position[0].lat != Natural_Info[i].Natural_Position[p_tot-1].lat){

                for(unsigned j=0; j < (p_tot)-1 ; j++){
                   float temp_lon = Natural_Info[i].Natural_Position[j+1].p_lon();
                   float temp_lat = Natural_Info[i].Natural_Position[j+1].lat;
                   
                   //draw nature only inside window.
                   if (max_screen_cordin.x > temp_lon || min_screen_cordin.x < temp_lon 
                        || max_screen_cordin.y > temp_lat || min_screen_cordin.y < temp_lat)    
                        drawline(Natural_Info[i].Natural_Position[j].p_lon(), Natural_Info[i].Natural_Position[j].lat, temp_lon, temp_lat);
               }
            }   
        
            //draw nature that is polygon shape
            else{
                if(!building && type != Island && type != Lake){
                    if(max_screen_cordin.x > points[0].x || min_screen_cordin.x < points[0].x 
                        || max_screen_cordin.y > points[0].y || min_screen_cordin.y < points[0].y
                        || max_screen_cordin.x > points[p_tot-1].x || min_screen_cordin.x < points[p_tot-1].x 
                        || max_screen_cordin.y > points[p_tot-1].y || min_screen_cordin.y < points[p_tot-1].y)
                        fillpoly(points, p_tot-1); //draw nature that is consists of lines 
                    
                    for(unsigned j=0; j < (p_tot)-1 ; j++){
                       float temp_lon = Natural_Info[i].Natural_Position[j+1].p_lon();
                       float temp_lat = Natural_Info[i].Natural_Position[j+1].lat;
                       setcolor(180,180,180);
                       setlinewidth(1);

                       if (max_screen_cordin.x > temp_lon || min_screen_cordin.x < temp_lon 
                            || max_screen_cordin.y > temp_lat || min_screen_cordin.y < temp_lat)               
                            drawline(Natural_Info[i].Natural_Position[j].p_lon(), Natural_Info[i].Natural_Position[j].lat, temp_lon, temp_lat); 
               }
            }  
        } 
    }
}

//draw outside of map with white space
void draw_outside_map(t_bound_box initial){
    current_screen = get_visible_world();
    setcolor(WHITE);
    float xmin = current_screen.bottom_left().x;
    float xmax = current_screen.top_right().x;
    float ymin = current_screen.bottom_left().y;
    float ymax = current_screen.top_right().y;
    float x = xmin;
    float y = ymin;  
 
    t_bound_box rect = t_bound_box(x, y , xmax-xmin, initial.bottom_left().y);
    fillrect(rect); // fill bot  with white space
    
    rect = t_bound_box(x,y, initial.bottom_left().x, ymax);
    fillrect(rect); // fill left 
    
    y = initial.top_right().y;
    rect = t_bound_box(x,y, xmax,ymax);
    fillrect(rect); // fill top
   
    x = initial.top_right().x;
    y = ymin;
    rect = t_bound_box(x,y, xmax, ymax);
    fillrect(rect); // fill right
}

/////////////LIST OF AREA OF INTERSET////////////////////////////////
/// bank, restaurant, fast_food, bank, cafe, subway, hospital//////
void draw_POI(){
    set_draw_mode (DRAW_NORMAL);
    setfontsize(7);
    t_bound_box current_screen= get_visible_world();     

    if (current_screen.area() < STREETNAME_SCREEN_AREA){
        for (unsigned i = 0; i < number.Points_Of_Interest; i++){
            float temp_lon = POI_Info[i].position.p_lon();      
            float temp_lat =POI_Info[i].position.lat;
            
            //draw point of interest with small square box
            t_point start_point = t_point(temp_lon,temp_lat);
            t_bound_box rect = t_bound_box(start_point, MINIMUM_ERROR_RANGE*(0.75), MINIMUM_ERROR_RANGE*(0.75));
            setcolor(235,97,78);  
            fillrect(rect);
            
            if (current_screen.area() < POI_SCREEN_AREA){
                setcolor(235,77,69); 
                drawtext (temp_lon, temp_lat-MINIMUM_ERROR_RANGE, POI_Info[i].name, MAXIMUM_ERROR_RANGE*2, MAXIMUM_ERROR_RANGE*2);
            }
        }
    }
}

void draw_highlited_intersection(){
    
    float size = 120;
    string type = "original";

    if (current_screen.area() < LOCATION_SYMBOL_SCREEN_AREA)
        size = 2;
    else if (current_screen.area() < STREETNAME_SCREEN_AREA)
        size = 5;
    else if (current_screen.area() < BUILDING_SCREEN_AREA)
        size = 10;
    else if (current_screen.area() < STREET_SCREEN_AREA)
        size = 20;
    else if (current_screen.area() < HIGHWAY_SCREEN_AREA)
        size = 50;
    
    if (!fastest_click_button && !closest_poi){
        for(unsigned i=0; i < highlighted_pos.size(); i++){
            LatLon temp = highlighted_pos[i];
            draw_current_location_symbol(temp.lon,temp.lat, size, type); 
        }
    }
    
    else{ // if fastest_click_button is clicked draw location symbol with "A" and "B" to find out start and end point
        for(unsigned i=0; i < highlighted_pos.size(); i++){
            char temp = 'A' + i;
            type = string(1, temp);
            draw_current_location_symbol(highlighted_pos[i].lon,highlighted_pos[i].lat, size, type); 
        }
    }
}

void draw_interest_symbol(){
    
    if (get_distance != INVALID_NUMBER){
        if(highlighted_pos.size() != 0){
        LatLon temp = highlighted_pos[0];
        find_interest(temp.lon,temp.lat,get_distance);
        }
    }
}

//highlight symbol with yellow box
void draw_highlighted_symbol(){
    if (symbol_click == true && interest_button == true){
        create_bigger_symbol_box(highlighted_symbol.lon, highlighted_symbol.lat); 
    }    
}

//draw intersection text at the bottom
void draw_intersection_text(){
    string name, pos, temp_string;
    
    stringstream convert;
    if (highlight_interesction == true){
        LatLon temp = highlighted_pos[0];
        name = highlighted_info.name;
        
        //store street name, position information into string
        convert << temp.lon;
        temp_string = convert.str();
        string pos = "Lon: " + temp_string  + " Lat: ";
        convert << temp.lat;
        temp_string = convert.str();
        pos = pos + temp_string;
        
        //get current screen info to draw white box at the bottom since box of size has to be same.
        current_screen = get_visible_world();
        t_point bot = current_screen.bottom_left();
        t_point top = current_screen.top_right();
        float width_screen = top.x - bot.x;
        float x = top.x - width_screen/(3.5);
        float y = bot.y;
        draw_bottom_box(x, y, width_screen);
        
        //draw intersection information text
        setcolor(BLACK);
        setfontsize(8);
        
        x = x + width_screen/8;
        y = y + width_screen/70;
        //drawtext(x,y,pos, width_screen, width_screen);
        y = y + width_screen/70;
        drawtext(x,y,name, width_screen, width_screen);
        y = y + width_screen/70;
        drawtext(x,y,"<Intersection Info>",width_screen , width_screen);
    }  
}

void draw_help(){ // draw help instruction
    
    if (help_button_click){
        current_screen = get_visible_world();
        t_bound_box rect = current_screen;
        draw_help_text(rect);
        help_draw_click = true;
    }
}

//if user type wrong input this box will show up for warning.
// if user click ok button it will disappear.
void draw_invalid_input(){
    if(invalid_input == true){
        invalid_input_box(current_screen.get_xcenter(), current_screen.get_ycenter(), current_screen.get_width());
    }
}

/**************************     NEW BUTTONS      *******************************/
void change_map_button(void (*load_ptr) (void)){
    string map_name;
    close_graphics();
    Map_Name = new_map_parser();
    load_ptr();
}

void show_interest_button (void(*load_ptr)(void)){
    if(highlighted_pos.size() != 0){
        //get distance from input
        Search_Text = "Type distance range to find point of interest(M): ";
        draw_searchtext();
        
        interest_button = true;
        load_ptr();
    }
}
// disable all of drawn graphic and set all vectors bools and others into the first set up.
void disable_interest_button (void(*load_ptr)(void)){
    get_distance = INVALID_NUMBER;
    interest_button = false;
    highlight_interesction = false;
    fastest_click_button = false;
    fastest_type_button = false;
    invalid_input = false;
    closest_poi = false;
    name_begin.clear();
    name_end.clear();
    type.clear();
    highlighted_pos.clear();
    Found_Path.clear();
    Search_Text = "Search...";
    draw_searchtext();     
    
    load_ptr();
}

void find_intersection_button( void(*load_ptr)(void)){
    closest_poi = true;
    Search_Text = "Input type of POI: ";
    draw_searchtext();
    load_ptr();
}

void find_street_button( void(*load_ptr)(void)){
    highlighted_street_id = find_street_parser();
    load_ptr();
}

void fastest_path_type_button( void(*load_ptr)(void)){
    fastest_path = true;
    Search_Text = "Input type of final destination (intersection or interest): ";
    draw_searchtext();
    load_ptr();
}

void fastest_path_click_button( void(*load_ptr)(void)){
    fastest_click_button = true;
    load_ptr();
}

void help_button( void(*load_ptr)(void)){
    help_button_click = true;
    if (help_draw_click){
        help_button_click = false;
        help_draw_click = false;
    }
    load_ptr();
}

/***************************    BUTTON EVENT    *******************************/
void act_on_button_press (float x, float y, t_event_buttonPressed event) {
    
    //after wrong input box show up, and user click okay button in the middle of screen
    //it will disable the box.
    if(invalid_input){
        float threshold = current_screen.get_width()/100;  
        t_bound_box temp = current_screen;
        temp.top_right().y = temp.top_right().y - threshold*2;
        if(temp.get_xcenter() - threshold*2 <= x && x <= temp.get_xcenter() + threshold*2 
                && temp.get_ycenter() - threshold <= y && y <= temp.get_ycenter() + threshold){
            invalid_input = false;
            fastest_type_found = false;
            load();
        }
    }
    
    else{
        hilight_intersection(x, y);
        hilight_symbol(x,y);
    }
}

void act_on_key_press(char c, int key){
    string temp;
    stringstream ss;
    
    if (c == 13 && interest_button){ // if interest button is clicked get distance from search box
        LatLon poistion = highlighted_pos[0];
        temp = Search_Text;
        temp.erase(0,50);
        ss << temp;
        ss >> get_distance;
        
        if (0 < get_distance){
            find_interest(poistion.lon, poistion.lat, get_distance);
            interest_button = true;
        }
        
        else{
            get_distance = INVALID_NUMBER;
            cout << "Invalid input is typed." << endl;
        }
         
    }
    
    if (c == 13 && closest_poi){
        auto startTime_D = chrono::high_resolution_clock::now();
        float distance = 0;
        float final_distance = 9999;
        vector <unsigned> final_path;
        unsigned poi_id;
        double final_time, new_time;
        final_time = 9999;
        temp = Search_Text;
        temp.erase(0,19);
        unsigned end = 0;
        unsigned start = 0;
        
       for(unsigned i = 0; i < number.Intersections; i++){
            if (Intersection_Info[i].pos.p_lon() == highlighted_pos[0].lon && Intersection_Info[i].pos.lat == highlighted_pos[0].lat)
            start = i;
       }
        for (unsigned i =0; i < number.Points_Of_Interest; i++){
            distance = sqrt((highlighted_pos[0].lon - POI_Info[i].position.p_lon())*(highlighted_pos[0].lon - POI_Info[i].position.p_lon()) + (highlighted_pos[0].lat - POI_Info[i].position.lat)*(highlighted_pos[0].lat - POI_Info[i].position.lat))*(1/REAL_DISTANCE_TO_MAP);
            if (POI_Info[i].type == temp && distance < final_distance){
                poi_id = find_closest_intersection(POI_Info[i].position);
                Found_Path = find_path_between_intersections(start,poi_id);
                new_time = compute_path_travel_time(Found_Path);
                    if(new_time < final_time){
                        final_time = new_time;
                        final_path = Found_Path;
                        end = poi_id;
                        name_end = POI_Info[i].name;
                        type = POI_Info[i].type;
                        final_distance = distance;
                    }
                
            } 
        }
        
        Found_Path = final_path;
        Destination_Id = end;
        Start_Id = start;
        passed = true;
        load();
    auto currentTime_D = chrono::high_resolution_clock::now();
    auto wallClock_D =  chrono::duration_cast<chrono::duration<double>> (currentTime_D - startTime_D);
    cout << wallClock_D.count() << endl;
    }
    
    /***************************************************************************************************/
    // keyboard input to get information that need to find shortest path
    // there are 6 bools:
    // fastest_path: TRUE if window button called fastest_path is pushed
    // fastest_path_type: TRUE if got type of destination
    // intersection1_street1: TRUE if got the first street for the start intersection
    // intersection1_street2: TRUE if got the second street for the start intersection
    // intersection2_street1: TRUE if got the first street for the end intersection
    // got_all_info: TRUE if got all information successfully
    
    //If enter is pressed and fastest_path_button is pressed
    if (c == 13 && fastest_path && !fastest_path_type){ 
        
        temp = Search_Text;
        temp.erase(0,60); // get string only contain type of destination
        type = temp;  
        
        //Getting the type of fastest_path from user input
        if(type == "intersection" || type == "interest"){//go to next step only if type is valid
            fastest_path_type = true;
            Search_Text = "Input name of street 1 for the START intersection: ";
            draw_searchtext();
            passed = false;
            }
        
        else
            cout << "Invalid input is typed!" << endl;
    }
    
    //If 'tab' key is pressed for completion of the 1.1 street
    if (c == 9 && fastest_path_type && !intersection1_street1)
    {
        //output the string in the search box
        Search_Text = complete_search_box_input(Search_Text,52,"intersection");
    }
    //Getting the name of the 1.1 street from user input
    if(c == 13 && fastest_path_type && !intersection1_street1)
    {
        if (passed){
            temp = Search_Text.substr(52,Search_Text.size()-52);
            name_begin = temp;
            
            if(name_begin != ""){
                intersection1_street1 = true;                 
                name_begin.append(" & ");
                
                //Ask for the 1.2 street
                Search_Text = "Input name of street 2 for the START intersection: ";
                draw_searchtext();
            }
        }
        passed = true;        
    }
    
    //If 'tab' key is pressed for completion of the 1.2 street
    if (c == 9 && intersection1_street1 && !intersection1_street2)
    {
        //output the string in the search box
        Search_Text = complete_search_box_input(Search_Text,52,"intersection");
        draw_searchtext();        
    }
    //Getting the name of the 1.2 street from user input
    if (c == 13 && intersection1_street1 && !intersection1_street2)
    {
        if (!passed)
        {
            temp = Search_Text.substr(52,Search_Text.size()-52); 
            
            if(temp != ""){
                intersection1_street2 = true;                
                name_begin.append(temp);
                name_begin = fix_blank_spaces(name_begin);
                              
                if(type == "intersection"){ // if type is intersection ask for the 2.1 street
                    Search_Text = "Input name of street 1 for the END intersection: ";
                    draw_searchtext();    
                }
                if (type == "interest"){ //if type is interest store interest name
                    Search_Text = "Input end interest name: ";
                    draw_searchtext();
                }                
            }
        }
        passed = false;        
    }
    
    //If 'tab' key is pressed for completion of point of interest name
    if (c == 9 && intersection1_street2 && (type == "interest"))
        Search_Text = complete_search_box_input(Search_Text,26,type);
    //Getting end interest name If the type was interest
    if(c == 13 && intersection1_street2 && (type == "interest")){ 
        if(passed){            
                temp = Search_Text.substr(26, Search_Text.size()-26);
                name_end = temp;
                name_end = fix_blank_spaces(name_end);
        }        
        if(name_end != "") // all information is founded.
            got_all_info = true;
        
        passed = true;
    }
    
    //If 'tab' key is pressed for completion of the 2.1 street
    if (c == 9 && intersection1_street2 && (type == "intersection") && !intersection2_street1)
    {
        //output the string in the search box
        Search_Text = complete_search_box_input(Search_Text,50,type);
    }
    //Getting the name of 2.1 street from user input if the type was intersection
    if(c == 13 && intersection1_street2 && (type == "intersection") && !intersection2_street1)
    { 
        if (passed)
        {
            temp = Search_Text.substr(50, Search_Text.size()-50);
            name_end = temp;
            
            if(name_end != ""){
                intersection2_street1 = true;
                name_end.append(" & ");

                //Ask for the 2.2 street
                Search_Text = "Input name of street 2 for the END intersection: ";
                draw_searchtext();
            }
        }
        passed = true;
    }
    
     //If 'tab' key is pressed for completion of the 2.2 street
    if (c == 9 && intersection2_street1 && (type == "intersection") && !got_all_info)
    {
        //output the string in the search box
        Search_Text = complete_search_box_input(Search_Text,50,type);
    }      
    //Getting the name of 2.2 street from user input if the type was intersection
    if(c == 13 && intersection2_street1 && (type == "intersection") && !got_all_info)
    { 
        if (!passed)
        {
            temp = Search_Text.substr(50, Search_Text.size()-50);
            if (temp != "")
            {
                name_end.append(temp);
                name_end = fix_blank_spaces(name_end);
                //all information was founded.
                got_all_info = true;                
            }            
        }
        passed = false;        
    }

    if (c == 13 && got_all_info){ // if all information is stored, set every fastest_path_bool as false
        fastest_path = false;
        fastest_path_type = false;
        got_all_info = false;
        intersection1_street1 = false;
        intersection1_street2 = false;
        intersection2_street1 = false;
        fastest_type_found = true;
        passed = false;
        
        Search_Text = "Search...";
        draw_searchtext();
        load();
    }
}   

void hilight_symbol(float x, float y){
    string type;
    bool found = false;
    
    //if interest button is clicked it is true.
    if (interest_button){
        for (unsigned i = 0; i < number.Points_Of_Interest ; i++){
            LatLon interest = POI_Info[i].position;
            
            //draw only inside of radius range
            if (x - MINIMUM_ERROR_RANGE*4 <= interest.p_lon() && interest.p_lon() <= x + MINIMUM_ERROR_RANGE*4 
                    && y - MINIMUM_ERROR_RANGE*4 <= interest.lat && interest.lat <= y + MINIMUM_ERROR_RANGE*4){

               highlighted_symbol.lon = interest.p_lon();
               highlighted_symbol.lat = interest.lat;
               
               load();//clear and redraw screen;
               create_bigger_symbol_box(highlighted_symbol.lon, highlighted_symbol.lat);  

               //print information of symbol.
               name_end = POI_Info[i].name;
               type = POI_Info[i].type;
               cout << "<Point of POI Info>" << endl;
               cout << "Type: " << type << endl;
               cout << "Name: " << name_end << endl;
               
               unsigned start = 0;
               for(unsigned i = 0; i < number.Intersections; i++){
                    if (Intersection_Info[i].pos.p_lon() == highlighted_pos[0].lon && Intersection_Info[i].pos.lat == highlighted_pos[0].lat)
                    start = i;
               }
                unsigned int_id = find_closest_intersection (POI_Info[i].position);
                Found_Path = find_path_between_intersections (start, int_id);
                draw_fastest_path(Found_Path);
                print_fastest_path(Found_Path, start, int_id);
            
               found = true;
               break;
            }
        }
        //print subway information since it has different data structure
        if (!found){
            for (unsigned i = 0; i < Subway_Stations.size(); i++){
            LatLon current_pos = Subway_Stations[i].pos;            
                if(x - MINIMUM_ERROR_RANGE*2 <= current_pos.p_lon() && current_pos.p_lon() <= x + MINIMUM_ERROR_RANGE*2 
                        && y - MINIMUM_ERROR_RANGE*2 <= current_pos.lat && current_pos.lat <= y + MINIMUM_ERROR_RANGE*2){
                    load();//clear and redraw screen;
                    create_bigger_symbol_box(current_pos.p_lon(),current_pos.lat);                                     
                    
                    cout << "<Point of POI Info>" << endl;
                    cout <<"Type: subway staion" << type << endl <<endl;      
                    break;
                }
            }  
        }
    }
}

/***************************    BUTTON EVENT SUPPORdraw_highlited_intersectionT  *************************/
void hilight_intersection(float x, float y){
    bool found = false;
    
    LatLon point;
    point.lon = x;
    point.lat = y;
    if (!interest_button){
        for (unsigned i = 0; i < number.Intersections ; i++){
           //highlight intersection that is in range, if it is not close to intersection doesn't highlight anything
           if(fabs(x) - MINIMUM_ERROR_RANGE*2 <= fabs(Intersection_Info[i].pos.p_lon()) && fabs(Intersection_Info[i].pos.p_lon()) <= fabs(x) + MINIMUM_ERROR_RANGE*2){
               if(y -MINIMUM_ERROR_RANGE*2 <= Intersection_Info[i].pos.lat && Intersection_Info[i].pos.lat <= y + MINIMUM_ERROR_RANGE*2){
                   if(!fastest_click_button){
                       highlighted_pos.clear();
                   }
                   
                   point.lon = Intersection_Info[i].pos.p_lon();
                   point.lat = Intersection_Info[i].pos.lat;
                   highlighted_pos.push_back(point);
                   highlight_interesction = true;
                   highlighted_info.name = Intersection_Info[i].name;
                   cout << "Intersection Name:"<< Intersection_Info[i].name << endl;
                   cout << "Intersection Id: " << i << endl;
                   cout << "Coordinates: (x:" << point.lon << " y:" << point.lat << ")" <<endl <<endl;
                   found = true;
                   symbol_click = true;
                   break;
               }
           }     
        }
        if (found == false)
            cout << "Invalid intersection is clicked (x:" << point.lon << "  y:" << point.lat <<")" <<endl;
        else{
            load(); // clear and redraw map 
        }
    }
}

void hilight_street (){
    if (highlighted_street_id.size() != 0){  
        string type = "highlight";
        vector<unsigned> street_id = highlighted_street_id;
        
        for(unsigned i = 0; i < highlighted_street_id.size(); i++){
            unsigned id = highlighted_street_id[i];
            
            for (unsigned i = 0; i < Street_Info[id].Street_Segments.size(); i++){
                unsigned segement_id = Street_Info[id].Street_Segments[i];
                unsigned intersection_id_from = Street_Segment_Info[segement_id].from;
                unsigned intersection_id_to = Street_Segment_Info[segement_id].to;

                LatLon from = Intersection_Info[intersection_id_from].pos;
                LatLon to = Intersection_Info[intersection_id_to].pos;

                draw_street(to.p_lon(), to.lat, from.p_lon(), from.lat ,type);    
            }
        }
    }
}


/***************************    DRAWING SUPPORT  ******************************/
void find_interest (float x, float y, float distance_1){
    // convert into map ratio
    distance_1 = distance_1 * REAL_DISTANCE_TO_MAP; 
    LatLon start_point, current_pos;
    start_point.lon = x;
    start_point.lat = y;
    
    float xmin = x - distance_1;
    float xmax = x + distance_1;
    float ymin = y - distance_1;
    float ymax = y + distance_1;
    
    if (interest_button){       
        for(unsigned i = 0; i < number.Points_Of_Interest ; i++){
            current_pos = POI_Info[i].position;
            double x = start_point.lon - current_pos.p_lon();
            double y = start_point.lat - current_pos.lat;
            double distance_2 = sqrt((x*x)+(y*y));

            if(distance_2 <= distance_1){
                string interest_type = POI_Info[i].type;
                if (interest_type == "cafe")
                    draw_cafe_symbol(current_pos.p_lon(),current_pos.lat);

                else if (interest_type == "restaurant")
                    draw_restaurant_symbol(current_pos.p_lon(),current_pos.lat);

                else if (interest_type == "hospital")
                    draw_hospital_symbol(current_pos.p_lon(),current_pos.lat);

                else if (interest_type == "fast_food")
                    draw_fastfood_symbol(current_pos.p_lon(),current_pos.lat);

                else if (interest_type == "bank")
                    draw_bank_symbol(current_pos.p_lon(),current_pos.lat); 
            }    
        }
        
        setlinewidth(2);
        setcolor(50,50,50);
        setlinestyle(DASHED);
        drawarc (x,y,distance_1,0, 360);
        // subway information
        for (unsigned i = 0; i < Subway_Stations.size(); i++)                                             
        {
            current_pos = Subway_Stations[i].pos;            
            if(current_pos.p_lon() > xmin && current_pos.p_lon() <xmax &&
                current_pos.lat > ymin && current_pos.lat < ymax)
                draw_subway_symbol(current_pos.p_lon(),current_pos.lat);                                     
        }
    
    }
}

// draw street that with two line, one is street and other one is outline
void draw_street(float to_lon, float to_lat, float from_lon, float from_lat, string type){     
    if (type == "oneway")
        setlinestyle(DASHED);
    
    setlinewidth(6);
    setcolor(180,180,180);
    drawline(to_lon, to_lat, from_lon, from_lat);
    
    if (type == "highway")
        setcolor(255,195,69);
    else if (type == "main")
        setcolor(255,253,139);
    else if (type == "highlight")
        setcolor(255,102,102);
    else
        setcolor(WHITE);
    
    setlinewidth(5);
    drawline(to_lon, to_lat, from_lon, from_lat);
    
    setlinestyle(SOLID);
}



/***********************    MATHEMATICS SUPPORT FUNCTIONS   ********************/

void get_min_max(float& xmin, float& xmax, float& ymin, float& ymax){
    LatLon pos;
    float lon, lat;
    
    pos = Intersection_Info[0].pos;
    xmin = xmax = pos.p_lon();
    ymin = ymax = pos.lat ;
    //Iterate over intersections to find min and max coordinates
    for(unsigned i = 1; i < number.Intersections; i++){
        pos = Intersection_Info[i].pos;
        lon = pos.p_lon();
        lat = pos.lat;
        
        if(lon < xmin)
            xmin = lon;
        if(lon > xmax)
            xmax = lon;
        
        if(lat < ymin)
            ymin = lat;
        if(lat > ymax)
            ymax = lat;
    }
}

float get_cosine(){
    float y_min, y_max, lat, cosine;
    
    lat = Intersection_Info[0].pos.lat;
    y_min = y_max = lat ;
    
    for(unsigned i = 1; i < number.Intersections; i++){
        lat = Intersection_Info[0].pos.lat;
        if(lat < y_min)
            y_min = lat;
        if(lat > y_max)
            y_max = lat;
    }
    
    cosine = cos((y_min + y_max)*DEG_TO_RAD/2);
    return cosine;
}

float LatLon::p_lon(){
    return (*this).lon * cosine;
}

//draw fastest path with given vector that contains street segments IDs.
void draw_fastest_path(vector<unsigned> result){
    for (unsigned i = 0; i < result.size(); i++){
        LatLon from = Intersection_Info[Street_Segment_Info[result[i]].from].pos;
        LatLon to = Intersection_Info[Street_Segment_Info[result[i]].to].pos;
          
        if (Street_Segment_Info[result[i]].curvePointCount != 0){//if street segment has curve points
                int size = Street_Segment_Info[result[i]].curvePointCount;
                vector<LatLon> curve = Street_Segment_Info[result[i]].curvePointPos;
                //draw from street_segment.from to first curve point
                draw_street(from.p_lon(), from.lat, curve[0].p_lon(), curve[0].lat, "highlight");
                //draw curve from point to point
                for(unsigned j = 0; j < Street_Segment_Info[result[i]].curvePointCount - 1; j++){
                    draw_street(curve[j].p_lon(), curve[j].lat, curve[j+1].p_lon(), curve[j+1].lat, "highlight");
                }
                //draw from last curve point to street_segment.to
                draw_street(curve[size-1].p_lon(), curve[size-1].lat, to.p_lon(), to.lat, "highlight");
            }
            else{            
                draw_street(from.p_lon(), from.lat, to.p_lon(), to.lat, "highlight");  
            }
    }
}
// print all direction of fastest path including total travel time
void print_fastest_path (vector<unsigned> result, unsigned start, unsigned end){
    t_bound_box rect = current_screen;
    float width = (rect.top_right().x - rect.bottom_left().x)/2;
    float error_range = width/30;
    int count = 0;
    string street_name;
    stringstream ss;
    string temp;
    
    //draw box for printing path
    print_help_fastest_path(rect);
    rect.bottom_left().x = rect.bottom_left().x + width/4;
    rect.top_right().y = rect.top_right().y - width/6;
    
    //print start point intersection info
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range ,Intersection_Info[start].name, width, width);
    
    setfontsize(9);
    setcolor(WHITE);
    
    LatLon from_next, from_cur;
    LatLon to_next, to_cur;
    //draw first segment street direction and name
    for (unsigned i = 0; i < result.size()-1; i++){
            // find out which way is FROM and TO in street segments. all segments direction is set up randomly
            from_next = Intersection_Info[Street_Segment_Info[result[i+1]].from].pos;
            to_next = Intersection_Info[Street_Segment_Info[result[i+1]].to].pos;
            from_cur = Intersection_Info[Street_Segment_Info[result[i]].from].pos;
            to_cur = Intersection_Info[Street_Segment_Info[result[i]].to].pos;
            if (from_next.lon != to_cur.lon && from_next.lat != to_cur.lat && to_next.lon != to_cur.lon && to_next.lat != to_cur.lat){
                to_cur = Intersection_Info[Street_Segment_Info[result[i]].from].pos;
                from_cur =  Intersection_Info[Street_Segment_Info[result[i]].to].pos;
            }
            if (to_cur.lon != from_next.lon && to_cur.lat != from_next.lat){
                to_next = Intersection_Info[Street_Segment_Info[result[i+1]].from].pos;
                from_next = Intersection_Info[Street_Segment_Info[result[i+1]].to].pos;
            }
        
        //calculate degree of first street segments and next segments to find out which direction need to be turned.
        float degree_cur = get_degree(from_cur, to_cur);
        float degree_next = get_degree(from_next, to_next);
        float degree = degree_cur - degree_next;
        string direction = get_direction(degree_cur);
        const int threshold = 20; 
        
        if (i==0){//in first street segments no turn direction is required so print which way need to head such as WEST, NORTH.
            street_name = Street_Info[Street_Segment_Info[result[i]].streetID].Name;
            temp = "1. Go " + direction + " on " + street_name;
            count = 2;
            
            drawtext(rect.bottom_left().x, rect.top_right().y - error_range*(count), temp, width, width);
            ss.str("");
        }
        
        if (street_name != Street_Info[Street_Segment_Info[result[i+1]].streetID].Name){
            street_name = Street_Info[Street_Segment_Info[result[i+1]].streetID].Name;
            ss << count << ". ";
            ss >> temp; //find out turn direction from angles difference
            if ((degree > threshold && degree < 180 - threshold) || degree < -180 - threshold)
                direction = "Turn right";
            else if ((degree < 0 - threshold && degree > -180 + threshold) || degree > 180 + threshold)
                direction = "Turn left";
            else if (degree >= 0 - threshold && degree <= threshold)
                direction = "Go straight";
            else
                direction = "U-turn";

            temp = temp + direction + " on " + street_name;
            drawtext(rect.bottom_left().x, rect.top_right().y - error_range*(count+1), temp, width, width);
            count++;
        }
        else{
            
        }

        ss.str("");
    }
    //Print destination intersection info and total time at the end
    setfontsize(11);
    setcolor(PINK);
    
    if(interest_button || closest_poi)
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*(count+2) ,name_end, width, width);
    else
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*(count+2) ,Intersection_Info[end].name, width, width);    
    
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*(count+3) ,"________________________________", width, width);
    setcolor(PINK);
    double total_time = compute_path_travel_time(result);
    string print_time = "Total travel time: ";
    int intpart = (int)total_time;
    double decimal = int((total_time - intpart)*60); // get decimal point and convert to second
    ss <<  int(total_time);
    ss >> temp;
    
    stringstream ss_1;
    if(total_time >= 1)
        print_time = print_time + temp + "min";
    ss.str(""); // clear stringstream
    ss_1 << int(decimal);
    ss_1 >> temp;
    print_time = print_time + " " + temp + "sec";
    
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*(count+4) ,print_time, width, width); 

    
}

void draw_fastest_path_in_load_map(){
    unsigned start = INVALID_NUMBER;
    unsigned destination = INVALID_NUMBER;
    
        auto startTime_D = chrono::high_resolution_clock::now();

    if (closest_poi && passed){
       draw_fastest_path(Found_Path);
       print_fastest_path(Found_Path, Start_Id, Destination_Id);

       highlighted_pos[1].lon = Intersection_Info[Destination_Id].pos.p_lon();
       highlighted_pos[1].lat = Intersection_Info[Destination_Id].pos.lat; 
       draw_highlited_intersection();
       
       Search_Text = "Search...";
       draw_searchtext();  
    }
            
           
    //if fastest_click_button is clicked draw fastest path with two clicked intersections
    if (highlighted_pos.size()>1 && fastest_click_button){
        for(unsigned i = 0; i < number.Intersections; i++){
            if (Intersection_Info[i].pos.p_lon() == highlighted_pos[0].lon && Intersection_Info[i].pos.lat == highlighted_pos[0].lat)
                start = i;
            if (Intersection_Info[i].pos.p_lon() == highlighted_pos[1].lon && Intersection_Info[i].pos.lat == highlighted_pos[1].lat)
                destination = i;
            if(start != INVALID_NUMBER && destination != INVALID_NUMBER){      
                break;
            }
        }
        Found_Path = find_path_between_intersections (start, destination);
        draw_fastest_path(Found_Path);
        print_fastest_path(Found_Path, start, destination);
    }

    
    if (fastest_type_found) {
        for (unsigned i = 0; i < number.Intersections; i++){ // find intersection Ids that match user inputs
            if(name_begin == Intersection_Info[i].name)
                start = i;
            if(name_end == Intersection_Info[i].name)
                destination = i;
            if(start != INVALID_NUMBER && destination != INVALID_NUMBER)
                break;
        }  
        
        if(start == INVALID_NUMBER){
        cout << "Start intersection, " << name_begin << ", could not be found. Click on \"Find fastest(Type)\" button again."  << endl;
        }
        
        if (type == "intersection"){
            if(destination == INVALID_NUMBER){
                cout << "End intersection, " << name_end << ", could not be found. Click on \"Find fastest(Type)\" button again." << endl; 
                invalid_input = true;
            }
            
            if (start != INVALID_NUMBER && destination != INVALID_NUMBER){
                if (!passed) // prevent from calculating everytime load map
                    Found_Path = find_path_between_intersections (start, destination);
                if (Found_Path.size() > 0){
                    draw_fastest_path(Found_Path);
                    print_fastest_path(Found_Path, start, destination);
                    passed = true;
                }
            }
        }
        
        
        else if (type == "interest"){ // store all point of interest IDs with same name in vector
            if(start != INVALID_NUMBER){
                if (!passed) // prevent from calculating everytime load map
                    Found_Path = find_path_to_point_of_interest(start, name_end);
                if (Found_Path.size() > 0){
                    draw_fastest_path(Found_Path);
                    print_fastest_path(Found_Path, start, Destination_Id);
                             
                    fastest_click_button = true;
                    highlighted_pos.clear();
                    LatLon point = Intersection_Info[start].pos;
                    point.lon = point.p_lon();
                    highlighted_pos.push_back(point);
                    point = Intersection_Info[Destination_Id].pos;
                    point.lon = point.p_lon();
                    highlighted_pos.push_back(point);
                    draw_highlited_intersection();
                    
                    
                    passed = true;
                
                
                }
                else
                    invalid_input = true;
            }
            else
                invalid_input = true;
        }
    }
            auto currentTime_D = chrono::high_resolution_clock::now();
    auto wallClock_D =  chrono::duration_cast<chrono::duration<double>> (currentTime_D - startTime_D);
    cout << wallClock_D.count() << endl;
    
}

string get_direction(float degree){
    string direction;
    if (degree >= 350 && degree <= 10){
        direction = "East";
    }
    else if (degree > 10 && degree < 80){
        direction = "North-East";
    }
    else if (degree >= 80 && degree <= 100){
        direction = "North";
    }
    else if (degree > 100 && degree < 170){
        direction = "North-West";
    }
    else if (degree >= 170 && degree <= 190){
        direction = "West";
    }
    else if (degree > 190 && degree < 260){
        direction = "South-West";
    }
    else if (degree >= 260 && degree <= 280){
        direction = "South";
    }
    else if (degree > 280 && degree < 350){
        direction = "South-East";
    }
    return direction;
}
