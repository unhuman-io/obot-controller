cmake_minimum_required(VERSION 3.28)
project(obot CXX C ASM)

set(CMAKE_CXX_STANDARD 23)
message(VERBOSE "cxx: " ${CMAKE_CXX_COMPILER})

# define optimization options
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3 -DNDEBUG" CACHE STRING "Custom RelWithDebInfo flags" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g3 -DNDEBUG" CACHE STRING "Custom RelWithDebInfo flags" FORCE)

add_compile_options(${MCU_FLAGS})
add_link_options(${MCU_FLAGS})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-fsingle-precision-constant")
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-c99-designator -Wno-c23-extensions)
    add_compile_options(-Xclang -cl-single-precision-constant)
    add_compile_options(-flto=full -fno-builtin-fminf)
    add_link_options(-flto=full)
endif()
add_compile_options("-ffast-math" "-fno-finite-math-only" "-Wdouble-promotion")
add_compile_options("-Werror")
add_compile_options("-fdata-sections" "-ffunction-sections")
add_link_options("-Wl,--gc-sections")

add_subdirectory(motorlib)
add_subdirectory(obot_g474/config)
add_subdirectory(obot3)