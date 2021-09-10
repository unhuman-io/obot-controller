#!/bin/bash

build_dir=build_r1

script_dir=$(cd $(dirname $0) && pwd)
pushd $script_dir/..
make CONFIG=motor_wheel -j
make CONFIG=motor_wheel PARAM_OVERRIDE=param/wr.h -C_DEFS=-DR0 -j
make CONFIG=motor_enc PARAM=motor_enc_large PARAM_OVERRIDE=param/j2.h -j
make CONFIG=motor_enc PARAM=motor_enc_small PARAM_OVERRIDE=param/j3.h -j
popd

mkdir -p $build_dir
cp -r $script_dir/../build/motor_wheel $build_dir
cp -r $script_dir/../build/motor_wheel_wr $build_dir
cp -r $script_dir/../build/motor_enc_j2 $build_dir
cp -r $script_dir/../build/motor_enc_j3 $build_dir
cp $script_dir/download_r1.sh $build_dir