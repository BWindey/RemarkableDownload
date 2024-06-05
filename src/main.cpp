#include <iostream>
#include <nlohmann/json.hpp>

#include "main.h"


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

    if (!system(nullptr)) {
        std::cerr << "Cannot execute commands\n";
        return 2;
    }

    if (!check_ssh_connection()) {
        std::cerr << "No ssh-connection could be established.\n"
                     "Ensure you are connected via USB and the tablet is on.\n";
        return 3;
    }

    json j = parseMetadata();
    std::unordered_map<std::string, std::vector<rm_file>> file_parent_map;

    for (const auto &entry : j) {
        rm_file file;
        file.UUID           = entry["UUID"];
        file.parent_UUID    = entry["parent"];
        file.visible_name   = entry["visibleName"];
        file.is_folder      = (entry["type"] == "CollectionType");

        file_parent_map[file.parent_UUID].push_back(std::move(file));
    }

    // De-allocate the memory used in program_options
    program_options::cleanup();

    return 0;
}


