#!/bin/bash
# gdb on an obot3 application over can, without a debug probe: through its
# text api and the debug monitor (obot3/debug_monitor.h).
#
#   obot3_gdb.sh [-n] CAN_ID [ELF] [gdb arguments, e.g. -batch -ex 'p fast_loop.sensors']
#
# The node keeps running when gdb connects: memory is live, so with the elf
# file every static variable is there by name, and the registers are a sample.
#   (gdb) p fast_loop.controller.pi_iq_.param_
#   (gdb) set var fast_loop.controller.pi_iq_.param_.kp = 3
#   (gdb) x/4xw 0x40021000
#   (gdb) monitor ikp                   a text api request
#
# continue, ctrl-c, step, next, break, watch and info threads work on the main
# loop (thread 2) and the background (thread 1). The fast loop and the text
# api run on while those are stopped; a breakpoint in them is dropped.
#   (gdb) break main_loop_update
#   (gdb) continue
#   (gdb) next
#   (gdb) info threads
# Which thread runs is gdb's to say. As it is, everything stops at a stop, and
# the other thread runs during a step:
#   (gdb) set scheduler-locking step    the other thread stays stopped during a step
# Thread 2 while the main loop is not in its handler is shown where thread 1
# is; stepi from there enters the handler.
# With -n gdb is in non-stop mode: the threads stop and run one by one, and
# the main loop keeps running while only the background is stopped.
#   (gdb) interrupt                     stops thread 1, the background
#   (gdb) interrupt -a                  both
#   (gdb) thread 2                      then continue, stepi, ... are for the main loop
#   (gdb) continue -a
#   (gdb) monitor live                  the node continues and gdb keeps its prompt, as at the start
#
# Thread 3 runs functions for gdb, from the scheduler loop (os_loop) between
# two of its tasks, with nothing else stopped for it:
#   (gdb) thread apply 3 print tmotor_drv8323_read(5)
#   (gdb) thread apply 3 call tmotor_led(0x4000, 0, 0)
#   (gdb) thread 3                      then print f() and call f() are for it
# A call from any other thread is refused ("Could not write register"). The
# function has to be in the elf: DEBUG_CALL in front of it keeps it there
# (obot3/debug_call.h). Integers, floats and pointers go in and come back;
# a string literal as an argument does not, gdb wants malloc for it. Called
# while the node is stopped, everything continues for the call and stops again
# after it. The scheduler loop waits for the function: one that takes longer
# than the watchdog allows resets the node.
# Leaving gdb removes the breakpoints and lets the node continue.
#
# The bridge is `motor_util gdbserver` on port 3293, from a motor-realtime with
# the thread support (gdbserver --verbose exists): $MOTOR_UTIL, else the build
# in ../motor-realtime, else the installed one. An earlier one is refused: it
# shows one thread and leaves the node stopped, which then resets after 60 s.
# A debug probe has to be detached (text api: debug_probe is 0), or
# breakpoints and steps halt the cpu for the probe instead.
set -e
non_stop=()
if [ "$1" == -n ]; then
    non_stop=(-ex "set non-stop on")
    shift
fi
if [ $# -lt 1 ]; then
    sed -n '2,52p' "$0"
    exit 2
fi
id=$1
root=$(cd "$(dirname "$0")/.." && pwd)
elf=${2:-$root/build/gcc/obot3/obot3_tmotor_driver.elf}
shift
[ $# -gt 0 ] && shift
gdb=$root/motorlib/gcc/bin/arm-none-eabi-gdb
[ -x "$gdb" ] || gdb=arm-none-eabi-gdb

motor_util=
for candidate in "$MOTOR_UTIL" "$root/../motor-realtime/build/src/motor/motor_util" motor_util; do
    if [ -n "$candidate" ] && "$candidate" gdbserver --help 2>/dev/null | grep -q -- --verbose; then
        motor_util=$candidate
        break
    fi
done
if [ -z "$motor_util" ]; then
    echo "no motor_util with the gdbserver for threads: build motor-realtime's feature/gdbserver_threads and set MOTOR_UTIL" >&2
    exit 1
fi

# A node that doesn't take gdb's packets, its bootloader for one, is no target
name=$("$motor_util" -f can0:$id --no-list --set-api name 2>/dev/null | tail -n1 | tr -d '\0')
stop=$("$motor_util" -f can0:$id --no-list --set-api '$?' 2>/dev/null | tail -n1 | tr -d '\0')
if [[ "$stop" != T* ]]; then
    echo "node can0:$id is '$name', which does not answer gdb's packets: load the application first (scripts/load_can.py)" >&2
    exit 1
fi

if ss -ltn 2>/dev/null | grep -q ':3293 '; then
    echo "port 3293 is in use: another gdbserver is running, $(ss -ltnp 2>/dev/null | grep -o 'pid=[0-9]*' | head -n1)" >&2
    exit 1
fi
# In a session of its own: ctrl-c at the terminal is for gdb, not for the server
setsid "$motor_util" -f can0:$id --no-list gdbserver > /dev/null 2>&1 &
server=$!
trap 'kill -INT $server 2>/dev/null' EXIT
trap 'exit 1' HUP TERM      # with the exit trap: no server left behind
sleep 0.5
"$gdb" -q -ex "set confirm off" "${non_stop[@]}" -ex "target remote :3293" "$@" "$elf"
