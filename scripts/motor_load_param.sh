#!/bin/bash
 
 sudo dfu-util -a0 -s 0x8060000:leave -D $@
