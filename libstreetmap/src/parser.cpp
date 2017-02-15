/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "parser.h"

static char** command_completion(const char* , int , int);
char* street_name_generator(const char*, int );

/*
 Structure with the names of main streets in Toronto. It is used for auto completer 
 to find possible matches with the user input
 */
std::vector<string> street_names = {
    "Airport Road",
    "Albion Road",
    "Annette Street",
    "Atwell Drive",
    "Avenue Road",
    "Bathurst Street",
    "Bay Street",
    "Bayly Street",
    "Bayview Avenue",
    "Bellamy Road North",
    "Birchmount Road",
    "Black Creek Drive",
    "Bloor Street East",
    "Bloor Street West",
    "Bovaird Drive",
    "Bovaird Drive East",
    "Bramalea Road",
    "Brimley Road",
    "Bristol Road West",
    "Britannia Road West",
    "Broadview Avenue",
    "Burnhamthorpe Road",
    "Burnhamthorpe Road East",
    "Burnhamthorpe Road West",
    "Carlton Street",
    "Castlemore Road",
    "Centre Street",
    "College Street",
    "Church Street South",
    "Creditview Road",
    "Danforth Avenue",
    "Davenport Road",
    "Derry Road East",
    "Dixie Road",
    "Dixon Road",
    "Don Mills Road",
    "Donald Cousens Parkway",
    "Dufferin Street",
    "Dundas Street East",
    "Dundas Street West",
    "Eglinton Avenue East",
    "Eglinton Avenue West",
    "Elgin Mills Road East",
    "Ellesmere Road",
    "Fourteenth Avenue",
    "Finch Avenue",
    "Finch Avenue East",
    "Finch Avenue West",
    "Front Street East",
    "Front Street West",
    "Gerrard Street East",
    "Goreway Drive",
    "Harwood Avenue North",
    "Hurontario Street",
    "Islington Avenue",
    "Jane Street",
    "Jarvis Street",
    "Keele Street",
    "Kennedy Road",
    "King Road",
    "Kingston Road", 
    "Kingston Road East",
    "Kingston Road West",
    "Kipling Avenue",
    "Lake Shore Boulevard West",
    "Lakeshore Road East",
    "Langstaff Road",
    "Lansdowne Avenue",
    "Lawrence Avenue East",
    "Lawrence Avenue West",
    "Leslie Street",
    "Liverpool Road",
    "Main Street",
    "Main Street North",
    "Main Street South",
    "Markham Road",
    "Martin Grove Road",
    "Mayfield Road",
    "McCowan Road",    
    "McLevin Avenue",
    "Midland Avenue",
    "Mississauga Road",
    "Morningside Avenue",
    "Mount Pleasant Road",
    "Neilson Road",
    "Ninth Line"
    "Oakwood Avenue",
    "O'Connor Drive",
    "Parkwoods Village Drive",
    "Parliament Street",
    "Pharmacy Avenue",
    "Pine Valley Drive",
    "Progress Avenue",
    "Queen Street East",
    "Queen Street South",
    "Queensway East",
    "Rathburn Road",
    "Rogers Road",
    "Rossland Road West",
    "Royal Windsor Drive",
    "Royal York Road",
    "Sandalwood Parkway",
    "Scarlett Road",
    "Sewells Road",
    "Sherbourne Street",
    "Sheppard Avenue East",
    "Sheppard Avenue West",
    "Spadina Avenue",
    "Southdown Road",    
    "Steeles Avenue",
    "Steeles Avenue East",
    "Steeles Avenue West",
    "Saint Clair Avenue East",
    "Saint Clair Avenue West",
    "St. Clair Avenue East",
    "Tapscott Road",
    "Teston Road",
    "The Queensway",
    "The East Mall",
    "The West Mall",
    "Trafalgar Road",    
    "University Avenue",
    "Valhalla Inn Road",
    "Victoria Park Avenue",
    "Yonge Street",
    "York Mills Road",
    "Warden Avenue",
    "Westney Road North",
    "Westney Road South",
    "Weston Road",
    "Weston Road North",
    "William R. Allen Road",
    "Wilson Avenue",
    "Winston Churchill Boulevard",
    "Woodbine Avenue",
};

/*
 Structure with the names of the most popular points of interest in Toronto. It is used
 to auto-complete user input
 */
