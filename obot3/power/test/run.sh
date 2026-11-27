#!/bin/bash -e
# Builds the power board's hardware-independent modules and their test for the
# host and runs it.
#   run.sh [compiler] [directory for pictures of the screens]     g++ 14 or newer, default g++
cxx=${1:-g++}
pictures=${2:-}
power=$(cd "$(dirname "$0")/.." && pwd)
build=$(mktemp -d)
trap 'rm -rf "$build"' EXIT
cd "$build"
flags="-std=c++23 -O2 -Wall -Wextra -Werror -fmodules-ts"
modules="frame font aes messages logic screen ina228"
objects=""
for module in $modules; do
    $cxx $flags -x c++ -c "$power/common/$module.cppm" -o $module.o
    objects="$objects $module.o"
done
$cxx $flags "$power/test/test_power.cpp" $objects -o test_power
./test_power $pictures
