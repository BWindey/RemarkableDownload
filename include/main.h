#pragma once

#include "program_options.h"

using json = nlohmann::json;

struct rm_file {
    char UUID[36];
    char parent_UUID[36];
    bool is_folder;
    std::string visible_name;
};

bool check_ssh_connection();
json parseMetadata();
