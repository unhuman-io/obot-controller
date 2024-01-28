#!/bin/bash -e

# need version 0.6 or above stm32flash
# workarounds for built in nucleo VCP seems to only allow max 255 bytes, E.g.
# 258 bytes will sent just 2 bytes.

# built in erase will try to send more than 256 bytes, -o erases all rather 
# than by page
~/tmp/stm32flash/stm32flash /dev/ttyACM0 -o -b 4000000
sleep 1
# reduce frame size from 256 to 252
~/tmp/stm32flash/stm32flash /dev/ttyACM0 -w boost_uart_noparam.bin -b 4000000 -F 252:252 -e0
~/tmp/stm32flash/stm32flash /dev/ttyACM0 -w boost_uart_param_only.bin -b 4000000 -F 252:252 -e0 -s 0x8060000
~/tmp/stm32flash/stm32flash /dev/ttyACM0 -g 0x8000000 -b 4000000
