#!/bin/bash -e

can_ids=(1 2 3 4 5 6 7)

cd $(dirname $0)

make -j -C ../obot_g474 CONFIG=tmotor_driver all

for can_id in "${can_ids[@]}"; do
    motor_util -f can0:$can_id --set-api go_to_bootloader=b007
    motor_util -f can0:$can_id set --mode reset
    
    sleep 1
    ./load_can.py --can_id $can_id --address 0x8002000 --file ../obot_g474/build/tmotor_driver/tmotor_driver_noparam.bin
    make -C ../obot_g474 CONFIG=tmotor_driver C_DEFS=-DCAN_ID=$can_id build_param
    ./load_can.py --can_id $can_id --address 0x8060000 --file ../obot_g474/build/param/param_obot_g474_tmotor_driver.bin
    motor_util -f can0:$can_id --set-api reset
done

