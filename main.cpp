// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// EECS 281 Project 1
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include "Hunt.h"

using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    Option_handler oh;
    oh.get_options(argc, argv); 

    // TODO: check that all member functions of Option_handler return the correct output when given certain arguments
    read_map();
}
