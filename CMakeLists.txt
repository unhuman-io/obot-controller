cmake_minimum_required(VERSION 3.28)
project(obot CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)
# Set the version of C++ for the project
set(MOTORLIB_DIR "${CMAKE_CURRENT_LIST_DIR}/motorlib")
#list(APPEND CMAKE_MODULE_PATH "${MOTORLIB_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)
message("cxx: " ${CMAKE_CXX_COMPILER})
add_subdirectory(obot_g474/config)
add_subdirectory(motorlib)
