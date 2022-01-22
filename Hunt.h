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
// (3) use a switch with cases to determine hunt order!
class Hunt {
    private:
        // current coords
        int current_r;
        int current_c;

        // treasure found?
        bool treasure_found = false;
        bool search_ended = false;

        // size of the map being read in (8 = 8x8)
        int map_size;


        // create 4 directions, change x and y based on the heading indicated
        struct Direction {
            int x;
            int y;
        };

        // when the location added to the container is treasure, stop the search
        // this struct represents a single point on the map
        struct Spot {
            // note: unsure if there is an easier way to represent the 'type' of spot
            char spot_type;
            // discovered
            bool discovered = false;
        };

        Spot current_sail;
        Spot current_search;
        // full map
        // implementation for reading in a map
        // vector<vector<point>> 
        // point: struct with land_type, been_visited
        // file will be map or list of grid coords
        std::vector<std::vector<Spot>> grid;

        // deques for searching
        std::deque<Spot> captain_deque;
        std::deque<Spot> firstmate_deque;

        // private functions that do work for public "read_map()"

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
        void search(std::string captain_type, std::string firstmate_type, std::string hunt_order, bool verbose_on, bool stats_on, bool show_path_on) {
            // add starting point to search container, mark as visited, then pass on the work to queue or stack search
            // searches for treasure until treasure is found, or the search returns with no results

            // add start to sail container
            // try sticking to grid[r][c] rather than using an extra 'current_spot' variable?
            captain_deque.push_back(grid[current_r][current_c]);

            // start the captain on the search process, looping until the search has ended, or treasure has been found (or both)
            if(captain_type == "STACK") {
                while(!search_ended || !treasure_found) {
                    // look in all adjacent spaces, add to container, move to new location 
                    stack_search('.');
                }
            } else {
                while(!search_ended || treasure_found) {
                    queue_search('.');
                }
            }
        }

        //  searches using a queue. parameter passed in is the type of spot to search for (i.e. captain or firstmate)
        void queue_search(char spot_type) {
            // if spot_type == 'o', make sure to consider treasure
            // search for spot_type

            // (1) Start at current location, look at adjacent locations, add to container



            // if no more places to go, end the search if treasure has not been found (i.e. sail container empty)
        }

        void stack_search(char spot_type) {
            // if spot_type == 'o', make sure to consider treasure
            // search for spot_type


            // if no more places to go, end the search if treasure has not been found (i.e. sail container empty)

        }
};