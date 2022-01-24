// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles the search process
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include "Hunt.h"

void Hunt::read_list_file() {
    std::cin >> map_size;

    Spot default_water{'.', no_where};
    grid.resize(map_size, std::vector<Spot>(map_size, default_water));

    std::string junk;
    getline(std::cin, junk);

    int r;
    int c;
    char spot_type;
    while(std::cin >> r >> c >> spot_type) {
        grid[r][c].spot_type = spot_type;
        if(grid[r][c].spot_type == '@') {
            current_r = r;
            current_c = c;
        }
    }
}

void Hunt::read_map_file() {
    std::cin >> map_size;
    std::string row;
    
    int r_count = 0;

    Spot default_water{'.', no_where};
    grid.resize(map_size, std::vector<Spot>(map_size, default_water));

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
}

int Hunt::calculate_path_length() {
    // start backtracing algorithm
    Location visit{treasure_r, treasure_c};
    Location start{-1, -1};
    while(grid[visit.r][visit.c].came_from != start) {
        if(grid[visit.r][visit.c].came_from == north) {
            visit.r -= 1;
        } else if(grid[visit.r][visit.c].came_from == south) {
            visit.r += 1;
        } else if(grid[visit.r][visit.c].came_from == east) {
            visit.c += 1;
        } else if(grid[visit.r][visit.c].came_from == west) {
            visit.c -= 1;
        }
        path_length += 1;
    }
    return path_length;
}

Hunt::Hunt(std::string hunt_order_in, std::string captain_type_in, std::string firstmate_type_in, bool verbose_on_in, 
    bool show_path_on_in, bool stats_on_in, char path_type_in)
    : hunt_order(hunt_order_in), captain_type(captain_type_in), firstmate_type(firstmate_type_in), verbose_on(verbose_on_in), 
    show_path_on(show_path_on_in), stats_on(stats_on_in), path_type(path_type_in) {}

