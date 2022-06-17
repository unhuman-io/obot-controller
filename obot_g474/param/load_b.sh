# make -j CONFIG=motor_enc_x PARAM=motor_enc_xk
# ./build/motor_enc_x/load_motor_enc_x_param.sh -S 208C366B4752
# make -j CONFIG=motor_enc_x PARAM=motor_enc_xp C_DEFS=-DR0
# ./build/motor_enc_x/load_motor_enc_x_param.sh -S 206033663053
make -j CONFIG=motor_enc_x PARAM=motor_enc_xk PARAM_OVERRIDE=param/rxk.h
#./build/motor_enc_x/load_motor_enc_x_param.sh -S 206033713053
make -j CONFIG=motor_enc_x PARAM=motor_enc_xp PARAM_OVERRIDE=param/rxp.h C_DEFS=-DR0
#./build/motor_enc_x/load_motor_enc_x_param.sh -S 

