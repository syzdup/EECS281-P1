// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles the search process
#ifndef HUNT_H
#define HUNT_H
#include <iostream>
#include <string>
#include <deque>
#include "Input.h"
// once the map is stored as a 2d vector, implement a queue search function and a stack
// search function, where the parameters are what characters to look for, i.e. if captain uses stack, call 
// stack function and look for water. stack_search() and queue_search 
class Hunt {
    private:
        // current coords
        int current_r;
        int current_c;

        // firstmate coords
        // int firstmate_r;
        // int firstmate_c;

        // treasure found?
        bool treasure_found = false;
        bool search_ended = false;
        bool search_party = false;

        // size of the map being read in (8 = 8x8)
        int map_size;

        // ctor vars
        std::string hunt_order;
        bool captain_type;
        bool firstmate_type;
        bool verbose_on;
        bool show_path_on;
        bool stats_on;
        char path_type;

        // full map
        // implementation for reading in a map
        // vector<vector<point>> 
        // point: struct with land_type, been_visited
        // file will be map or list of grid coords
        struct Location {
            int r;
            int c;
        };

        // private functions that do work for public "read_map()":
        // read input for a list file
        void read_list_file();
        // read input from a map file
        void read_map_file();
        // calculate path length
        int calculate_path_length();


    public:
        // directions with pairs that change row and column 
        /* 
           came from start = A
           came from nowhere = B
           came from north = N
           came from east = E
           came from south = S
           came from west = W
        */

        // represents where you are
        Location current_location;
        Location firstmate_location;

        // keep track for stats
        int start_r;
        int start_c;
        int water_locations = 0;
        int land_locations = 0;
        int went_ashore = 0;
        int path_length = 0;
        int treasure_r;
        int treasure_c;

        // when the location added to the container is treasure, stop the search
        // this struct represents a single point on the map

        struct Spot {
            char spot_type;
            char came_from = 'B';
        };

        std::vector<std::vector<Spot>> grid;
        // ctor
        Hunt(std::string hunt_order_in, bool captain_type_in, bool firstmate_type_in, bool verbose_on_in, 
        bool show_path_on_in, bool stats_on_in, char path_type_in);
        // calls other functions based on whether it is in list or map format
        void read_map();
        // print out the map
        void print_map();
        // search function
        void search();
        //  searches using a deque
        void deque_search(bool deque_type, std::deque<Location> &captain_deque, std::deque<Location> &firstmate_deque);
        void check_adjacents_captain(std::deque<Location> &captain_deque, std::deque<Location> &firstmate_deque);
            
        // change to look for treasure, ignore water, etc. 
        void check_adjacents_firstmate(std::deque<Location> &firstmate_deque);

        void treasure_has_been_found(char treasure_direction, char came_from_spot, bool initial_land);
};
#endif