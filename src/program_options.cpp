#include <iostream>
#include <cstring>

#include "program_options.h"

namespace {
    bool _verbose = false;
    bool _recursive = false;
    bool _help = false;

    char* _dir_name = nullptr;
}

void program_options::parse(int argc, char **argv) {
    bool assigned_dir_name = false;

    for (int i = 1; i < argc; i++) {
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
        } else {
            // Ensure there is only 1 name specified.
            // This could in the future be expanded, but for now this is a good safe starting-point.
            if (assigned_dir_name) {
                program_options::cleanup();
                throw std::runtime_error("Can only handle one directory at a time.");
            }
            _dir_name = new char[std::strlen(argument) + 1];
            std::strcpy(_dir_name, argument);
            assigned_dir_name = true;
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

char* program_options::dir_name() {
    return _dir_name;
}

void program_options::cleanup() {
    delete[] _dir_name;
}