
#include "m1.h"
#include "m3.h"

//************************* GLOBAL VARIBLES  ***************************
//global variable of unordered_maps and vectors store information

struct Number number;
unordered_multimap<string, unsigned> Map_street; // unordered_multimap contains street name and street id
multimap<unsigned long long, NodeInfo> OSMNodes; //Contains all OSMNodes but they are easy to access by their ID
unordered_map<string, unsigned> Middle_Street_Segment;                          //M3

//Street - Street Segment - Intersection Info Containers
vector<StreetSegmentInfo_2> Street_Segment_Info; 
vector<IntersectionInfo> Intersection_Info;                                    
vector<StreetInfo>Street_Info;
vector<Natural>Natural_Info;
vector<POI>POI_Info;
vector<NodeInfo> Subway_Stations;
vector<NodeInfo> Secondary_Road_Nodes;
vector<NodeInfo> Highway_Nodes;

//m3
unordered_multimap <string, unsigned> POI_Name_Map;
vector <aNode> Node_Info;
vector <aNode> Node_Map [NUM_THREADS];
//*************************  FUNCTIONS  ********************************
//load the map
bool load_map(string map_name) {

    bool load_success = loadStreetsDatabaseBIN(map_name);    
      
    //Number of Intersection - Street - Street segment - Ponts of POI
    load_number();
    //Street
    load_street();
    //Intersection
    load_intersection();
    //Street Segment
    load_street_segments();
    
    //erase duplicates in street intersections
    for(unsigned i=0; i < number.Streets; i++){
        vector<unsigned> * intersections = &Street_Info[i].Intersections;
        sort( intersections->begin(), intersections->end() );
        intersections->erase( unique( intersections->begin(), intersections->end() ), intersections->end() );
    } 
 
    load_street_segment_travel_time();
    
    //Natural
    load_natural();    
    //Points of POI
    load_POI();    
    //OSM Nodes
    load_OSM_Nodes();    
    //OSM way information (For secondary roads)
    load_roads();
    //Bus Stops and Subway stations
    load_transit();
    
    //m3
    load_node ();
    
    return load_success;
}

//close the map
void close_map() {
    closeStreetDatabase();
    
    // destroy/clear any data structures you created in load_map
    Map_street.clear();                                //street name -> street id

    //Street - Street Segment - Intersection Info Containers
    Street_Segment_Info.clear(); 
    Intersection_Info.clear();                                    
    Street_Info.clear();
    Natural_Info.clear();
}

//function to return street id(s) for a street name
//return a 0-length vector if no street with this name exists.
vector<unsigned> find_street_ids_from_name(string street_name){
    vector<unsigned> street_id_;
    vector<unsigned> temp;
    temp.push_back(100);
    
    if (street_name == "<unknown>") // street name doesn't exist return 0-length vector;
        return street_id_; 
    
    auto range = Map_street.equal_range(street_name); //find same street_name in Map_Street and return iterator
    
    for(auto it = range.first; it != range.second; it++){
        street_id_.push_back(it->second); //store street ids in new vector
    }
    
    return street_id_;
}  

//function to return the street segments for a given intersection 
vector<unsigned> find_intersection_street_segments(unsigned intersection_id){
    
    if (intersection_id > number.Intersections || intersection_id < 0 )
        cout<< "Error: Intersection_ID is out of range!" << endl; // if intersection id is out of range it print error.
    return Intersection_Info[intersection_id].Street_Segments; // return global variable vector that is already made in load_map
    
}

//function to return street names at an intersection (include duplicate street names in returned vector)
vector<string> find_intersection_street_names(unsigned intersection_id){
     vector<string> street_name_;
    //error message
    if (intersection_id > number.Intersections || intersection_id < 0 ){
        cout<< "Error: Intersection_ID is out of range!" << endl;
        return street_name_; // if intersection id is out of range it print error and return 0 vector.
    }
     
    vector<unsigned> street_segments_ = find_intersection_street_segments(intersection_id); // return street segments from given intersection id
    
    //add street names from street segments
    for(vector<unsigned>::iterator it = street_segments_.begin(); it != street_segments_.end(); it++){
        street_name_.push_back(Street_Info[Street_Segment_Info[*it].streetID].Name); // use iterator for loop to store street name vector into new vector
    }
   
    return street_name_;
}

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

//basic idea: test for all 2nd intersections of street segments. If any of them is id2, true.
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2){
    //error message
    if (intersection_id1 > number.Intersections || intersection_id1 < 0 || intersection_id2 > number.Intersections || intersection_id2 < 0){
        cout<< "Error: Intersection_ID is out of range!" << endl;
        return false;
    }
    //we may need to check for out of bound
    
    //check if they are the same intersections
    if(intersection_id1 == intersection_id2)
        return true;
    
    vector<unsigned> segments = find_intersection_street_segments(intersection_id1);
    
    for(auto iter = segments.begin(); iter != segments.end(); iter++){
        StreetSegmentInfo_2 temp = Street_Segment_Info[*iter];
        
        //case 1: one-way or two-way
        if(temp.to == intersection_id2)
            return true;
        
        //case 2: only two-way
        if(temp.from == intersection_id2 && !temp.oneWay)
            return true;
    }
    
    return false;
}

