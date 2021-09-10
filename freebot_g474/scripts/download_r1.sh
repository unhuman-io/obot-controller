#!/bin/bash

dir=$(dirname $0)
$dir/motor_wheel/load_motor_wheel.sh -p 1-2.1
#$dir/motor_wheel/load_motor_wheel_r.sh -p 1-2.3
$dir/motor_enc_j2/load_motor_enc_j2.sh -p 1-2.2
$dir/motor_enc_j3/load_motor_enc_j3.sh -p 1-2.4
