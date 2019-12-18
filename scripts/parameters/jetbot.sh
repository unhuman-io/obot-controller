#!/bin/bash

names=(J1 J2)
for name in ${names[@]}; do
    echo $name
    path=$(motor_util -n $name --list-path-only)
    make PARAM_GEN="-n $name" DFU="-p $path"
done