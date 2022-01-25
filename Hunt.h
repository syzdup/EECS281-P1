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
        int firstmate_r;
        int firstmate_c;

        // treasure found?
        bool treasure_found = false;
        bool search_ended = false;
        bool search_party = false;

        // size of the map being read in (8 = 8x8)
        int map_size;

        // ctor vars
        std::string hunt_order;
        std::string captain_type;
        std::string firstmate_type;
        bool verbose_on;
        bool show_path_on;
        bool stats_on;
        char path_type;

        // create 4 directions, change r and c based on the heading indicated
        struct Location {
            int r;
            int c;
        };
        // directions with pairs that change row and column 
        Location north{-1, 0};
        Location east{0, 1};
        Location south{1, 0};
        Location west{0, -1};
        Location no_where{0, 0};

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
            Location came_from{0, 0};
        };

        // full map
        // implementation for reading in a map
        // vector<vector<point>> 
        // point: struct with land_type, been_visited
        // file will be map or list of grid coords
        std::vector<std::vector<Spot>> grid;

        // deques for searching
        std::deque<Location> captain_deque;
        std::deque<Location> firstmate_deque;

        // private functions that do work for public "read_map()":
        // read input for a list file
        void read_list_file();
        // read input from a map file
        void read_map_file();
        // calculate path length
        int calculate_path_length();


    public:
        // ctor
        Hunt(std::string hunt_order_in, std::string captain_type_in, std::string firstmate_type_in, bool verbose_on_in, 
        bool show_path_on_in, bool stats_on_in, char path_type_in);
        // calls other functions based on whether it is in list or map format
        void read_map();
        // print out the map
        void print_map();
        // search function
        void search();
        //  searches using a deque
        void deque_search(bool deque_type);
        void check_adjacents_captain();
            
        // change to look for treasure, ignore water, etc. 
        void check_adjacents_firstmate();
            
        // compare locations
        friend bool operator==(const Location &lhs, const Location &rhs) { if((lhs.r == rhs.r) && (lhs.c == rhs.c)) { return true; } return false; }
        friend bool operator!=(const Location &lhs, const Location &rhs) { return !(lhs == rhs); }
};
#endif