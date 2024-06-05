#include "main.h"
#include <iostream>


int main(int argc, char* argv[]) {
    // Parse command-line arguments
    try {
        program_options::parse(argc, argv);
    } catch (const std::exception &x) {
        std::cerr << x.what() << '\n';
        std::cerr << "Usage: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n";
        return 1;
    }

    // Check if there was a directory-name given
    if (program_options::dir_name() == nullptr) {
        std::cerr << "Usage: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n";
        return 1;
    }

    // Check if we can execute shell-commands (ssh later)
    if (!system(nullptr)) {
        std::cerr << "Cannot execute commands\n";
        return 2;
    }

    // Check if ssh-connection can be made
    if (!check_ssh_connection()) {
        std::cerr << "No ssh-connection could be established.\n"
                     "Ensure you are connected via USB and the tablet is on.\n";
        return 3;
    }

    // Load the json-data in
    json json_data = parseMetadata();
    std::unordered_map<std::string, std::vector<rm_file>> file_relations_map;

    for (const auto &entry : json_data) {
        rm_file file;
        file.UUID           = entry["UUID"];
        file.parent_UUID    = entry["parent"];
        file.visible_name   = entry["visibleName"];
        file.is_folder      = (entry["type"] == "CollectionType");

        file_relations_map[file.parent_UUID].push_back(std::move(file));
    }

    // Get the needed UUIDs


    // De-allocate the memory used in program_options
    program_options::cleanup();

    return 0;
}


