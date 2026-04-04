cmake_minimum_required(VERSION 3.28)
project(obot CXX C ASM)

set(CMAKE_CXX_STANDARD 20)
message(VERBOSE "cxx: " ${CMAKE_CXX_COMPILER})

# define optimization options
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

# note release seems to be -O3 by default, relwithdebinfo is -O2
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3 -DNDEBUG")

add_compile_options(${MCU_FLAGS})
add_link_options(${MCU_FLAGS})
add_compile_options("-fsingle-precision-constant")
add_compile_options("-fdata-sections" "-ffunction-sections")
add_link_options("-Wl,--gc-sections")

add_subdirectory(motorlib)
add_subdirectory(obot_g474/config)