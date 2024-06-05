#pragma once

#include "program_options.h"
#include "ssh_stuff.h"

using json = nlohmann::json;

struct rm_file {
    std::string UUID;
    std::string parent_UUID;
    bool is_folder;
    std::string visible_name;
};