std::vector<string> POI_names = {
    
"Shoppers Drug Mart",           "Cardiac Care Clinic",              "Christ Embassy",
"Toronto Sun",                  "Pizza Pizza",                      "Domino's Pizza",
"Mr. Sub",                      "KFC",                              "Pharmasave",
"Petro-Canada",                 "Shell",                            "Tim Hortons",
"Canada Post Letter Carrier Depot",                                 "CIBC",
"Pump House Grille Co.",        "SPoT Coffee",                      "Mi Ne Sushi Japanese Restaurant",
"Bank of Montreal",             "East Liberty Medical Centre",      "Starbucks Coffee",
"Subway",                       "Booster Juice",                    "Shoeless Joe's",
"BMO Bank of Montreal",         "City Kids Child Care Centre",      "24 Hours",
"Druxy's",                      "Pharma Plus Drugmart",             "Bramalea Medical Centre",
"Coffee Time",                  "Husky",                            "Freshly Squeezed",
"Wendy's",                      "TriOS College",                    "State Bank of India (Canada)",
"Retro Burger",                 "Engineering Library",              "First Baptist Church",
"Varsity Cinema",               "Emergency - Sunnybrook Hospital",
"Sunnybrook Pharmacy",          "All Health Rehabilitation",        "Rexall IDA",
"Animal Vaccination",           "Good Luck Chinese Restaurant",     "Pizza Nova",
"Cake Town Cafe",               "Dental Office",                    "Hero Certified Burgers",
"McDonald's",                   "Baskin Robbins",                   "Dental Care",
"Pizzaiolo",                    "Bonaventure Child Care Centre",    "Simple Bistro",
"Integra Health & Wellness Clinic",                                 "Mr Burrito",
"7 Eleven",                     "Popular Choice Medical Centre",    "Medical Centre",
"U-Haul",                       "Wheels 4 Rent",                    "RBC Financial Group",
"Rexall Drug Store",            "TD Canada Trust",                  "A&W",
"Thai Express",                 "Aroma Espresso Bar",               "Second Cup",
"Harvey's",                     "Chipotle Mexican Grill",           "South Street Burger Co.",
"Campus Security",              "Quick Pita",                       "Sushi & BBQ",
"Rexall",                       "Foreign Exchange",                 "Yogen Fruz",
"Scotiabank",                   "Swiss Chalet",                     "Dairy Queen",
"BMO",                          "Centre Cafe",                      "City Centre Health Clinic",
"Milestone's Grill & Bar",      "Pizza Hut",                        "Sushi One",
"Money Mart",                   "Jimmy the Greek",                  "Canada Post",
"Ali Baba's",                   "Popeyes Louisiana Kitchen",        "Spring Rolls",
"Domino's",                     "SuperWash",                        "Bell",
"Teriyaki",                     "Club House",                       "O Noir",
"Sushi Time",                   "Bourbon St. Grill",                "Dental Emergency Service",
"Top Sushi",                    "National Bank",                    "Five Guys",
"Tokyo Sushi",                  "Quesada Mexican Grill",            "PharmaPlus Drugmart",
"Toronto Public Library - Forest Hill",                             "The Toronto Wedding Chapel",
"Cafe Iris",                    "Etobicoke Medical Centre",         "The Pita Pit",
"Yogurty's",                    "Bixi Bike Sharing",

};

/*************************      PARSER          ****************************/
string new_map_parser(){
    stringstream ss;
    string input;
    int map_id;
    
    cout << setw(25) << left << "1. Cairo (Egypt)"  << 
            setw(25) << left << "2. Hamilton (Canada)" << 
            setw(25) << left << "3. Moscow" << endl << 
            setw(25) << left << "4. Newyork" << 
            setw(25) << left << "5. Saint Helena" <<  
            setw(25) << left << "6. Toronto (Canada)" << endl <<
            "Please enter ONLY map id matched with map name above: "; 
    
    do{
        getline(cin, input);
        ss << input;
        
        ss >> map_id;
        map_id--;
        
        if(map_id < 0 || map_id > 5 || ss.fail() || !ss.eof())
            cout << "Invalid. Enter again: ";
        else{
            cout << "Valid. New map is loaded" << endl;
            break;
        }
        input = "";
        ss.clear();
    }while(true);
    
    load_new_map(map_id);
    
    return map_name_2(map_id);
}

vector<LatLon> find_intersection_parser(){
    string street_1, street_2;
    char *first_street;
    char *second_street;
    vector<unsigned> street_id;
    vector<unsigned> intersection_id;
    vector<LatLon> intersection_pos;   
    
    //Use our function for auto-complete
    rl_attempted_completion_function = command_completion;        
    
    //Parser name of street 1
    while((first_street = readline("Please type in the street name 1: ")) != NULL) { //Prompt the user for input        
        //Use tab for auto completion
        rl_bind_key('\t', rl_complete); 
        //make the input an string
        street_1 = string(first_street);        
        street_1.pop_back();//Get rid of the last character enter when pressing 'tab'        
        street_id = find_street_ids_from_name(street_1); //Find street id
        
        if (street_id.size() != 0)
            add_history(first_street); //Only add to history if user input is correct        
        else
        {
            cout << "Street name 1 can't be found. Try again." << endl;
            continue;
        }
        break;
        
        //readline generates a new buffer every time, 
        //so we need to free the current one after we've finished
        free(first_street);
        first_street = NULL; //Mark it null to show we freed it
    }
    free(first_street);
    
    //Parser name of street 2
    while((second_street = readline("Please type in the street name 2: ")) != NULL) { //Prompt the user for input        
        //Use tab for auto completion
        rl_bind_key('\t', rl_complete);         
        
        street_2 = string(second_street); //make the input an string
        street_2.pop_back(); //Get rid of the last character enter when pressing 'tab'
        
        street_id = find_street_ids_from_name(street_2);
        
        if (street_id.size() != 0) 
            add_history(second_street); //Only add to history if user input is correct        
        else
        {
            cout << "Street name 2 can't be found. Try again." << endl;
            continue;
        }
        break;
        
        //readline generates a new buffer every time, 
        //so we need to free the current one after we've finished
        free(second_street);
        second_street = NULL; //Mark it null to show we freed it
    }
    free(second_street);
    
    //Find Intersection
    intersection_id = find_intersection_ids_from_street_names(street_1, street_2);
   
    if(intersection_id.size() == 0){
        cout << "There is no intersection between 2 streets." << endl;
    }
    else{
        cout << "Intersections found." << endl;
        for(unsigned i=0; i < intersection_id.size(); i++){
            unsigned id;
            LatLon pos;

            id = intersection_id[i];
            pos = Intersection_Info[id].pos;
            pos.lon = pos.p_lon();
            intersection_pos.push_back(pos);
        }
    }
    return intersection_pos;
}