void Hunt::read_map() {
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
void Hunt::print_map() {
    for(int i = 0; i < map_size; ++i) {
        for(int j = 0; j < map_size; ++j) {
            std::cout << grid[i][j].spot_type;
        }
        std::cout << "\n";
    }
}

// the main search function. everything for the algorithm starts here
// search function
void Hunt::search() {
    // add starting point to search container, mark as visited, then pass on the work to queue or stack search
    // searches for treasure until treasure is found, or the search returns with no results

    // seeding the container
    captain_deque.push_back(current_location);
    grid[current_location.r][current_location.c].came_from = {-1, -1};
    // only set these in order to keep track for stats
    start_r = current_location.r;
    start_c = current_location.c;
    if(verbose_on) {
        std::cout << "Treasure hunt started at: " << current_location.r << "," << current_location.c << "\n";
    }

    // start the captain on the search process, looping until the search has ended, or treasure has been found (or both)
    if(captain_type == "STACK") {
        while(!search_ended && !treasure_found) {
            // look in all adjacent spaces, add to container, move to new location 
            stack_search();
        }
    } else {
        while(!search_ended && !treasure_found) {
            queue_search();
        }
    }
    // search has failed
    if(search_ended && !treasure_found && verbose_on) {
        std::cout << "Treasure hunt failed\n";
    }

    // print out stats if on
    if(stats_on) {
        std::cout << "--- STATS ---\nStarting location: " << start_r << "," << start_c << "\nWater locations investigated: " << water_locations << "\nLand locations investigated: "
            << land_locations << "\nWent ashore: " << went_ashore; 
            if(treasure_found) {
                std::cout << "\nPath length: " << calculate_path_length() << "\nTreasure location: " <<  treasure_r << "," << treasure_c << "\n"; 
            } else {
                std::cout << "\n";
            }
            std::cout << "--- STATS ---\n";
    }

    // path_on prints
    if(show_path_on && treasure_found) {
        if(path_type == 'M') {
            Location visit{treasure_r, treasure_c};
            Location start{-1, -1};
            grid[treasure_r][treasure_c].spot_type = 'X'; 
            while(grid[visit.r][visit.c].came_from != start) {
                if(grid[visit.r][visit.c].came_from == north) {
                    visit.r -= 1;
                    if(grid[visit.r][visit.c].came_from == north) {
                        grid[visit.r][visit.c].spot_type = '|';
                    } else {
                        grid[visit.r][visit.c].spot_type = '+';
                    }
                } else if(grid[visit.r][visit.c].came_from == south) {
                    visit.r += 1;
                    if(grid[visit.r][visit.c].came_from == south) {
                        grid[visit.r][visit.c].spot_type = '|';
                    } else {
                        grid[visit.r][visit.c].spot_type = '+';
                    }
                } else if(grid[visit.r][visit.c].came_from == east) {
                    visit.c += 1;
                    if(grid[visit.r][visit.c].came_from == east) {
                        grid[visit.r][visit.c].spot_type = '-';
                    } else {
                        grid[visit.r][visit.c].spot_type = '+';
                    }
                } else if(grid[visit.r][visit.c].came_from == west) {
                    visit.c -= 1;
                    if(grid[visit.r][visit.c].came_from == west) {
                        grid[visit.r][visit.c].spot_type = '-';
                    } else {
                        grid[visit.r][visit.c].spot_type = '+';
                    }
                }
                if(!stats_on) {
                    path_length += 1;
                }
            }
            grid[visit.r][visit.c].spot_type = '@';
            print_map();
        } else if(path_type == 'L') {
            std::cout << "Sail:\n";
            Location visit{treasure_r, treasure_c};
            Location start{-1, -1};
            Location starting_pos{start_r, start_c};
            std::vector<Location> path;
            if(!stats_on) {
                path.reserve(calculate_path_length() + 1);
            } else {
                path.reserve(path_length + 1);
            }
            while(grid[visit.r][visit.c].came_from != start) {
                if(grid[visit.r][visit.c].came_from == north) {
                    path.push_back(visit);
                    visit.r -= 1;
        
                } else if(grid[visit.r][visit.c].came_from == south) {
                    path.push_back(visit);
                    visit.r += 1;
                    
                } else if(grid[visit.r][visit.c].came_from == east) {
                    path.push_back(visit);
                    visit.c += 1;
                    
                } else if(grid[visit.r][visit.c].came_from == west) {
                    path.push_back(visit);
                    visit.c -= 1;
                    
                }
            }
            path.push_back(starting_pos);
            bool search_print = false;
            for(int i = int(path.size() - 1); i >= 0; --i) {
                if(!search_print) {
                    if(grid[path[i].r][path[i].c].spot_type == 'o') {
                    std::cout << "Search:\n";
                    search_print = true;
                    }
                }
                std::cout << path[i].r << "," << path[i].c << "\n";
            }
        }
    }
    // final results. always prints regardless of the mode
    if(treasure_found) {
        std::cout << "Treasure found at " << treasure_r << "," << treasure_c << " with path length ";
        // calculate path length unless previously calculated by path_on or stats (it has been initialized as zero so hasnt changed)
        if(path_length != 0) {
            std::cout << path_length << ".\n";
        } else {
            std::cout << calculate_path_length() << ".\n";   
        }             
    } else {
        std::cout << "No treasure found after investigating " << water_locations + land_locations << " locations.\n";
    }
}

//  searches using a queue. parameter passed in is the type of spot to search for (i.e. captain or firstmate)
void Hunt::queue_search() {
    if(!search_party) {
        while(!captain_deque.empty()) {
            // now look for adjacent locations, add to container if possible
            // SAIL LOCATION IS FRONT FOR QUEUE!!!
            check_adjacents_captain();
            if(!treasure_found && !search_ended) {
                water_locations += 1;
            }
            current_location = captain_deque.front();
            captain_deque.pop_front();
            
        }
    } else {
        // search party sent out:
        if(verbose_on) {
            std::cout << "Went ashore at: " << firstmate_location.r << "," << firstmate_location.c << "\n" << "Searching island... ";
        }
        went_ashore += 1;           
        while(!firstmate_deque.empty()) {
            //firstmate_deque.pop_front();
            firstmate_location = firstmate_deque.front();
            firstmate_deque.pop_front();
            if(grid[firstmate_location.r][firstmate_location.c].spot_type == '$') {
                treasure_r = firstmate_location.r;
                treasure_c = firstmate_location.c;
                if(verbose_on) {
                    std::cout << "party found treasure at " << firstmate_location.r << "," << firstmate_location.c << ".\n";
                    land_locations += 1;
                    //std::cout << "---DEBUG: Investigating treasure spot at: " << firstmate_location.r << "," << firstmate_location.c << "...\n";
                    
                }
                treasure_found = true;
                search_ended = true;
                break;
            }
            check_adjacents_firstmate();
        }
        if(!treasure_found && verbose_on) {
            std::cout << "party returned with no treasure.\n";
        }
        search_party = false;
    }
    // if no more places to go, end the search if treasure has not been found (i.e. sail container empty)
    if(firstmate_deque.empty() && captain_deque.empty()) {
        search_ended = true;
    }
}

void Hunt::stack_search() {
    // if captain is searching
    if(!search_party) {
        while(!captain_deque.empty()) {
            // now look for adjacent locations, add to container if possible
            // SAIL LOCATION IS BACK FOR STACK!!!
            current_location = captain_deque.back();
            captain_deque.pop_back();
            if(!treasure_found && !search_ended) {
                water_locations += 1;
            }
            check_adjacents_captain();  
        }
    } else {
        // search party sent out:
        if(verbose_on) {
            std::cout << "Went ashore at: " << firstmate_location.r << "," << firstmate_location.c << "\n" << "Searching island... ";
        }
        went_ashore += 1;
        while(!firstmate_deque.empty()) {
            firstmate_location = firstmate_deque.back();
            firstmate_deque.pop_back();
            if(grid[firstmate_location.r][firstmate_location.c].spot_type == '$') {
                treasure_r = firstmate_location.r;
                treasure_c = firstmate_location.c;
                if(verbose_on) {
                    std::cout << "party found treasure at " << firstmate_location.r << "," << firstmate_location.c << ".\n";
                    //std::cout << "---DEBUG: Investigating treasure spot at: " << firstmate_location.r << "," << firstmate_location.c << "...\n";
                    land_locations += 1;
                    
                }
                treasure_found = true;
                search_ended = true;
            }
            check_adjacents_firstmate();
        }
        // the firstmate hunt has ended
        if(!treasure_found && verbose_on) {
            std::cout << "party returned with no treasure.\n";
        }
        search_party = false;
    }
    // if no more places to go, end the search if treasure has not been found (i.e. sail container empty)
    if(firstmate_deque.empty() && captain_deque.empty()) {
        search_ended = true;
    }
}

void Hunt::check_adjacents_captain() {
    for(int i = 0; i < int(hunt_order.length()); ++i) {
        switch(hunt_order[i]) {
            case 'N':
            if(treasure_found || search_ended) {
                break;
            }
            // check bounds, visited, spot type 
                if(current_location.r + north.r >= 0) {
                    if((grid[current_location.r + north.r][current_location.c].came_from == no_where) && (grid[current_location.r + north.r][current_location.c].spot_type != '#')) {
                        // land found, start a subsearch using a queue or stack
                        if((grid[current_location.r + north.r][current_location.c].spot_type == 'o') || (grid[current_location.r + north.r][current_location.c].spot_type == '$')) {
                            // search party has been sent out (information is needed for stack_search or queue_search functions)
                            search_party = true;
                            // new location to be pushed to stack/queue
                            Location new_land{current_location.r + north.r, current_location.c};
                            grid[new_land.r][new_land.c].came_from = south;
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type == "QUEUE") {
                                queue_search();
                            } else {
                                stack_search();
                            }
                        } else {
                            // not land, valid water spot on map!
                            Location new_water{current_location.r + north.r, current_location.c};
                            grid[new_water.r][new_water.c].came_from = south;
                            captain_deque.push_back(new_water);
                        }
                    }
                }
                break;
            case 'E':
                if(treasure_found || search_ended) {
                    break;
                }
                if(current_location.c + east.c < map_size) {
                    if((grid[current_location.r][current_location.c + east.c].came_from == no_where) && (grid[current_location.r][current_location.c + east.c].spot_type != '#')) {
                        // check if land has been found
                        if((grid[current_location.r][current_location.c + east.c].spot_type == 'o') || (grid[current_location.r][current_location.c + east.c].spot_type == '$')) {
                            search_party = true;
                            Location new_land{current_location.r, current_location.c + east.c};
                            grid[new_land.r][new_land.c].came_from = west;
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type == "QUEUE") {
                                queue_search();
                            } else {
                                stack_search();
                            }
                        } else {
                            // not land, valid water spot
                            Location new_water{current_location.r, current_location.c + east.c};
                            grid[new_water.r][new_water.c].came_from = west;
                            captain_deque.push_back(new_water);
                        }
                    }
                }
                break;
            case 'S':
                if(treasure_found || search_ended) {
                break;
                }
                // check bounds, visited, spot type 
                if(current_location.r + south.r < map_size) {
                    if((grid[current_location.r + south.r][current_location.c].came_from == no_where) && (grid[current_location.r + south.r][current_location.c].spot_type != '#')) {
                        // land found, start a subsearch using a queue or stack
                        if((grid[current_location.r + south.r][current_location.c].spot_type == 'o') || (grid[current_location.r + south.r][current_location.c].spot_type == '$')) {
                            search_party = true;
                            Location new_land{current_location.r + south.r, current_location.c};
                            grid[new_land.r][new_land.c].came_from = north;
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type == "QUEUE") {
                                queue_search();
                            } else {
                                stack_search();
                            }
                        } else {
                            // not land, valid water spot on map!
                            Location new_water{current_location.r + south.r, current_location.c};
                            grid[new_water.r][new_water.c].came_from = north;
                            captain_deque.push_back(new_water);
                        }   
                    }
                }
                break;
            case 'W':
                if(treasure_found || search_ended) {
                break;
                }
                if(current_location.c + west.c >= 0) {
                    if((grid[current_location.r][current_location.c + west.c].came_from == no_where) && (grid[current_location.r][current_location.c + west.c].spot_type != '#')) {
                        // check if land has been found
                        if((grid[current_location.r][current_location.c + west.c].spot_type == 'o') || (grid[current_location.r][current_location.c + west.c].spot_type == '$')) {
                            search_party = true;
                            Location new_land{current_location.r, current_location.c + west.c};
                            grid[new_land.r][new_land.c].came_from = east;
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type == "QUEUE") {
                                queue_search();
                            } else {
                                stack_search();
                            }
                        } else {
                            // not land, valid water spot
                            Location new_water{current_location.r, current_location.c + west.c};
                            grid[new_water.r][new_water.c].came_from = east;
                            captain_deque.push_back(new_water);
                        }
                    }
                }
                break;

            default:
                std::cout << "ERROR: No such direction.\n";
                break;
        }
    }
}

