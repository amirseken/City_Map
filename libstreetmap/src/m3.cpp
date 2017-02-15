
#include <queue>

#include "m3.h"

vector <unsigned> Found_Path;

// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end){
    vector<unsigned> path;
    
    aNode* sourceNode = &(Node_Info[intersect_id_start]);
    
    if(astarSearchPath (sourceNode, intersect_id_end))
        path = traceBack(intersect_id_end);
    else{
        cout << "Path can't be found";
    }    
    return path;
}  

unsigned find_segment_between_2_intersections(int from, int to){
    unsigned id;    
    string key;
    stringstream convert;  
    
    convert << from << " " << to;
    key = convert.str();
    
    auto got = Middle_Street_Segment.find(key);
    if(got != Middle_Street_Segment.end())
        id = got->second;
    else{
        stringstream c;
        key = "";
        c << to << " " << from;
        key = c.str();
        got = Middle_Street_Segment.find(key);
        id = got->second;
    }    
    return id;
}

// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const vector<unsigned>& path){
    double time = 0;
    
    //check if path size is 0
    if(path.size() == 0)
        return time;
    
    //constant turn time
    const double turn_time = 0.25;
    // current and next street segment elements
    unsigned prev_id, curr_id;
    string prev_name, curr_name;
     
    prev_id = path[0];
    time += find_street_segment_travel_time(prev_id);
    
    for(unsigned i = 1; i < path.size(); i++){
        //get street segment ids
        prev_id = path[i-1];
        curr_id = path[i];
        //get street segment names
        prev_name = Street_Info[Street_Segment_Info[prev_id].streetID].Name;
        curr_name = Street_Info[Street_Segment_Info[curr_id].streetID].Name;
        //add turn time if names are different
        if(prev_name != curr_name){
            time += turn_time;
        }
        
        time += find_street_segment_travel_time(curr_id);
    }
    return time;
}

// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, string point_of_interest_name){
  //min heap for collection point of interest
    priority_queue<poi_str, vector<poi_str>, poiCompare> poiHeap;
    
    
    vector<unsigned> shortest_path, final_path, temp_path;
    double min_time = numeric_limits <double>::infinity();
    unsigned poi_id, int_id;
    POI poi_info, saved_info;
    IntersectionInfo dest_int_info;
    IntersectionInfo start_int_info = Intersection_Info [intersect_id_start];
    double temp_time;
    
    vector <unsigned> radius_poi;
    auto range = POI_Name_Map.equal_range (point_of_interest_name);
    
    //if there is no POI with the name
    if (range.first == POI_Name_Map.end() && range.second == POI_Name_Map.end())
        return final_path;
    double temp_dist;
    for (auto i = range.first; i != range.second; i++){
        poi_id = i->second;
        poi_info = POI_Info [poi_id];
        
        
        
        dest_int_info = Intersection_Info [int_id];
        
        temp_dist = find_distance_between_two_points (start_int_info.pos, poi_info.position);
        poi_str new_poi;
        new_poi.id = poi_id;
        new_poi.dist = temp_dist;
        poiHeap.push (new_poi);
        
    }    
    
    for (unsigned i=0; i<7; i++){
        if (poiHeap.size() == 0)
            break;
        poi_str curr_poi = poiHeap.top();
        poiHeap.pop();
        
        
        aNode* source = &(Node_Info[intersect_id_start]);
    
        poi_info = POI_Info [curr_poi.id];
        int_id = find_closest_intersection (poi_info.position);
        
        if(astarSearchPath (source, int_id))
            temp_path = traceBack(int_id);
        else{
            continue;
        }
        
        temp_time = compute_path_travel_time (temp_path);
        if (temp_time < min_time){
            min_time = temp_time;
            saved_info = poi_info;
            shortest_path = temp_path;
            Destination_Id = int_id;
        }
    }
    final_path = shortest_path;
    
    return final_path;
 }


