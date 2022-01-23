// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// EECS 281 Project 1
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Hunt.h"
using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    Option_handler oh;
    oh.get_options(argc, argv);

    Hunt hunt(oh.get_hunt_order(), oh.get_captain_type(), oh.get_firstmate_type(), oh.verbose_on(), oh.show_path_on(), oh.stats_on(), oh.get_path_type());
    hunt.read_map();
    hunt.search();
}