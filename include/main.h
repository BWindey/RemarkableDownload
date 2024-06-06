#pragma once

#include "program_options.h"
#include <iostream>
#include <unordered_set>
#include <cpr/cpr.h>
#include "ssh_stuff.h"

using json = nlohmann::json;

struct rm_file {
    std::string UUID;
    std::string parent_UUID;
    bool is_folder;
    std::string visible_name;

    bool operator==(const rm_file &file) const {
        return UUID == file.UUID;
    }
};

struct rm_file_hash {
    size_t operator()(const rm_file &file) const {
        return std::hash<std::string>()(file.UUID);
    }
};

bool case_insensitive_compare(char a, char b);