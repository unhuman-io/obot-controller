#!/bin/bash
# Loads obot3_tmotor_driver onto a tmotor driver board through its can
# bootloader, checks that it runs, and puts the previous firmware back.
#
#   test_can.sh CAN_ID obot3_tmotor_driver.bin [tmotor_driver_noparam.bin]
#
# The board is the node CAN_ID on can0. It must answer as one of the firmwares
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
dev=can0:$id
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

load() {
    to_bootloader
    "$load_can" --can_id $id --address 0x8002000 --file "$1"
    motor_util -f $dev --no-list --set-api reset || true
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
check vbus $(api vbus) -1 4096
check ia $(api ia) -1 4096
check ib $(api ib) -1 4096
check ic $(api ic) -1 4096
check vrefint $(api vrefint) 1300 1700
echo "mode $(api mode), menc $(api menc), vbus $(api vbus_mv) mV, temperature $(api temperature)"

# still there after the watchdog period several times over
sleep 3
[ "$(name)" == obot3_tmotor_driver ]
echo "obot3_tmotor_driver passed"
