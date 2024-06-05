#include <iostream>
#include <cstring>

#include "program_options.h"

namespace {
    bool _verbose = false;
    bool _recursive = false;

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
                        break;

                    case 'r':
                        _recursive = true;
                        break;

                    case 'h':
                        program_options::help(argv[0]);
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

char* program_options::dir_name() {
    return _dir_name;
}

void program_options::help(const char* program_name) {
    std::cout <<
              "Download all files as pdf under the <FOLDER_NAME> folder. <FOLDER_NAME> is matched with regex (grep).\n"
              "To get the contents of the root-folder (My files), use 'root' as <FOLDER_NAME>.\n"
              "\n"
              "Syntax: " << program_name << "[-h] [-r] [-v] <FOLDER_NAME>\n"
                                            "\n"
                                            "options:\n"
                                            "  -v      verbose, display file-id's and wget output\n"
                                            "  -h      help, show this message\n";
    exit(0);
}

void program_options::cleanup() {
    delete[] _dir_name;
}