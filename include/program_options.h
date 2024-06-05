#pragma once

namespace program_options {
    void parse(int argc, char* argv[]);

    bool verbose();
    bool recursive();
    bool help();

    char* dir_name();

    void cleanup();
};