#include <iostream>
#include <unordered_map>

#include "program_options.h"


int main(int argc, char* argv[]) {
    try {
        program_options::parse(argc, argv);
    } catch (const std::exception &x) {
        std::cerr << x.what() << '\n';
        std::cerr << "Usage: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n";
        return 1;
    }

    if (program_options::verbose()) {
        std::cout << "Main function found verbose\n";
    }

    if (program_options::recursive()) {
        std::cout << "Main function found recursive\n";
    }

    if (program_options::help()) {
        std::cout << "Main function found help\n";
    }

    return 0;
}