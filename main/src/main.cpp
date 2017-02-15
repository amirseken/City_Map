#include <sstream>
#include <list>

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "global_variables.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
using namespace std;

void parser();

int main() {
    
    loadOSMDatabaseBIN("/cad2/ece297s/public/maps/toronto.osm.bin");
    load_map("/cad2/ece297s/public/maps/toronto.streets.bin");
     
//            std::vector<DeliveryInfo> deliveries;
//        std::vector<unsigned> depots;
//        std::vector<unsigned> result_path;
//    
//        deliveries = {DeliveryInfo(64489, 10192), DeliveryInfo(66348, 47055)};
//        depots = {75020, 59249};
//    Found_Path = traveling_courier(deliveries, depots);
    
    draw_map();
    close_graphics ();
    close_map();
    closeOSMDatabase();

    return 0;
}
