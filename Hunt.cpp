// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles the search process
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include "Hunt.h"

void Hunt::read_list_file() {
    std::cin >> map_size;

    Spot default_water{'.', 'B'};
    grid.resize((unsigned long)map_size, std::vector<Spot>((unsigned long)map_size, default_water));

    std::string junk;
    getline(std::cin, junk);

    int r;
    int c;
    char spot_type;
    while(std::cin >> r >> c >> spot_type) {
        grid[(unsigned long)r][(unsigned long)c].spot_type = spot_type;
        if(grid[(unsigned long)r][(unsigned long)c].spot_type == '@') {
            current_r = r;
            current_c = c;
        }
    }
}

void Hunt::read_map_file() {
    std::cin >> map_size;
    std::string row;
    
    int r_count = 0;

    Spot default_water{'.', 'B'};
    grid.resize((unsigned long)map_size, std::vector<Spot>((unsigned long)map_size, default_water));

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
            grid[(unsigned long)r_count][(unsigned long)c_count].spot_type = c;
            ++c_count;
        }
        ++r_count;
    }   
}

int Hunt::calculate_path_length() {
    // start backtracing algorithm
    Location visit{treasure_r, treasure_c};
    while(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from != 'A') {
        if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'N') {
            visit.r -= 1;
        } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'S') {
            visit.r += 1;
        } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'E') {
            visit.c += 1;
        } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'W') {
            visit.c -= 1;
        }
        path_length += 1;
    }
    return path_length;
}

Hunt::Hunt(std::string hunt_order_in, bool captain_type_in, bool firstmate_type_in, bool verbose_on_in, 
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
            std::cout << grid[(unsigned long)i][(unsigned long)j].spot_type;
        }
        std::cout << "\n";
    }
}

