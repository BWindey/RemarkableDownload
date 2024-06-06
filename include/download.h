#pragma once
#include <main.h>

void download_directory(const rm_file &directory,
    const std::unordered_map<std::string,
    std::vector<rm_file>> &file_relations_map,
    const std::string &directory_name,
    const std::string &cout_prefix);