// change to look for treasure, ignore water, etc. 
void Hunt::check_adjacents_firstmate() {
    if(!treasure_found && !search_ended) {
        //std::cout << "---DEBUG: Investigating land location at: " << firstmate_location.r << "," << firstmate_location.c << "...\n";
        land_locations += 1;
    }
    for(int i = 0; i < int(hunt_order.length()); ++i) {
        switch(hunt_order[i]) {
            case 'N':
                if(firstmate_location.r + north.r >= 0) {
                    if((grid[firstmate_location.r + north.r][firstmate_location.c].came_from == no_where) && (grid[firstmate_location.r + north.r][firstmate_location.c].spot_type != '#') 
                    && (grid[firstmate_location.r + north.r][firstmate_location.c].spot_type != '.')) {
                        Location new_land{firstmate_location.r + north.r, firstmate_location.c};
                        grid[new_land.r][new_land.c].came_from = south;
                        firstmate_deque.push_back(new_land);
                    }     
                }
                break;
            case 'E':
                if(firstmate_location.c + east.c < map_size) {
                    if((grid[firstmate_location.r][firstmate_location.c + east.c].came_from == no_where) && (grid[firstmate_location.r][firstmate_location.c + east.c].spot_type != '#') 
                    && (grid[firstmate_location.r][firstmate_location.c + east.c].spot_type != '.')) {
                        Location new_land{firstmate_location.r, firstmate_location.c + east.c};
                        grid[new_land.r][new_land.c].came_from = west;
                        firstmate_deque.push_back(new_land);
                        }
                }
                break;
            case 'S':
                if(firstmate_location.r + south.r < map_size) {
                    if((grid[firstmate_location.r + south.r][firstmate_location.c].came_from == no_where) && (grid[firstmate_location.r + south.r][firstmate_location.c].spot_type != '#') 
                    && (grid[firstmate_location.r + south.r][firstmate_location.c].spot_type != '.')) {
                        Location new_land{firstmate_location.r + south.r, firstmate_location.c};
                        grid[new_land.r][new_land.c].came_from = north;
                        firstmate_deque.push_back(new_land);
                    }     
                }
                break;
            case 'W':
                if(firstmate_location.c + west.c >= 0) {
                    if((grid[firstmate_location.r][firstmate_location.c + west.c].came_from == no_where) && (grid[firstmate_location.r][firstmate_location.c + west.c].spot_type != '#') 
                    && (grid[firstmate_location.r][firstmate_location.c + west.c].spot_type != '.')) {
                        Location new_land{firstmate_location.r, firstmate_location.c + west.c};
                        grid[new_land.r][new_land.c].came_from = east;
                        firstmate_deque.push_back(new_land);
                    }
                }
                break;
            default:
                std::cout << "ERROR: No such direction." << "\n";
                exit(1);
                break;
        }
    }
}