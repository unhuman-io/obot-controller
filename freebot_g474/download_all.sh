#!/bin/bash

touch tmp.bin
#assume 100k is enough, will make upload faster
dd bs=100k count=1 if=build/freebot_g474.bin of=tmp.bin
motor_util set --mode phase_lock --current -1

sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 206B3372484E
sudo dfu-util -a0 -s 0x8060000:leave -D build/param/col2.bin -S 206B3372484E

rm tmp.bin