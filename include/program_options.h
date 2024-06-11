#pragma once

namespace program_options {
    void parse(int argc, char* argv[]);

    bool file();
    bool verbose();
    bool recursive();
    bool no_confirm_dir();
    char* dir_name();

    void help(const char* program_name);
    void cleanup();
}
