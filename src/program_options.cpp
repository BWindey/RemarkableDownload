#include <iostream>

#include "program_options.h"

namespace {
    bool _verbose = false;
    bool _recursive = false;
    bool _help = false;
}

void program_options::parse(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        char* argument = argv[i];
        if (argument[0] == '-') {
            while(*argument != '\0') {
                switch (*argument) {
                    case 'v':
                        _verbose = true;
                        std::cout << "Verbose is set to true\n";
                        break;

                    case 'r':
                        _recursive = true;
                        std::cout << "Recursive is set to true\n";
                        break;

                    case 'h':
                        _help = true;
                        std::cout << "Help is set to true\n";
                        break;
                }

                argument++;
            }
        }

    }
}

bool program_options::verbose() {
    return _verbose;
}

bool program_options::recursive() {
    return _recursive;
}

bool program_options::help() {
    return _help;
}