vector<unsigned> find_street_parser(){
    char* input;
    string street_name;
    vector<unsigned> street_id;
    
    //Use our function for auto-complete
    rl_attempted_completion_function = command_completion;
    
    while((input = readline("Please input street name: ")) != NULL)
    {
         //Use tab for auto completion
        rl_bind_key('\t', rl_complete);         
        
        street_name = string(input); //make the input an string
        street_name.pop_back(); //Get rid of the extra last character enter when pressing 'tab'
        street_id.clear();
        street_id = find_street_ids_from_name(street_name); // Find street ID
        
        if(street_id.size() == 0)
            cout << "Street name can't be found. Try again." << endl;
        else
        {
            add_history(input); 
            cout << "Street name is found." << endl;
            break;
        }
        
        //readline generates a new buffer every time, 
        //so we need to free the current one after we've finished
        free(input);
        input = NULL; //Mark it null to show we freed it
    }
    free(input);
          
    return street_id;
}

/***********************      LOAD NEW MAP SUPPORT      ***********************/
void load_new_map(int map_id){
    const string maps[6] = {"cairo_egypt", "hamilton_canada", "moscow", "newyork", "saint_helena", "toronto"}; 
    
    string path, map_name_2, extention;
    close_map();
    path = "/cad2/ece297s/public/maps/";
    map_name_2 = maps[map_id];
    extention = ".streets.bin";
    load_map(path + map_name_2 + extention);
}

string map_name_2(int map_id){
    const string maps[6] = {"Cairo Egypt", "Hamilton (Canada)", "Moscow", 
                            "Newyork", "Saint Helena", "Toronto"};
    return maps[map_id];
}

/*********************     INPUT COMPLETION FUNCTIONS     **********************/
char** command_completion(const char* text, int start, int end) 
{    
    char **matches = NULL;
     
    if (start == 0)
        matches = rl_completion_matches (text, &street_name_generator);
    else    
        rl_bind_key('\t',rl_abort);
   
    return matches;
}

char* street_name_generator(const char* text, int state) 
{
    static int list_index, len;
    const char *name;
 
    if (state == 0)
    {
        list_index = 0; 
        len = strlen (text);
    }
 
    while (unsigned(list_index) < street_names.size()) {
        string temp = street_names[list_index];
        name = temp.c_str();
        list_index++;
 
        if (strncmp (name, text, len) == 0)       
            return strdup(name);        
    }
 
    /* If no names matched, then return NULL. */
    return ((char *)NULL);
}

string look_for_match (string input)
{
    string name;
    int length = input.length();
    
    for (unsigned i =0; i< street_names.size(); i++)
    {
      name = street_names[i];
      
      if (name.compare(0,length,input) == 0)
          return name;      
    }
    
    //if no match, return empty string
    return "";        
}

string complete_search_box_input (string text, int length, string type)
{
    //Take the user input only 
    string input = text.substr(length,text.size()-length); 
    //look for matches in our street_names structure
    string match;
    if (type == "intersection") match = look_for_match(input);
    else if (type == "interest") match = match_POI(input);
    else match = "";
    //complete the string with our match
    string tempOutput = text.substr(0,length);
    string output = tempOutput.append(match);// output = Search_text - input + match;
    
    return output;
}

string fix_blank_spaces (string text_in)
{    
    stringstream ss;
    string text_out, dummy;    
    ss << text_in;
    
    ss >> text_out; //First word    
    while (ss >> dummy){
        text_out.append(" ");
        text_out.append(dummy);
    }
    
    return text_out;
}

string match_POI (string input)
{
    string name;
    int length = input.length();
    
    for (unsigned i =0; i< POI_names.size(); i++)
    {
      name = POI_names[i];
      
      if (name.compare(0,length,input) == 0)
          return name;      
    }
    
    //if no match, return empty string
    return ""; 
}