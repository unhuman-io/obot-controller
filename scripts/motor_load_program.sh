#!/bin/bash
 
 sudo dfu-util -a0 -s 0x8000000:leave -D $@
