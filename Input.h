// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles args and input for our program
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <getopt.h>

// class to handle all options and args provided

class Option_handler {

private:
    // true is queue false is stack
    std::string captain_type_string = "STACK";
    std::string firstmate_type_string = "QUEUE";
    bool captain_type = 0;
    bool firstmate_type = 1;
    std::string hunt_order = "NESW";
    std::string temp;
    bool valid_nesw = true;
    bool verbose_mode = false;
    bool stats_mode = false;
    bool show_path = false;
    bool show_path_specified = false;
    char path_type;

    // print help function
    void print_help() {
        std::cout << "usage: ./hunt [options] < inputfile\n";
    }

public:
    // read in all of the options
    // (make seperate function getters to show modes, etc.)
    void get_options(int argc, char *argv[]) {

        opterr = false; 
        int choice;
        int option_index = 0;
        option long_options[] = {
            { "help",  no_argument,       nullptr, 'h'  },
            {"captain", required_argument, nullptr, 'c'},
            {"first-mate", required_argument, nullptr, 'f'},
            {"hunt-order", required_argument, nullptr, 'o'},
            {"verbose", no_argument, nullptr, 'v'},
            {"stats", no_argument, nullptr, 's'},
            {"show-path", required_argument, nullptr, 'p'},
            { nullptr, 0,                 nullptr, '\0' }
        };

        while ((choice = getopt_long(argc, argv, "hc:f:o:vsp:", long_options, &option_index)) != -1) {
            switch (choice) {
                case 'h':
                    print_help();
                    exit(0);
                    break;

                case 'c':
                    captain_type_string = optarg;
                    if (captain_type_string != "STACK" && captain_type_string != "QUEUE") {
                        std::cerr << "Invalid argument to --captain\n";
                        exit(1);
                    }
                    // set captain type bool
                    if(captain_type_string == "STACK") {
                        captain_type = 0;
                    } else {
                        captain_type = 1;
                    }
                    break;
                case 'f':
                    firstmate_type_string = optarg;
                    if (firstmate_type_string != "STACK" && firstmate_type_string != "QUEUE") {
                        std::cerr << "Invalid argument to --first-mate\n";
                        exit(1);
                    }
                    if(firstmate_type_string == "QUEUE") {
                        firstmate_type = 1;
                    } else {
                        firstmate_type = 0;
                    }
                    break;
                case 'o':
                    // TODO: check if show path has been specified multiple times
                    hunt_order = optarg;
                    // sorting is necessary for unique test
                    temp = "";
                    // check they are the right chars
                    if(hunt_order.length() != 4) {
                        std::cerr << "Invalid argument to --hunt-order\n";
                        exit(1);
                    } 
                    for(char a: hunt_order) {
                        if(((a != 'N') && (a != 'E') && (a != 'S') && (a != 'W')) || (temp.find(a) != std::string::npos)) {
                            valid_nesw = false;
                            break;
                        }
                        temp += a;
                    }
                    if(!valid_nesw) {
                        std::cerr << "Invalid argument to --hunt-order\n";
                        exit(1);
                    }
                    break;
                case 'v':
                    verbose_mode = true;
                    break;
                case 's':
                    stats_mode = true;
                    break;
                case 'p':
                    if(show_path_specified) {
                        std::cerr << "Specify --show-path only once\n";
                        exit(1);
                    }
                    show_path_specified = true;
                    if(std::string(optarg).length() != 1) {
                        std::cerr << "Invalid argument to --show-path\n";
                        exit(1);
                    }
                    path_type = *optarg;
                    if(path_type != 'L' && path_type != 'M') {
                        std::cerr << "Invalid argument to --show-path\n";
                        exit(1);
                    }
                    show_path = true;
                    break;

                default:
                    std::cerr << "Unknown option\n";
                    exit(1);
            }
        }
    }
    // functions to return options
    bool get_captain_type() { return captain_type; }

    bool get_firstmate_type() { return firstmate_type; }

    std::string get_hunt_order() { return hunt_order; }

    char get_path_type() { return path_type; }

    bool verbose_on() { return verbose_mode; }

    bool stats_on() { return stats_mode; }

    bool show_path_on() { return show_path; }
};