#include "main.h"

#include "download.h"


int main(int argc, char *argv[]) {
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
        std::cerr << "Cannot execute commands\n"
                     "Exiting\n";
        return 2;
    }

    // Check if ssh-connection can be made
    if (!check_ssh_connection()) {
        std::cerr << "No ssh-connection could be established.\n"
                "Ensure you are connected via USB and the tablet is on.\n"
                "Exiting\n";
        return 3;
    }

    // Load the json-data in
    json json_data = parseMetadata();
    std::unordered_map<std::string, std::vector<rm_file> > file_relations_map;
    std::unordered_set<rm_file, rm_file_hash> all_files_set;

    for (const auto &entry: json_data) {
        rm_file file;
        file.UUID = entry["UUID"];
        file.parent_UUID = entry["parent"];
        file.visible_name = entry["visibleName"];
        file.is_folder = entry["type"] == "CollectionType";

        file_relations_map[file.parent_UUID].push_back(file);
        all_files_set.insert(std::move(file));
    }

    // Get the needed UUIDs
    std::string name_to_search = program_options::dir_name();
    std::vector<rm_file> found_directories;

    for (const rm_file &file: all_files_set) {
        if (!file.is_folder) {
            continue;
        }
        if (!std::ranges::search(file.visible_name, name_to_search, case_insensitive_compare).empty()) {
            found_directories.push_back(file);
        }
    }

    // For each file in found_files, download it through http-request
    /*
     * TODO:
     *  - add extra new-line between directories
     *  - add confirmation when multiple directories
     *  - recursive
     */
    for (int i = 0; i < found_directories.size(); i++) {
        download_directory(found_directories.at(i), file_relations_map);
        if (i + 1 < found_directories.size()) {
            std::cout << std::endl;
        }
    }

    // De-allocate the memory used in program_options
    program_options::cleanup();

    return 0;
}


bool case_insensitive_compare(const char a, const char b) {
    return std::tolower(a) == std::tolower(b);
}
