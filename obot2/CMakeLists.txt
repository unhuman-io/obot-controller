cmake_minimum_required(VERSION 3.28)
project(obot2 CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)
# Set the version of C++ for the project
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)
message("cxx: " ${CMAKE_CXX_COMPILER})
add_subdirectory(src)
