#pragma once

#include "json.hpp"

using json = nlohmann::json;

bool check_ssh_connection();
json parseMetadata();