//find all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
vector<unsigned> find_adjacent_intersections(unsigned intersection_id){
    vector<unsigned> result;
    //error message
    if (intersection_id > number.Intersections || intersection_id < 0 ){
        cout<< "Error: Intersection_ID is out of range!" << endl;
        return result;
    }
    
    vector<unsigned> segments = find_intersection_street_segments(intersection_id);
    
    //check if intersection 2 matches 2nd intersections of street segments (also check for one-way)
    for(auto it = segments.begin(); it != segments.end(); it++){
        StreetSegmentInfo_2 info  = Street_Segment_Info[*it];
        
        unsigned id2;
        if(info.from == intersection_id)
            id2 = info.to;
        else
            id2 = info.from;
        
        if(are_directly_connected(intersection_id, id2))
            result.push_back(id2);
    }
    
    //erase duplicates
    sort( result.begin(), result.end() );
    result.erase( unique( result.begin(), result.end() ), result.end() );

    return result;
}

//for a given street, return all the street segments
vector<unsigned> find_street_street_segments(unsigned street_id){
    //check for street bound
    if (street_id > number.Streets || street_id < 0 )
        cout<< "Error: Street_ID is out of range!" << endl;
    
    
    return Street_Info[street_id].Street_Segments;    
}

//for a given street, find all the intersections
vector<unsigned> find_all_street_intersections(unsigned street_id){
    //check for street bound
    if (street_id > number.Streets || street_id < 0 )
        cout<< "Error: Street_ID is out of range!" << endl;
    
    vector<unsigned> intersections = Street_Info[street_id].Intersections;
    
    return intersections;
}

//function to return all intersection ids for two intersecting streets
//this function will typically return one intersection id between two street names
//but duplicate street names are allowed, so more than 1 intersection id may exist for 2 street names
vector<unsigned> find_intersection_ids_from_street_names(string street_name1, string street_name2){
    vector<unsigned> id;
    
    if (street_name1 == "<unknown>" || street_name2 == "<unknown>") // if either of street name doesn't exist return 0-length vector;
        return id; 
    
    vector<unsigned> street_id1, street_id2, intersection_id1, intersection_id2;
    //Get vector of street id of street name
    street_id1 = find_street_ids_from_name(street_name1);
    street_id2 = find_street_ids_from_name(street_name2);
           
    //get all street intersections of both streets
    for(auto it = street_id1.begin(); it != street_id1.end(); it++){
        vector<unsigned> temp_id = find_all_street_intersections(*it);
        intersection_id1.insert(intersection_id1.end(), temp_id.begin(), temp_id.end());
    }
    for(auto it = street_id2.begin(); it != street_id2.end(); it++){
        vector<unsigned> temp_id = find_all_street_intersections(*it);
        intersection_id2.insert(intersection_id2.end(), temp_id.begin(), temp_id.end());
    }
    
    //gather the same intersections 
    for(auto it1 = intersection_id1.begin(); it1 != intersection_id1.end(); it1++)
        for(auto it2 = intersection_id2.begin(); it2 != intersection_id2.end(); it2++){
            if(*it2 == *it1){
                id.push_back(*it2);
                break;
            }
        }    
    return id;
}

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2)
{
    double distancey, distancex;
    double lon1,lon2,lat1,lat2;  
    //Converting degrees to radians
    lon1 = point1.lon * DEG_TO_RAD;
    lat1 = point1.lat * DEG_TO_RAD;
    lon2 = point2.lon * DEG_TO_RAD;
    lat2 = point2.lat * DEG_TO_RAD;
    
    double avg_lat = (lat1 + lat2)/2;
    //Transforming longitude and latitude to rectangular coordinates (x,y)
    double cosine = cos_core(avg_lat);
    double x1 = lon1 * cosine; //x1
    double x2 = lon2 * cosine; //x2
      
    distancey = lat2 - lat1;
    distancex = x2 - x1;    
    
    return (EARTH_RADIUS_IN_METERS * sqrt((distancey*distancey)+(distancex*distancex)));
}