// the main search function. everything for the algorithm starts here
// search function
void Hunt::search() {
    std::deque<Location> captain_deque;
    std::deque<Location> firstmate_deque;
    // add starting point to search container, mark as visited, then pass on the work to queue or stack search
    // searches for treasure until treasure is found, or the search returns with no results

    // seeding the container
    captain_deque.push_back(current_location);
    grid[(unsigned long)current_location.r][(unsigned long)current_location.c].came_from = 'A';
    // only set these in order to keep track for stats
    start_r = current_location.r;
    start_c = current_location.c;
    if(verbose_on) {
        std::cout << "Treasure hunt started at: " << current_location.r << "," << current_location.c << "\n";
    }

    // start the captain on the search process, looping until the search has ended, or treasure has been found (or both)
    if(!captain_type) {
        while(!search_ended && !treasure_found) {
            // look in all adjacent spaces, add to container, move to new location 
            deque_search(0, captain_deque, firstmate_deque);
        }
    } else {
        while(!search_ended && !treasure_found) {
            deque_search(1, captain_deque, firstmate_deque);
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

            grid[(unsigned long)treasure_r][(unsigned long)treasure_c].spot_type = 'X'; 
            while((visit.r != start_r) || (visit.c != start_c)) {
                if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'A') {
                    break;
                }
                if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'N') {
                    visit.r -= 1;
                    if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'N') {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '|';
                    } else {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '+';
                    }
                } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'S') {
                    visit.r += 1;
                    if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'S') {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '|';
                    } else {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '+';
                    }
                } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'E') {
                    visit.c += 1;
                    if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'E') {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '-';
                    } else {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '+';
                    }
                } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'W') {
                    visit.c -= 1;
                    if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'W') {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '-';
                    } else {
                        grid[(unsigned long)visit.r][(unsigned long)visit.c].spot_type = '+';
                    }
                }
                if(!stats_on) {
                    path_length += 1;
                }
            }
            grid[(unsigned long)start_r][(unsigned long)start_c].spot_type = '@';
            print_map();
        } else if(path_type == 'L') {
            std::cout << "Sail:\n";
            Location visit{treasure_r, treasure_c};
            Location starting_pos{start_r, start_c};
            // change to locations below here !!!!!!!!!!!!!!!!!!!!!!!!!
            std::vector<Location> path;
            if(!stats_on) {
                path.reserve((unsigned long)calculate_path_length() + 1);
            } else {
                path.reserve((unsigned long)path_length + 1);
            }
            if(path_length == 0) {
                std::cout << start_r << "," << start_c << "\n";
                std::cout << "Search:\n" << treasure_r << "," << treasure_c << "\n";
            } else {

                while(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from != 'A') {
                    if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'N') {
                        path.push_back(visit);
                        visit.r -= 1;
            
                    } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'S') {
                        path.push_back(visit);
                        visit.r += 1;
                        
                    } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'E') {
                        path.push_back(visit);
                        visit.c+= 1;
                        
                    } else if(grid[(unsigned long)visit.r][(unsigned long)visit.c].came_from == 'W') {
                        path.push_back(visit);
                        visit.c -= 1;
                    }
                }
                path.push_back(starting_pos);
                bool search_print = false;
                for(int i = int(path.size() - 1); i >= 0; --i) {
                    if(!search_print) {
                        if(grid[(unsigned long)path[(unsigned long)i].r][(unsigned long)path[(unsigned long)i].c].spot_type == 'o' || grid[(unsigned long)path[(unsigned long)i].r][(unsigned long)path[(unsigned long)i].c].spot_type == '$') {        
                        std::cout << "Search:\n";
                        search_print = true;
                        }
                    }
                    std::cout << path[(unsigned long)i].r << "," << path[(unsigned long)i].c << "\n";
                }
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
// true is queue, false is stack
void Hunt::deque_search(bool deque_type, std::deque<Location> &captain_deque, std::deque<Location> &firstmate_deque) {
    if(!search_party) {
        while(!captain_deque.empty()) {
            if(treasure_found) {
                break;
            }
            // RUNS IF QUEUE BEING USED
            if(deque_type == true) {
                // NOTE: JUST CHANGED THE ORDER OF THESE
                current_location = captain_deque.front();
                captain_deque.pop_front();
                // END OF CHANGE (look at commented out code)
                if(!search_ended && !treasure_found) {
                    check_adjacents_captain(captain_deque, firstmate_deque); 
                }
                water_locations += 1;
                if(treasure_found) {
                    return;
                }
                // current_location = captain_deque.front();
                // captain_deque.pop_front();
            // RUNS IF STACK BEING USED
            } else {
                current_location = captain_deque.back();
                captain_deque.pop_back();
                water_locations += 1;
                if(treasure_found) {
                    return;
                }
                if(!search_ended && !treasure_found) {
                    check_adjacents_captain(captain_deque, firstmate_deque); 
                } 
            }       
        }  
    // A SEARCH PARTY HAS BEEN SENT OUT 
    } else {
        if(verbose_on) {
            std::cout << "Went ashore at: " << firstmate_location.r << "," << firstmate_location.c << "\n" << "Searching island... ";
        }
        went_ashore += 1;

        while(!firstmate_deque.empty()) {
            // RUNS IF FIRST TILE FOUND IS TREASURE
            if(!treasure_found && grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c].spot_type == '$') {
                treasure_has_been_found(grid[(unsigned long)current_location.r][(unsigned long)current_location.c].came_from, grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c].came_from, true);
            }
            if(treasure_found) {
                break;
            }
            // RUNS IF QUEUE BEING USED
            if(deque_type == true) {
                firstmate_location = firstmate_deque.front();
                firstmate_deque.pop_front();
                if(!treasure_found) {
                    land_locations += 1;
                }
            // RUNS IF STACK BEING USED
            } else {
                firstmate_location = firstmate_deque.back();
                firstmate_deque.pop_back();
                if(!treasure_found) {
                    land_locations += 1;
                }
            }
            if(!search_ended && !treasure_found) {
                check_adjacents_firstmate(firstmate_deque);
            }
        }
        if(!treasure_found && verbose_on) {
            std::cout << "party returned with no treasure.\n";
        }
        search_party = false;
    }
    if(firstmate_deque.empty() && captain_deque.empty()) {
        search_ended = true;
    }
}

