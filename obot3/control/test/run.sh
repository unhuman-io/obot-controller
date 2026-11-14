#!/bin/bash -e
# Builds the control modules and their test for the host and runs it.
#   run.sh [compiler]     g++ 14 or newer, default g++
cxx=${1:-g++}
obot3=$(cd "$(dirname "$0")/../.." && pwd)
build=$(mktemp -d)
trap 'rm -rf "$build"' EXIT
cd "$build"
flags="-std=c++23 -O2 -Wall -fmodules-ts -I $obot3"
for module in latest control_fun sincos foc motor_controller; do
    $cxx $flags -x c++ -c "$obot3/control/$module.cppm" -o $module.o
done
$cxx $flags "$obot3/control/test/test_control.cpp" latest.o control_fun.o sincos.o foc.o motor_controller.o -o test_control
./test_control
