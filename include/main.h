#pragma once

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

bool contains(const std::string &s1, const std::string &s2);
bool case_insensitive_compare(char a, char b);