//find the length of a given street segment. 
//This function calculates the distance between all the curve points and add them up including
//the distances between the starting and final curve points with the "from" and 
//the "to" intersection respectively.(The function assumes that the curves points are 
//ordered from the "from" intersection to the "to" intersection) 
double find_street_segment_length(unsigned street_segment_id) {
    //check for street segment bound
    if (street_segment_id > number.Street_Segments || street_segment_id < 0 ){
        cout<< "Error: Street_segment ID is out of range!" << endl;
        return 0;
    }
    double distance = 0;
    //First get the info of the street segment
    StreetSegmentInfo_2 streetSegment = Street_Segment_Info[street_segment_id]; 
    unsigned curve_points_num = streetSegment.curvePointCount;
    LatLon point_1 = Intersection_Info[streetSegment.from].pos;
    LatLon point_2;
    
    //If the street segment is straight
    if (curve_points_num == 0)     
        return distance = find_distance_between_two_points(Intersection_Info[streetSegment.from].pos, Intersection_Info[streetSegment.to].pos);
    
    //If the street segment has curve points
    for (int i=0; i < (int)curve_points_num ; i++)
    {
        LatLon temp = getStreetSegmentCurvePoint(street_segment_id, i);
        //Get the position of each curve point
        point_2 = temp;
        //Compute distance
        distance += find_distance_between_two_points(point_1,point_2);        
        //Make the point 2 the point 1 to calculate next distance
        point_1 = point_2;
    }
    //Add the last distance (distance between the last curve point and the "to" intersection)
    point_1 = Intersection_Info[streetSegment.to].pos;
    point_2 = getStreetSegmentCurvePoint(street_segment_id, curve_points_num-1);
    
    return distance += find_distance_between_two_points(point_1,point_2);  
}

//find the length of a whole street
double find_street_length(unsigned street_id){
    double length = 0;
    //check for street bound
    if (street_id > number.Streets || street_id < 0 ){
        cout<< "Error: Street_ID is out of range!" << endl;
        return 0;
    }
    
    //Get the street segments
    vector<unsigned> street_segments = find_street_street_segments(street_id);
    //Add the lengths of all segments in the street
    for (auto it=street_segments.begin(); it != street_segments.end() ; it++)
    {
        length += find_street_segment_length(*it);       
    }
    
    return length;    
}

//find the travel time to drive a street segment (time(minutes) = distance(km)/speed_limit(km/hr) * 60
double find_street_segment_travel_time(unsigned street_segment_id){
    //check for street segment bound
    if (street_segment_id > number.Street_Segments || street_segment_id < 0 ){
        cout<< "Error: Street_segment_ID is out of range!" << endl;
        return 0;
    }
    //Get the street segment information
    StreetSegmentInfo_2 streetSegment = Street_Segment_Info[street_segment_id];
    //Compute distance (street segment length)
    double distance = find_street_segment_length(street_segment_id);
    //Convert distance from meters to Km
    distance = distance / 1000;
    //Compute travel time in minutes
    double time = (distance / streetSegment.speedLimit) * 60;
    return time;        
}

//find the nearest point of interest to a given position
unsigned find_closest_point_of_interest(LatLon my_position){
    unsigned closest_point = 0;     
    vector<LatLon> Points_Of_Interest;
    //get Point of POI position
    for(unsigned i = 0; i < number.Points_Of_Interest; i++){
        Points_Of_Interest.push_back(getPointOfInterestPosition(i));
    }
    
    double min_dist = find_distance_between_two_points(Points_Of_Interest[0], my_position);
    //test all points of interest distance to my position
    for(unsigned i=0; i < number.Points_Of_Interest; i++){
        double temp = find_distance_between_two_points_2(Points_Of_Interest[i], my_position);
        if(min_dist > temp){
            min_dist = temp;
            closest_point = i;
        }
            
    }    
    return closest_point;
}

//find the nearest intersection (by ID) to a given position
unsigned find_closest_intersection(LatLon my_position){    
    unsigned closest_point = 0;
    double min_dist = find_distance_between_two_points(Intersection_Info[0].pos, my_position);
    for(unsigned i=0; i < number.Intersections; ++i){
        LatLon pos = Intersection_Info[i].pos;
        //position is too far away -> continue
        if(new_abs(pos.lat - my_position.lat) > 0.1 || new_abs(pos.lon - my_position.lon) > 0.1)
            continue;
        //test distance
        double temp = find_distance_between_two_points_2(Intersection_Info[i].pos, my_position);
        if(min_dist > temp){
            min_dist = temp;
            closest_point = i;
        }        
    }    
    return closest_point; 
}

//A special find-distance function for closest intersection to save time
double find_distance_between_two_points_2(LatLon point1, LatLon point2){
    double distancey, distancex;
    double lon1,lon2,lat1,lat2;  
    //Converting degrees to radians
    lon1 = point1.lon * DEG_TO_RAD;
    lat1 = point1.lat * DEG_TO_RAD;
    lon2 = point2.lon * DEG_TO_RAD;
    lat2 = point2.lat * DEG_TO_RAD;
    
    double avg_lat = (lat1 + lat2)/2;
    //Transforming longitude and latitude to rectangular coordinates (x,y)
    double cosine = cos_core(avg_lat);
    double x1 = lon1 * cosine; //x1
    double x2 = lon2 * cosine; //x2
      
    distancey = lat2 - lat1;
    distancex = x2 - x1;    
    return ((distancey*distancey)+(distancex*distancex));
}

