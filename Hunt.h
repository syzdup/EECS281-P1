// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles the search process
#include <iostream>
#include <string>
#include <vector>
#include "Input.h"

// (2) once the map is stored as a 2d vector, implement a queue search function and a stack
// search function, where the parameters are what characters to look for, i.e. if captain uses stack, call 
// stack function and look for water. stack_search() and queue_search 
// (3) use a switch with cases to determine hunt order!
class Hunt {
    private:
        // starting point row and column
        int start_r;
        int start_c;

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
        // full map
        // implementation for reading in a map
        // vector<vector<point>> 
        // point: struct with land_type, been_visited
        // file will be map or list of grid coords
        std::vector<std::vector<Spot>> grid;

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
                        start_r = i;
                        start_c = j;
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
                        start_r = r_count;
                        start_c = c_count;
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
};