cmake_minimum_required(VERSION 3.28)
project(RemarkableDownload)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG a58fb8798eab4b554229b0e4b1eaafb7d24affbe) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_executable(RemarkableDownload
        src/main.cpp
        src/program_options.cpp
        src/ssh_stuff.cpp
        src/download.cpp
)

target_compile_features(RemarkableDownload PUBLIC cxx_std_23)

target_include_directories(RemarkableDownload PUBLIC include)
target_link_libraries(RemarkableDownload PRIVATE cpr::cpr)