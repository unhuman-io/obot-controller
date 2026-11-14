#!/bin/bash
# gdb on a running obot3 application, through its text api: the node keeps
# running while gdb reads and writes its memory, so with the elf file every
# static variable is there by name.
#
#   obot3_gdb.sh CAN_ID [ELF] [gdb arguments, e.g. -batch -ex 'p fast_loop.sensors']
#
#   (gdb) p fast_loop.controller.pi_iq_.param_
#   (gdb) set var fast_loop.controller.pi_iq_.param_.kp = 3
#   (gdb) p main_loop.controller.position_controller
#   (gdb) x/4xw 0x40021000
#
# `motor_util gdbserver` is the bridge, on port 3293. The registers read 0 and
# continue, step and breakpoints do nothing: there is no stopped context.
set -e
if [ $# -lt 1 ]; then
    sed -n '2,14p' "$0"
    exit 2
fi
id=$1
root=$(cd "$(dirname "$0")/.." && pwd)
elf=${2:-$root/build/gcc/obot3/obot3_tmotor_driver.elf}
shift
[ $# -gt 0 ] && shift
gdb=$root/motorlib/gcc/bin/arm-none-eabi-gdb
[ -x "$gdb" ] || gdb=arm-none-eabi-gdb

motor_util -f can0:$id --no-list gdbserver > /dev/null 2>&1 &
server=$!
trap 'kill $server 2>/dev/null' EXIT
sleep 0.5
"$gdb" -q -ex "set confirm off" -ex "target remote :3293" "$@" "$elf"
