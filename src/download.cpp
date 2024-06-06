#include "download.h"


bool create_directory(const std::string &dir_name) {
    /**
     *  Create a directory with the 'mkdir' command.
     *  Warns user when the directory already exists.
     *  @return: whether directory was created succesfully and we can proceed with download
     */
    std::string command = "mkdir '";
    command += dir_name;
    command += "'";

    // Warn user if directory already exists
    if (system(command.c_str()) != 0) {
        std::cout << "\033[1;33mWarning:\033[0m directory '"
                << dir_name
                << "' already exists, contents may be overwritten!\n";
        std::cout << "Do you want to continue? [y/n]: ";
        char input;
        std::cin >> input;
        if (input != 'y' && input != 'Y') {
            std::cout << "Skipping this directory\n";
            return false;
        }
    }
    return true;
}

void download_file(const rm_file &file, const std::string &directory_name) {
    std::cout << "- \033[1m" << file.visible_name << "\033[0m: ";

    if (program_options::verbose()) {
        std::cout << "\033[3m" << file.UUID << "\033[3m\n";
    }

    std::string url = "http://10.11.99.1/download/";
    url += file.UUID;
    url += "/placeholder";

    const cpr::Response response = Get(
        cpr::Url{url}
    );
    if (response.status_code != 200) {
        std::cerr << " \033[31mfailed\033[0m\n";
        return;
    }
    std::cout << " \033[32mdownloaded\033[0m\n";

    std::string output_file_name = directory_name;
    output_file_name += file.visible_name;
    output_file_name += ".pdf";

    std::ofstream out(output_file_name, std::ios::binary);
    out << response.text;
    out.close();
}

void download_directory(const rm_file &directory,
                        const std::unordered_map<std::string, std::vector<rm_file> > &file_relations_map,
                        const std::string &directory_name) {
    std::cout << "\033[1;4mDirectory '" << directory_name << "':\033[0m ";
    if (program_options::verbose()) {
        std::cout << "\033[3m" << directory.UUID << "\033[0m";
    }
    std::cout << std::endl;

    if (!create_directory(directory_name)) {
        return;
    }

    std::vector<rm_file> found_folders;
    // For every file that has the directory of this parent, download it
    for (const rm_file &file: file_relations_map.at(directory.UUID)) {
        if (file.is_folder) {
            if (program_options::recursive()) {
                found_folders.push_back(file);
            }
        } else {
            download_file(file, directory_name);
        }
    }

    // Recursively download the found folders
    for (const rm_file &file: found_folders) {
        std::string recusive_dir_name = directory_name;
        recusive_dir_name += file.visible_name;
        recusive_dir_name += "_rm/";

        download_directory(file, file_relations_map, recusive_dir_name);
    }
}
