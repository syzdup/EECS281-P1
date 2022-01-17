// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// EECS 281 Project 1
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include "Input.h"

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

    // read the map into private data structure of option handler
    oh.read_map();

    // print_map() test run
    oh.print_map();

    // now comes the actual hunt algorithm, not sure if i should put it into main
    // unsure if i should be using two deques
    deque<Spot> captain_deque;
    deque<Spot> firstmate_deque;

}
