#!/bin/bash
# Loads obot3_tmotor_driver onto a tmotor driver board through its can
# bootloader, checks that it runs, and puts the previous firmware back.
#
#   test_can.sh CAN_ID obot3_tmotor_driver.bin [tmotor_driver_noparam.bin]
#
# The board is the node CAN_ID on can0, or on the interface in CAN_IF. It must answer as one of the firmwares
# below: anything else is left alone. With a third argument that firmware is
# loaded at the end, whether the test passed or not; without it the board is
# left in the bootloader.
set -e
if [ $# -lt 2 ]; then
    sed -n '2,10p' "$0"
    exit 2
fi
id=$1
bin=$2
restore=$3
interface=${CAN_IF:-can0}
dev=$interface:$id
load_can=$(dirname "$0")/../../scripts/load_can.py

name() { motor_util -f $dev --list-names-only | tail -n1; }
api() { motor_util -f $dev --no-list --set-api "$1" | tail -n1 | cut -f2 -d' '; }

to_bootloader() {
    local current=$(name)
    echo "node $dev is '$current'"
    case "$current" in
        bootloader)
            ;;
        obot3_tmotor_driver)
            motor_util -f $dev --no-list --set-api go_to_bootloader=b007
            motor_util -f $dev --no-list --set-api reset || true
            sleep 1
            ;;
        tmotor*)
            motor_util -f $dev --no-list --set-api go_to_bootloader=b007
            motor_util -f $dev --no-list set --mode reset || true
            sleep 1
            ;;
        *)
            echo "not a tmotor driver: not touching it"
            exit 1
            ;;
    esac
    [ "$(name)" == bootloader ]
}

# load_can.py gets the node into the bootloader and resets it afterwards
load() {
    "$load_can" --interface $interface --can_id $id --address 0x8002000 --file "$1"
    sleep 1
}

finish() {
    status=$?
    set +e
    if [ -n "$restore" ]; then
        echo "restoring $restore"
        load "$restore"
        echo "node $dev is '$(name)'"
    else
        to_bootloader
    fi
    exit $status
}

check() { # name, value, low limit, high limit
    echo "checking $3 < $1 < $4 with value $2"
    (( $3 < $2 && $2 < $4 ))
}

to_bootloader
trap finish EXIT
load "$bin"
[ "$(name)" == obot3_tmotor_driver ]
echo "version $(api version), serial $(api serial)"

# loop rates, from the counts 2 s apart. The limits allow for the time the
# queries themselves take.
fast1=$(api fast_loop_count)
main1=$(api main_loop_count)
sleep 2
fast2=$(api fast_loop_count)
main2=$(api main_loop_count)
check fast_loop_rate $(( (fast2 - fast1) / 2 )) 45000 65000
check main_loop_rate $(( (main2 - main1) / 2 )) 9000 13000

# readings: 12 bit adc counts. The gate driver is off, so the current sense
# outputs are only checked for being conversions at all; vrefint is 1.21 V.
check IA $(api IA) -1 4096
check IB $(api IB) -1 4096
check IC $(api IC) -1 4096
check vrefint $(api vrefint) 1300 1700
echo "mode $(api mode), menc $(api menc), temperature $(api temperature)"

# the gate driver: 0 errors means every register read back as written
echo "drv_errors $(api drv_errors), drv_fault $(api drv_fault), drv_status $(api drv_status)"

# the host's time: 3 s of motor_util time_sync at 10 Hz. Each pair of frames
# sets the node's clock, none is dropped, and by the end a time moves the
# clock by little: 2 ms allows for an adapter that is slow to say when a frame
# went out.
motor_util --help | grep -q time_sync || { echo "this motor_util has no time_sync: it is older than motor-realtime's develop"; exit 1; }
syncs=$(api time_syncs)
dropped=$(api time_dropped)
timeout -s INT 3 motor_util time_sync --interface $interface --frequency 10 || [ $? -eq 124 ]
check time_syncs $(( $(api time_syncs) - syncs )) 20 40
check time_dropped $(( $(api time_dropped) - dropped )) -1 1
check time_step_ns $(api time_step) -2000000 2000000
echo "time_rate $(api time_rate) ppb"

# still there after the watchdog period several times over
sleep 3
[ "$(name)" == obot3_tmotor_driver ]
echo "obot3_tmotor_driver passed"
