
// IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
// handles args and input for our program
#include <iostream>
#include <string>
#include <getopt.h>

class Option_handler {

private:
    std::string captain_type = "STACK";
    std::string firstmate_type = "QUEUE";
    std::string hunt_order = "NESW";
    bool valid_nesw = true;
    bool verbose_mode = false;
    bool stats_mode = false;
    bool show_path = false;
    std::string path_type;

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
                captain_type = optarg;
                if (captain_type != "STACK" && captain_type != "QUEUE") {
                    std::cerr << "Invalid argument to --captain\n";
                    std::cerr << "  I don't know recognize: " << captain_type << "\n";
                    exit(1);
                }
                break;
            case 'f':
                firstmate_type = optarg;
                if (firstmate_type != "STACK" && firstmate_type != "QUEUE") {
                    std::cerr << "Invalid argument to --first-mate\n";
                    std::cerr << "  I don't know recognize: " << firstmate_type << "\n";
                    exit(1);
                }
                break;
            case 'o':
                // TODO: check if show path has been specified multiple times
                hunt_order = optarg;
                // sorting is necessary for unique test
                std::sort(hunt_order.begin(), hunt_order.end());
                // check that there are only 4 chars and each is unique
                // TODO: come back and simplify logic somehow. seems messy and first if statement should break before checking each char 
                if(hunt_order.length() != 4 || std::unique(hunt_order.begin(), hunt_order.end()) != hunt_order.end()) {
                    valid_nesw = false;
                }
                // check they are the right chars 
                for(char a: hunt_order) {
                    if(a != 'N' && a != 'E' && a != 'S' && a != 'W') {
                        valid_nesw = false;
                        break;
                    }
                }
                if(!valid_nesw) {
                    std::cerr << "Invalid argument to --hunt-order\n";
                    std::cerr << "  I don't know recognize: " << hunt_order << "\n";
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
                path_type = optarg;
                if(path_type != "M" && path_type != "L") {
                    std::cerr << "Invalid argument to --show-path\n";
                    std::cerr << "  I don't recognize: " << path_type << "\n";
                    exit(1);
                }
                show_path = true;
                break;

            default:
                std::cerr << "Unknown option" << std::endl;
                exit(1);
            }
        }
    }

    // functions
    void print_help() {
        std::cout << "usage: ./hunt [options] < inputfile\n";

    }

    std::string get_captain_type() {
        return captain_type;
    }

    std::string get_firstmate_type() {
        return firstmate_type;
    }

    std::string get_hunt_order() {
        return hunt_order;
    }

    bool verbose_on() {
        return verbose_mode;
    }

    bool stats_on() {
        return stats_mode;
    }

    bool show_path_on() {
        return show_path;
    }

};

// implementation for reading in a map
// vector<vector<point>> 
// point: struct with land_type, been_visited

