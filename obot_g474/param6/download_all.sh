#!/bin/bash

touch tmp.bin
#assume 100k is enough, will make upload faster
dd bs=100k count=1 if=../build/obot_g474.bin of=tmp.bin
motor_util set --mode open
motor_util set --mode phase_lock --current -1
sleep 1

sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 206A3376484E
sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 206C339D484E
sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 206B3392484E
sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 206A3375484E
sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 206C3394484E
sudo dfu-util -a0 -s 0x8000000 -D tmp.bin -S 2069339F484E

sudo dfu-util -a0 -s 0x8060000:leave -D ../build/param/A1.bin -S 206A3376484E
sudo dfu-util -a0 -s 0x8060000:leave -D ../build/param/A2.bin -S 206C339D484E
sudo dfu-util -a0 -s 0x8060000:leave -D ../build/param/A3.bin -S 206B3392484E
sudo dfu-util -a0 -s 0x8060000:leave -D ../build/param/A4.bin -S 206A3375484E
sudo dfu-util -a0 -s 0x8060000:leave -D ../build/param/A5.bin -S 206C3394484E
sudo dfu-util -a0 -s 0x8060000:leave -D ../build/param/A6.bin -S 2069339F484E

rm tmp.bin