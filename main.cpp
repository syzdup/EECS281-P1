// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// EECS 281 Project 1
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Hunt.h"
using namespace std;

// TODO: delete debug functions 
void DEBUG_check_options(Option_handler oh) {
    cout << "Captain type: " << oh.get_captain_type() << "\n";
    cout << "First-mate type: " << oh.get_firstmate_type() << "\n";
    cout << "Hunt order: " << oh.get_hunt_order() << "\n";
    cout << "Verbose on: " << oh.verbose_on() << "\n";
    cout << "Stats on: " << oh.stats_on() << "\n";
    cout << "Show path on: " << oh.show_path_on() << "\n";
    cout << "---ALL STATS LISTED ABOVE---\n\n";
}

int main(int argc, char *argv[]) {

    ios_base::sync_with_stdio(false);

    Option_handler oh;

    // read in all provided options
    oh.get_options(argc, argv);

    // for debug only, delete later 
    DEBUG_check_options(oh);

    Hunt hunt;
    hunt.read_map();
    hunt.print_map();

}

    // now comes the actual hunt algorithm, not sure if i should put it into main
    // unsure if i should be using two deques
    // vector<vector<Spot>> map = oh.get_grid();
    // deque<Spot> sail_container;
    // deque<Spot> search_container;
    // Spot sail_location;
    // Spot search_location;
    // int r_sail = oh.get_start_r();
    // int c_sail = oh.get_start_c();

    // discover new spot, add to container, continue search until treasure is found
    // a spot can only be current location once, after discovered, can't revisit
    // start with captain at starting point (note: this doesn't set out current spot yet)
//     sail_container.push_back(map[r_sail][c_sail]);
//     // discovered starting point
//     map[r_sail][c_sail].discovered = true;
//     // now the container is 'seeded' go to loop now
//     bool treasure_found = false;
//     // starts at beginning of hunt order and goes around
//     // tip: add 1 when location is tried, then mod by 4
//     int hunt_order_index = 0;

//     while(!treasure_found) {
//         // start captain loop
//         // if water not found, start firstmate loop
//         if(!sail_container.empty()) {
//             sail_container.pop_back();
//             sail_location = map[r_sail][c_sail];
//             // look for a new location and decide what to do from there
//             // first, check which direction to go, then check if it is a valid spot (water location, on grid)
//             if(oh.get_hunt_order()[hunt_order_index] == 'N') {
//                 if((r_sail -  1) > 0 && map[r_sail - 1][c_sail].spot_type == '.') {
//                     r_sail -= 1;
//                     break;
//                 } else {
//                     break;
//                 }
//             } else if(oh.get_hunt_order()[hunt_order_index] == 'E') {
//                 if((c_sail +  1) < oh.get_map_size() && map[r_sail][c_sail + 1].spot_type == '.') {
//                     c_sail += 1;
//                     break;
//                 } else {
//                     break;
//                 }
//             } else if(oh.get_hunt_order()[hunt_order_index] == 'S') {
//                 if((r_sail +  1) < oh.get_map_size() && map[r_sail + 1][c_sail].spot_type == '.') {
//                     r_sail += 1;
//                     break;
//                 } else {
//                     break;
//                 }
//             } else if(oh.get_hunt_order()[hunt_order_index] == 'W') {
//                 if((r_sail -  1) > 0 && map[r_sail - 1][c_sail].spot_type == '.') {
//                     r_sail -= 1;
//                     break;
//                 } else {
//                     break;
//                 }
//             }
//     }

// }
