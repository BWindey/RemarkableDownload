#include "main.h"
#include <iostream>
#include <unordered_set>
#include <cpr/cpr.h>


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
    for (const rm_file &directory: found_directories) {
        // Make directory for the files
        std::string directory_name = directory.visible_name;
        directory_name += "_rm/";

        std::cout << "\033[1;4mDirectory '" << directory.visible_name << "':\033[0m ";
        if (program_options::verbose()) {
            std::cout << "\033[3m" << directory.UUID << "\033[0m";
        }
        std::cout << std::endl;

        std::string command = "mkdir '";
        command += directory_name;
        command += "'";

        // Warn user if directory already exists
        if (system(command.c_str()) != 0) {
            std::cout << "\033[1;33mWarning:\033[0m directory '"
                    << directory_name
                    << "' already exists, contents may be overwritten!\n";
            std::cout << "Do you want to continue? [y/n]: ";
            char input;
            std::cin >> input;
            if (input != 'y' && input != 'Y') {
                std::cout << "Skipping this directory\n";
                continue;
            }
        }

        // For every file that has the directory of this parent, download it
        for (const rm_file &file: file_relations_map[directory.UUID]) {
            if (file.is_folder) {
                // for now
                continue;
            }

            std::cout << "- \033[1m" << file.visible_name << "\033[0m: ";

            if (program_options::verbose()) {
                std::cout << "\033[3m" << file.UUID << "\033[3m\n";
            }

            std::string url = "http://10.11.99.1/download/";
            url += file.UUID;
            url += "/placeholder";

            cpr::Response response = Get(
                cpr::Url{url}
            );
            if (response.status_code != 200) {
                std::cerr << " \033[31mfailed\033[0m\n";
                continue;
            }
            std::cout << " \033[32mdownloaded\033[0m\n";

            std::string output_file_name = directory_name;
            output_file_name += file.visible_name;
            output_file_name += ".pdf";

            std::ofstream out(output_file_name, std::ios::binary);
            out << response.text;
            out.close();
        }
    }

    // De-allocate the memory used in program_options
    program_options::cleanup();

    return 0;
}


bool case_insensitive_compare(const char a, const char b) {
    return std::tolower(a) == std::tolower(b);
}
