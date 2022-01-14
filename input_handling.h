// TODO:

/*

***delete line when option exists and way to check***

--captain <"QUEUE"|"STACK">, -c <"QUEUE"|"STACK">: The route-finding container
used while sailing in the water (if unspecified, container default is stack)
--first-mate <"QUEUE"|"STACK">, -f <"QUEUE"|"STACK">: The route-finding
container used while searching on land (if unspecified, container default is
queue)
--hunt-order <ORDER>, -o <ORDER>: The order of discovery of adjacent
tiles around the current location, a four character string using exactly one
of each of the four characters 'N', 'E', 'S', and 'W' (if unspecified, the
default order is: North->East->South->West)
--verbose, -v: Print verbose output while searching
--stats, -s: Display statistics after the search is complete
--show-path <M|L>, -p <M|L>: Display a treasure map or the list of locations
that describe the path



*/



// handles args and input for our program
#include <iostream>
#include <string>
#include <getopt.h>

class Option_handler {

private:
    int num_args;
    char *arg_list[];

    // captain and first-mate container types
    // boolean value: stack = 0; queue = 1
    // set to default values unless specified in argument list
    
    bool captain_type = 0;
    bool firstmate_type = 1;

public:
    // only one type of ctor because we need argc and argv

    Option_handler(int argc, char *argv[])
    : num_args(argc) {
        // copy contents passed into handler array to 
        // our class variable

        for(int i = 0; i < argc; ++i) {
            *arg_list[i] = *argv[i];
        } // for
     } // option handler

    // return the mode of the program
    void get_mode(int argc, char * argv[]) {
        //bool modeSpecified = false;
        std::string mode;

        opterr = false; 
        int choice;
        int option_index = 0;
        option long_options[] = {
            
            { "mode",  required_argument, nullptr, 'm'  },
            { "help",  no_argument,       nullptr, 'h'  },
            {"captain", required_argument, nullptr, 'c'},
            {"first-mate", required_argument, nullptr, 'f'},
            { nullptr, 0,                 nullptr, '\0' }
        };

        // Remember: m is followed by a colon because it has a required argument after it, h does not.
        while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
            switch (choice) {
            case 'h':
                print_help(arg_list);
                exit(0);

            case 'm':
                mode = optarg;
                if (mode != "resize" && mode != "reserve" && mode != "nosize") {
                    // The first line of error output has to be a 'fixed' message for the autograder
                    // to show it to you.
                    std::cerr << "Error: invalid mode" << std::endl;
                    // The second line can provide more information, but you won't see it on the AG.
                    std::cerr << "  I don't know recognize: " << mode << std::endl;
                    exit(1);
                } // if
                modeSpecified = true;
                break;

            default:
                std::cerr << "Error: invalid option" << std::endl;
                exit(1);
            } // switch
        } // while

        if (!modeSpecified) {
            std::cerr << "Error: no mode specified" << endl;
            exit(1);
        } // if

        return mode;
    } // get_mode

    void print_help(char *argv[]) {
        std::cout << "usage: ./hunt [options] < inputfile" << std::endl;

    } // printHelp()

    bool get_captain_type() {
        return captain_type;
    }

    bool get_firstmate_type() {
        return firstmate_type;
    }



};