void Hunt::check_adjacents_captain(std::deque<Location> &captain_deque, std::deque<Location> &firstmate_deque) {
    for(int i = 0; i < int(hunt_order.length()); ++i) {
        switch(hunt_order[(unsigned long)i]) {
            case 'N':
            if(treasure_found || search_ended) {
                break;
            }
            // check bounds, visited, spot type 
                if(current_location.r -1 >= 0) {
                    if((grid[(unsigned long)current_location.r -1][(unsigned long)current_location.c].came_from == 'B') && (grid[(unsigned long)current_location.r -1][(unsigned long)current_location.c].spot_type != '#')) {
                        // land found, start a subsearch using a queue or stack
                        if((grid[(unsigned long)current_location.r -1][(unsigned long)current_location.c].spot_type == 'o') || (grid[(unsigned long)current_location.r -1][(unsigned long)current_location.c].spot_type == '$')) {
                            // search party has been sent out (information is needed for stack_search or queue_search functions)
                            search_party = true;
                            // new location to be pushed to stack/queue
                            Location new_land{current_location.r -1, current_location.c};
                            grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'S';
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type) {
                                deque_search(1, captain_deque, firstmate_deque);
                            } else {
                                deque_search(0, captain_deque, firstmate_deque);
                            }
                        } else {
                            // not land, valid water spot on map!
                            Location new_water{current_location.r - 1, current_location.c};
                            grid[(unsigned long)new_water.r][(unsigned long)new_water.c].came_from = 'S';
                            captain_deque.push_back(new_water);
                        }
                    }
                }
                break;
            case 'E':
                if(treasure_found || search_ended) {
                    break;
                }
                if(current_location.c + 1 < map_size) {
                    if((grid[(unsigned long)current_location.r][(unsigned long)current_location.c + 1].came_from == 'B') && (grid[(unsigned long)current_location.r][(unsigned long)current_location.c + 1].spot_type != '#')) {
                        // check if land has been found
                        if((grid[(unsigned long)current_location.r][(unsigned long)current_location.c + 1].spot_type == 'o') || (grid[(unsigned long)current_location.r][(unsigned long)current_location.c + 1].spot_type == '$')) {
                            search_party = true;
                            Location new_land{current_location.r, current_location.c + 1};
                            grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'W';
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type) {
                                deque_search(1, captain_deque, firstmate_deque);
                            } else {
                                deque_search(0, captain_deque, firstmate_deque);
                            }
                        } else {
                            // not land, valid water spot
                            Location new_water{current_location.r, current_location.c + 1};
                            grid[(unsigned long)new_water.r][(unsigned long)new_water.c].came_from = 'W';
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
                if(current_location.r + 1 < map_size) {
                    if((grid[(unsigned long)current_location.r + 1][(unsigned long)current_location.c].came_from == 'B') && (grid[(unsigned long)current_location.r + 1][(unsigned long)current_location.c].spot_type != '#')) {
                        // land found, start a subsearch using a queue or stack
                        if((grid[(unsigned long)current_location.r + 1][(unsigned long)current_location.c].spot_type == 'o') || (grid[(unsigned long)current_location.r + 1][(unsigned long)current_location.c].spot_type == '$')) {
                            search_party = true;
                            Location new_land{current_location.r + 1, current_location.c};
                            grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'N';
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type) {
                                deque_search(1, captain_deque, firstmate_deque);
                            } else {
                                deque_search(0, captain_deque, firstmate_deque);
                            }
                        } else {
                            // not land, valid water spot on map!
                            Location new_water{current_location.r + 1, current_location.c};
                            grid[(unsigned long)new_water.r][(unsigned long)new_water.c].came_from = 'N';
                            captain_deque.push_back(new_water);
                        }   
                    }
                }
                break;
            case 'W':
                if(treasure_found || search_ended) {
                break;
                }
                if(current_location.c - 1 >= 0) {
                    if((grid[(unsigned long)current_location.r][(unsigned long)current_location.c - 1].came_from == 'B') && (grid[(unsigned long)current_location.r][(unsigned long)current_location.c - 1].spot_type != '#')) {
                        // check if land has been found
                        if((grid[(unsigned long)current_location.r][(unsigned long)current_location.c - 1].spot_type == 'o') || (grid[(unsigned long)current_location.r][(unsigned long)current_location.c - 1].spot_type == '$')) {
                            search_party = true;
                            Location new_land{current_location.r, current_location.c - 1};
                            grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'E';
                            firstmate_location = new_land;
                            firstmate_deque.push_back(new_land);
                            if(firstmate_type) {
                                deque_search(1, captain_deque, firstmate_deque);
                            } else {
                                deque_search(0, captain_deque, firstmate_deque);
                            }
                        } else {
                            // not land, valid water spot
                            Location new_water{current_location.r, current_location.c - 1};
                            grid[(unsigned long)new_water.r][(unsigned long)new_water.c].came_from = 'E';
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
void Hunt::check_adjacents_firstmate(std::deque<Location> &firstmate_deque) {
    for(int i = 0; i < int(hunt_order.length()); ++i) {
        switch(hunt_order[(unsigned long)i]) {
            case 'N':
            if(search_ended || treasure_found) {
                break;
            }
                if(firstmate_location.r - 1 >= 0) {
                    if(grid[(unsigned long)firstmate_location.r - 1][(unsigned long)firstmate_location.c].spot_type == '$') {
                        treasure_has_been_found('N', 'S', false);
                        if(treasure_found) {
                            return;
                        }
                        break;
                    }
                    if((grid[(unsigned long)firstmate_location.r - 1][(unsigned long)firstmate_location.c].came_from == 'B') && 
                    (grid[(unsigned long)firstmate_location.r - 1][(unsigned long)firstmate_location.c].spot_type == 'o')) {
                        Location new_land{firstmate_location.r - 1, firstmate_location.c};
                        grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'S';
                        firstmate_deque.push_back(new_land);
                    }     
                }
                break;
            case 'E':
                if(search_ended || treasure_found) {
                    break;
                }
                if(firstmate_location.c + 1 < map_size) {
                    if(grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c + 1].spot_type == '$') {
                        treasure_has_been_found('E', 'W', false);
                        if(treasure_found) {
                            return;
                        }
                        break;
                    }
                    if((grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c + 1].came_from == 'B') && 
                    (grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c + 1].spot_type == 'o')) {
                        Location new_land{firstmate_location.r, firstmate_location.c + 1};
                        grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'W';
                        firstmate_deque.push_back(new_land);
                        }
                }
                break;
            case 'S':
                if(search_ended || treasure_found) {
                    break;
                }
                if(firstmate_location.r + 1 < map_size) {
                    if(grid[(unsigned long)firstmate_location.r + 1][(unsigned long)firstmate_location.c].spot_type == '$') {
                        treasure_has_been_found('S', 'N', false);
                        if(treasure_found) {
                            return;
                        }
                        break;
                    }
                    if((grid[(unsigned long)firstmate_location.r + 1][(unsigned long)firstmate_location.c].came_from == 'B') && 
                    (grid[(unsigned long)firstmate_location.r + 1][(unsigned long)firstmate_location.c].spot_type == 'o')) {
                        Location new_land{firstmate_location.r + 1, firstmate_location.c};
                        grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'N';
                        firstmate_deque.push_back(new_land);
                    }     
                }
                break;
            case 'W':
                if(search_ended || treasure_found) {
                    break;
                }
                if(firstmate_location.c - 1 >= 0) {
                    if(grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c - 1].spot_type == '$') {
                        treasure_has_been_found('W', 'E', false);
                        if(treasure_found) {
                            return;
                        }
                        break;
                    }
                    if((grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c - 1].came_from == 'B') 
                    && (grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c - 1].spot_type == 'o')) {
                        Location new_land{firstmate_location.r, firstmate_location.c - 1};
                        grid[(unsigned long)new_land.r][(unsigned long)new_land.c].came_from = 'E';
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

void Hunt::treasure_has_been_found(char treasure_direction, char came_from_spot, bool initial_land) {
    land_locations += 1;
    if(initial_land) {
        grid[(unsigned long)firstmate_location.r][(unsigned long)firstmate_location.c].came_from = came_from_spot;
        treasure_r = firstmate_location.r;
        treasure_c = firstmate_location.c;
        if(verbose_on) {
            std::cout << "party found treasure at " << firstmate_location.r << "," << firstmate_location.c << ".\n";
        }
    } else {
        int temp_r = 0;
        int temp_c = 0;
        if(treasure_direction == 'N') {
            temp_r = -1;
        } else if(treasure_direction == 'E') {
            temp_c = 1;
        } else if(treasure_direction == 'S') {
            temp_r = 1;
        } else {
            temp_c = -1;
        }
        grid[(unsigned long)firstmate_location.r + (unsigned long)temp_r][(unsigned long)firstmate_location.c + (unsigned long)temp_c].came_from = came_from_spot;
        treasure_r = firstmate_location.r + temp_r;
        treasure_c = firstmate_location.c + temp_c;
        if(verbose_on) {
            std::cout << "party found treasure at " << firstmate_location.r + temp_r << "," << firstmate_location.c + temp_c << ".\n";
        }
    }

    treasure_found = true;
    search_ended = true;
}
