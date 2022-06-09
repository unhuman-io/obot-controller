make -j CONFIG=motor_enc_x PARAM=motor_enc_xk C_DEFS=-DR0
./build/motor_enc_x/load_motor_enc_x_param.sh -S 208C366B4752
make -j CONFIG=motor_enc_x PARAM=l
./build/motor_enc_x/load_motor_enc_x_param.sh -S 206033663053
