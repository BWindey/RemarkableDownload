cmake_minimum_required(VERSION 3.28)
project(RemarkableDownload)

set(CMAKE_CXX_STANDARD 23)

add_executable(RemarkableDownload
        src/main.cpp
        src/program_options.cpp
        src/ssh_stuff.cpp
)

target_compile_features(RemarkableDownload PUBLIC cxx_std_23)

target_include_directories(RemarkableDownload PUBLIC include)
