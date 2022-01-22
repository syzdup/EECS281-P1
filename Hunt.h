// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles the search process
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include "Input.h"

// (2) once the map is stored as a 2d vector, implement a queue search function and a stack
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
        bool sub_search = false;

        // size of the map being read in (8 = 8x8)
        int map_size;

        // ctor vars
        std::string hunt_order;
        std::string firstmate_type;
        std::string captain_type;
        bool verbose_on;
        bool stats_on;
        bool show_path_on;

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

        // represents where you are
        Location current_location;
        Location firstmate_location;

        // when the location added to the container is treasure, stop the search
        // this struct represents a single point on the map
        struct Spot {
            // note: unsure if there is an easier way to represent the 'type' of spot
            char spot_type;
            // discovered
            bool discovered = false;
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
        void read_list_file() {
            std::cin >> map_size;

            grid.resize(map_size);
            for(int i = 0; i < map_size; ++i) {
                grid[i].resize(map_size);
            }

            std::string junk;
            getline(std::cin, junk);

            int r;
            int c;
            char spot_type;
            while(std::cin >> r >> c >> spot_type) {
                grid[r][c].spot_type = spot_type;
            }
            // fill in blank spots with water (can i check if the spot has a blank value rather than checking if it is each of the types? would be faster prob)
            for(int i = 0; i < map_size; ++i) {
                for(int j = 0; j < map_size; ++j) {
                    // check for starting point
                    if(grid[i][j].spot_type == '@') {
                        current_r = i;
                        current_c = j;
                    }
                    if(grid[i][j].spot_type != 'o' && grid[i][j].spot_type != '$' && grid[i][j].spot_type != '@') {
                        grid[i][j].spot_type = '.';
                    }
                }
            }
        } // read_list_file()

        // read input from a map file
        void read_map_file() {
            std::cin >> map_size;
            std::string row;
            // counter to keep track of row
            int r_count = 0;
            // resize grid based on map size
            grid.resize(map_size);
            for(int i = 0; i < map_size; ++i) {
                grid[i].resize(map_size);
            }

            // deal with 'junk' newline
            std::string junk;
            getline(std::cin, junk);

            // read in each row and fill up grid
            while(getline(std::cin, row)) {
                // counter to keep track of column
                int c_count = 0;
                for(char c: row) {
                    // check for starting point
                    if(c == '@') {
                        current_r = r_count;
                        current_c = c_count;
                    }
                    grid[r_count][c_count].spot_type = c;
                    ++c_count;
                }
                ++r_count;
            }
        } // read_map_file()


    public:
        // ctor
        Hunt(std::string hunt_order_in, std::string captain_type_in, std::string firstmate_type_in, bool verbose_on_in, bool show_path_on_in, bool stats_on_in)
        : hunt_order(hunt_order_in), captain_type(captain_type_in), firstmate_type(firstmate_type_in), verbose_on(verbose_on_in), show_path_on(show_path_on_in), stats_on(stats_on_in){}
        // calls other functions based on whether it is in list or map format
        void read_map() {
            char map_type = ' ';
            // read in comments until map_type is specified, then break
            std::string comments;
            while(getline(std::cin, comments)) {
                if(comments[0] != '#') {
                    map_type = comments[0];
                    break;
                }
            }
            // store the rest of the map based on map_type
            if(map_type == 'M') {
                read_map_file();
            // map type should be 'L' in this case
            } else {
                read_list_file();
            }
            // set the current location now
            current_location.r = current_r;
            current_location.c = current_c;
        }

        // print out the map
        void print_map() {
            for(int i = 0; i < map_size; ++i) {
                for(int j = 0; j < map_size; ++j) {
                    std::cout << grid[i][j].spot_type;
                }
                std::cout << "\n";
            }
        }

        // search function
        // NOTE: add backtracking feature later. maybe add a 'came_from' direction container to spot struct
        // when backtracking is added, use 'came_from' and get rid of the visited bool. you can just see if the location has been 'came_from' before.
        void search() {
            // add starting point to search container, mark as visited, then pass on the work to queue or stack search
            // searches for treasure until treasure is found, or the search returns with no results

            // add start to sail container
            captain_deque.push_back(current_location);

            // start the captain on the search process, looping until the search has ended, or treasure has been found (or both)
            if(captain_type == "STACK") {
                while(!search_ended || !treasure_found) {
                    // look in all adjacent spaces, add to container, move to new location 
                    stack_search();
                }
            } else {
                while(!search_ended || treasure_found) {
                    queue_search();
                }
            }
        }
        /*
                WILL PROBABLY WANT TO MAKE THESE FUNCTIONS PRIVATE IN THE FUTURE.

                ONLY THING IMPORTANT FOR INTERFACE IS SEARCH.
        */

        //  searches using a queue. parameter passed in is the type of spot to search for (i.e. captain or firstmate)
        void queue_search() {
            // if spot_type == 'o', make sure to consider treasure
            // search for spot_type

            // (1) Start at current location, look at adjacent locations, add to container

            // !!! sail location will always be grid[current_r][current_c]. don't make a new variable for this yet !!!
            if(!sub_search) {
                while(!captain_deque.empty()) {
                    // now look for adjacent locations, add to container if possible
                    // SAIL LOCATION IS FRONT FOR QUEUE!!!
                    check_adjacents_captain();
                    current_location = captain_deque.front();
                    captain_deque.pop_front();
                }
            } else {
                // search party sent out:
                // (1) add adjacent locations
                // (2) set current location, check if treasure
                // (3) pop if not, check until empty
                while(!firstmate_deque.empty()) {
                    check_adjacents_firstmate();
                    firstmate_location = firstmate_deque.front();

                    if(grid[firstmate_location.r][firstmate_location.c].spot_type == '$') {
                        treasure_found = true;
                        search_ended = true;
                    }
                    firstmate_deque.pop_front();
                }
                sub_search = false;
            }
            // if no more places to go, end the search if treasure has not been found (i.e. sail container empty)
            search_ended = true;
        }

        void stack_search() {
            // if spot_type == 'o', make sure to consider treasure
            // search for spot_type
            // SAIL LOCATION IS TOP FOR STACK

            if(!sub_search) {
                while(!captain_deque.empty()) {
                    // now look for adjacent locations, add to container if possible
                    // SAIL LOCATION IS BACK FOR STACK!!!
                    check_adjacents_captain();
                    current_location = captain_deque.back();
                    captain_deque.pop_back();
                }
            } else {
                // search party sent out:
                // (0) add starting location 
                // (1) add adjacent locations
                // (2) set current location, check if treasure
                // (3) pop if not, check until empty
                while(!firstmate_deque.empty()) {
                    check_adjacents_firstmate();
                    firstmate_location = firstmate_deque.back();

                    if(grid[firstmate_location.r][firstmate_location.c].spot_type == '$') {
                        treasure_found = true;
                        search_ended = true;
                    }
                    firstmate_deque.pop_back();
                }
                // the firstmate hunt has ended
                sub_search = false;

            }
            // if no more places to go, end the search if treasure has not been found (i.e. sail container empty)
            search_ended = true;

        }

        void check_adjacents_captain() {
            for(int i = 0; i < int(hunt_order.length()); ++i) {
                switch(hunt_order[i]) {
                    case 'N':
                    // check bounds, visited, spot type 
                        if(current_location.r + north.r >= 0) {
                            if(grid[current_location.r + north.r][current_location.c].discovered = false && grid[current_location.r + north.r][current_location.c].spot_type != '#') {
                                // land found, start a subsearch using a queue or stack
                                if(grid[current_location.r + north.r][current_location.c].spot_type == 'o') {
                                    sub_search = true;
                                    Location new_land{current_location.r + north.r, current_location.c};
                                    grid[new_land.r][new_land.c].discovered = true;
                                    firstmate_deque.push_back(new_land);
                                    firstmate_location = new_land;
                                    // i think this is only for subsearch? can i replace these variables
                                    if(firstmate_type == "QUEUE") {
                                        queue_search();
                                    } else {
                                        stack_search();
                                    }
                                }
                                // not land, valid water spot on map!
                                Location new_water{current_location.r + north.r, current_location.c};
                                grid[new_water.r][new_water.c].discovered = true;
                                captain_deque.push_back(new_water);
                                }
                        }
                    case 'E':
                        if(current_location.c + east.c < map_size) {
                            if(grid[current_location.r][current_location.c + east.c].discovered = false && grid[current_location.r][current_location.c + east.c].spot_type != '#') {
                                // check if land has been found
                                if(grid[current_location.r][current_location.c + east.c].spot_type == 'o') {
                                    sub_search = true;
                                    Location new_land{current_location.r, current_location.c + east.c};
                                    grid[new_land.r][new_land.c].discovered = true;
                                    firstmate_deque.push_back(new_land);
                                    firstmate_location = new_land;
                                    if(firstmate_type == "QUEUE") {
                                        queue_search();
                                    } else {
                                        stack_search();
                                    }
                                }
                                // not land, valid water spot
                                Location new_water{current_location.r, current_location.c + east.c};
                                grid[new_water.r][new_water.c].discovered = true;
                                captain_deque.push_back(new_water);
                            }
                        }
                    case 'S':
                        // check bounds, visited, spot type 
                        if(current_location.r + south.r < map_size) {
                            if(grid[current_location.r + south.r][current_location.c].discovered = false && grid[current_location.r + south.r][current_location.c].spot_type != '#') {
                                // land found, start a subsearch using a queue or stack
                                if(grid[current_location.r + south.r][current_location.c].spot_type == 'o') {
                                    sub_search = true;
                                    Location new_land{current_location.r + south.r, current_location.c};
                                    grid[new_land.r][new_land.c].discovered = true;
                                    firstmate_deque.push_back(new_land);
                                    firstmate_location = new_land;
                                    // i think this is only for subsearch? can i replace these variables
                                    if(firstmate_type == "QUEUE") {
                                        queue_search();
                                    } else {
                                        stack_search();
                                    }
                                }
                                // not land, valid water spot on map!
                                Location new_water{current_location.r + south.r, current_location.c};
                                grid[new_water.r][new_water.c].discovered = true;
                                captain_deque.push_back(new_water);
                                }
                        }
                    case 'W':
                        if(current_location.c + west.c >= 0) {
                            if(grid[current_location.r][current_location.c + west.c].discovered = false && grid[current_location.r][current_location.c + west.c].spot_type != '#') {
                                // check if land has been found
                                if(grid[current_location.r][current_location.c + west.c].spot_type == 'o') {
                                    sub_search = true;
                                    Location new_land{current_location.r, current_location.c + west.c};
                                    grid[new_land.r][new_land.c].discovered = true;
                                    firstmate_deque.push_back(new_land);
                                    firstmate_location = new_land;
                                    if(firstmate_type == "QUEUE") {
                                        queue_search();
                                    } else {
                                        stack_search();
                                    }
                                }
                                // not land, valid water spot
                                Location new_water{current_location.r, current_location.c + west.c};
                                grid[new_water.r][new_water.c].discovered = true;
                                captain_deque.push_back(new_water);
                            }
                        }

                    default:
                        std::cout << "ERROR: No such direction." << "\n";
                        break;
                }

            }
        }

        // change to look for treasure, ignore water, etc. 
        void check_adjacents_firstmate() {
            for(int i = 0; i < int(hunt_order.length()); ++i) {
                switch(hunt_order[i]) {
                    case 'N':
                        if(firstmate_location.r + north.r >= 0) {
                            if(grid[firstmate_location.r + north.r][firstmate_location.c].discovered = false && grid[firstmate_location.r + north.r][firstmate_location.c].spot_type != '#' && grid[firstmate_location.r + north.r][firstmate_location.c].spot_type != '.') {
                                Location new_land{firstmate_location.r + north.r, firstmate_location.c};
                                grid[new_land.r][new_land.c].discovered = true;
                                firstmate_deque.push_back(new_land);   
                            }     
                        }
                    case 'E':
                        if(firstmate_location.c + east.c < map_size) {
                            if(grid[firstmate_location.r][firstmate_location.c + east.c].discovered = false && grid[firstmate_location.r][firstmate_location.c + east.c].spot_type != '#' && grid[firstmate_location.r][firstmate_location.c + east.c].spot_type != '.') {
                                Location new_land{firstmate_location.r, firstmate_location.c + east.c};
                                firstmate_deque.push_back(new_land);
                                }
                        }
                    case 'S':
                        if(firstmate_location.r + south.r >= 0) {
                            if(grid[firstmate_location.r + south.r][firstmate_location.c].discovered = false && grid[firstmate_location.r + south.r][firstmate_location.c].spot_type != '#' && grid[firstmate_location.r + south.r][firstmate_location.c].spot_type != '.') {
                                Location new_land{firstmate_location.r + south.r, firstmate_location.c};
                                firstmate_deque.push_back(new_land);   
                            }     
                        }
                    case 'W':
                        if(firstmate_location.c + west.c >= 0) {
                            if(grid[firstmate_location.r][firstmate_location.c + west.c].discovered = false && grid[firstmate_location.r][firstmate_location.c + west.c].spot_type != '#' && grid[firstmate_location.r][firstmate_location.c + west.c].spot_type != '.') {
                                Location new_land{firstmate_location.r, firstmate_location.c + west.c};
                                firstmate_deque.push_back(new_land);
                            }
                        }
                    default:
                        std::cout << "ERROR: No such direction." << "\n";
                        break;
                }

            }

        }

};