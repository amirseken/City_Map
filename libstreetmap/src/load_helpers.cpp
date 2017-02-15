
#include <sstream>

#include "load_helpers.h"
#include "global_variables.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

void load_street_segment_travel_time(){
    for(unsigned i=0; i<number.Street_Segments; i++){
        Street_Segment_Info[i].travelTime = find_street_segment_travel_time (i);
    }
}

void load_number()
{
    number.Intersections = getNumberOfIntersections();
    number.Streets = getNumberOfStreets();
    number.Street_Segments = getNumberOfStreetSegments();
    number.Points_Of_Interest = getNumberOfPointsOfInterest(); 
    number.Natural_Feature = getNumberOfFeatures();
    number.OSM_Node = getNumberOfNodes();
    number.OSM_Way = getNumberOfWays();
    number.OSM_Relation = getNumberOfRelations();
}

void load_street()
{
    for(unsigned i = 0; i < number.Streets; i++){
        string temp = getStreetName(i);
        Map_street.insert(make_pair(temp, i));         //store street name and id in (unordered_multimap)
       
        StreetInfo info;
        info.Name = temp;
        Street_Info.push_back(info);            // store street id and name in (2d vector)
    }
}

void load_intersection()
{
    for(unsigned int i = 0; i < number.Intersections; i++){
        //Intersection Info
        struct IntersectionInfo info; 
        info.pos = getIntersectionPosition(i);
        info.name = getIntersectionName(i);
        Intersection_Info.push_back(info);        
        
        //My_position.push_back(info.pos);
        //street id -> intersections
        auto range = Map_street.equal_range(info.name);
    
        for(auto it = range.first; it != range.second; it++)
            Street_Info[it->second].Intersections.push_back(i);
    } // get all intersection info in one for loop.
}

void load_street_segments()
{
    for(unsigned i=0; i < number.Street_Segments; i++){
        //id -> Street Segment Info
        StreetSegmentInfo segment_info = getStreetSegmentInfo(i);
        StreetSegmentInfo_2 temp;
        temp.curvePointCount = segment_info.curvePointCount;
        temp.from  = segment_info.from;
        temp.to  = segment_info.to;
        temp.oneWay  = segment_info.oneWay;
        temp.streetID  = segment_info.streetID;
        temp.speedLimit = segment_info.speedLimit;
        //curve point pos
        for(unsigned j=0; j<temp.curvePointCount; j++){
            temp.curvePointPos.push_back(getStreetSegmentCurvePoint(i, j));
        }
        Street_Segment_Info.push_back(temp);
            
        //street id -> street segments
        Street_Info[segment_info.streetID].Street_Segments.push_back(i);
        
        //intersection id -> number of street segments
        Intersection_Info[segment_info.from].Street_Segments.push_back(i);
        Intersection_Info[segment_info.to].Street_Segments.push_back(i);
        //m3
        Intersection_Info[segment_info.from].outEdge.push_back (i);
        Intersection_Info[segment_info.from].adjacentNode.push_back (segment_info.to);
//        if(info.oneWay || i == info.from)
//            adjacentNode.push_back(info.to);
//        else{
//            adjacentNode.push_back(info.from);
//        }
        if(!segment_info.oneWay) {
            Intersection_Info[segment_info.to].outEdge.push_back(i);
            Intersection_Info[segment_info.to].adjacentNode.push_back (segment_info.from);
        }
        //street id -> intersections
        Street_Info[segment_info.streetID].Intersections.push_back(segment_info.from);
        Street_Info[segment_info.streetID].Intersections.push_back(segment_info.to);  
        
        //M3
        stringstream convert;
        string key;
        
        convert << temp.from << " " << temp.to;
        key = convert.str();
        Middle_Street_Segment.insert(make_pair(key, i));
        
    }    // get all street segment info in one for loop
}

void load_natural()
{
    for (unsigned i =0; i< number.Natural_Feature; i++){
        Natural temp;
        
        temp.Natural_type = getFeatureType(i);
        temp.Natural_name = getFeatureName(i);
        temp.Point_Count = getFeaturePointCount(i);
        
        for (unsigned j=0; j <  temp.Point_Count; j++){
            temp.Natural_Position.push_back(getFeaturePoint(i,j));
        }
        temp.Natural_OSM_Id = getFeatureOSMID(i);
        temp.Natural_OSM_Type = getFeatureOSMEntityType(i);
        Natural_Info.push_back(temp);      
    }
}

void load_POI()
{
    for (unsigned i = 0; i < number.Points_Of_Interest; i++){
        POI temp;
        temp.type = getPointOfInterestType(i);
        temp.name = getPointOfInterestName(i);
        temp.position = getPointOfInterestPosition(i);  
        POI_Info.push_back(temp);       
        
        pair <string, unsigned> item = make_pair (temp.name, i);
        POI_Name_Map.insert (item);
    }
}

