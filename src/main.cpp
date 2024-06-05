#include <iostream>

#include "program_options.h"


int main(int argc, char* argv[]) {
    try {
        program_options::parse(argc, argv);
    } catch (const std::exception &x) {
        std::cerr << x.what() << '\n';
        std::cerr << "Usage: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n";
        return 1;
    }

    if (program_options::help()) {
        std::cout <<
            "Download all files as pdf under the <FOLDER_NAME> folder. <FOLDER_NAME> is matched with regex (grep).\n"
            "To get the contents of the root-folder (My files), use 'root' as <FOLDER_NAME>.\n"
            "\n"
            "Syntax: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n"
            "\n"
            "options:\n"
            "  -v      verbose, display file-id's and wget output\n"
            "  -h      help, show this message\n";
    }

    // De-allocate the memory used in program_options
    program_options::cleanup();

    return 0;
}