// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// EECS 281 Project 1
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    string mode;
    vector<double> data;

    mode = getMode(argc, argv);

    if (mode == "resize")
        readWithResize(data);
    else if (mode == "reserve")
        readWithReserve(data);
    else if (mode == "nosize")
        readWithNoSize(data);
    else {
        cerr << "Invalid mode \"" << mode << "\" specified, should not be possible to reach this point!" << endl;
        return 1;
    }
    return 0;
} // main()