void load_OSM_Nodes()
{
    for (unsigned i = 0; i < number.OSM_Node; i++)
    {
        const OSMNode* OSMNodeTemp = getNodeByIndex(i);
        NodeInfo tempNode(OSMNodeTemp->id(), OSMNodeTemp->coords(),-1, "<unknown>");
        OSMNodes.insert(pair<unsigned long long,NodeInfo>(OSMNodeTemp->id(), tempNode));        
    }
}

void load_roads()
{
    vector<vector<unsigned long long>> RoadNodesTemp;
    vector<vector<unsigned long long>> HighwayNodesTemp;
    vector<string> RoadNames;
    
    for (unsigned i=0; i < number.OSM_Way; i++)
    {
        //Get Way Tags
        const OSMWay* tempOSMWay = getWayByIndex (i);
        unsigned tags_number = getTagCount(tempOSMWay);
        string tempRoadName;
                
        //This loop is done because OSM entities can have more that one tag
        for (unsigned j=0; j < tags_number; j++)
        {
            //Get OSM way tag
            pair<string,string> tempTag = getTagPair(tempOSMWay,j);
            
            //If the way has a name, store it
            if (tempTag.first == "name")
                tempRoadName = tempTag.second;            
            //If the OSMWay is a secondary Road, store its nodes in RoasNodesTemp
            if ((tempTag.first == "highway") && (tempTag.second == "secondary"))
            {
                vector<unsigned long long> temp_node_ids = tempOSMWay->ndrefs();
                RoadNodesTemp.push_back(temp_node_ids);
                RoadNames.push_back(tempRoadName);                
            }
            //If the OSMWay is a primary Road, store its nodes in HighwayNodesTemp
            if ((tempTag.first == "highway") && (tempTag.second == "motorway"))
            {
                vector<unsigned long long> temp_node_ids = tempOSMWay->ndrefs();
                HighwayNodesTemp.push_back(temp_node_ids);
            }
        }       
    }
    //Loop to store the nodes in Secondary_Road_Nodes global vector
    for (unsigned i=0; i<RoadNodesTemp.size(); i++)
        for (unsigned j=0; j< RoadNodesTemp[i].size(); j++)
        {
            //Get current node ID
            unsigned long long current_node_ID = RoadNodesTemp[i][j];
            //Copy node position
            LatLon tempPos = OSMNodes.find(current_node_ID)->second.pos;
            //Create a tag
            int tempTag = int(i);
            //Create temporary Node
            NodeInfo nodeTemp(current_node_ID,tempPos,tempTag, RoadNames[i]);
            //Include the node in the global vector structure
            Secondary_Road_Nodes.push_back(nodeTemp);
        }        
    //Loop to store the nodes in Secondary_Road_Nodes global vector
    for (unsigned i=0; i<HighwayNodesTemp.size(); i++)
        for (unsigned j=0; j< HighwayNodesTemp[i].size(); j++)
        {
            //Get current node ID
            unsigned long long current_node_ID = HighwayNodesTemp[i][j];
            //Copy node position
            LatLon tempPos = OSMNodes.find(current_node_ID)->second.pos;
            //Create a tag
            int tempTag = int(i);
            //Create temporary Node
            NodeInfo nodeTemp(current_node_ID,tempPos,tempTag, "Highway");
            //Include the node in the global vector structure
            Highway_Nodes.push_back(nodeTemp);
        }    
}

void load_transit()
{
    for (unsigned i = 0; i < number.OSM_Node; i++)
    {
        //Get Node tags
        const OSMNode* tempOSMNode = getNodeByIndex(i);
        unsigned tags_number = getTagCount(tempOSMNode);
        
        //This loop is done because OSM entities can have more that one tag
        for (unsigned j=0; j < tags_number; j++)
        {
            pair<string,string> tempTag = getTagPair(tempOSMNode,j);
            //If the node is a subway station
            if ((tempTag.first == "railway") && (tempTag.second == "subway_entrance"))
            {
                NodeInfo tempNode (tempOSMNode->id(), tempOSMNode->coords(), -2, "no_road");
                Subway_Stations.push_back(tempNode);      
                
            }            
        }             
    }
}

void load_node (){
    vector <unsigned> adjacentNode, adjacentEdge;
    
    for (unsigned i=0; i<number.Intersections; i++){
        adjacentEdge = Intersection_Info [i].outEdge;
        adjacentNode = Intersection_Info [i].adjacentNode;
        Node_Info.emplace_back(aNode (i, adjacentNode, adjacentEdge));
        
        for(unsigned j=0; j<NUM_THREADS; j++){
            Node_Map [j].emplace_back(aNode (i, adjacentNode, adjacentEdge));
        }
    }
    
}