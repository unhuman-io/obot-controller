#!/usr/bin/env python

import motor
import sys
import re
import time

def check_var(item, val):
    result = mot[item].get()
    if result != val:
        print("ERROR: " + item + " is not " + val + " (" + result + ")")
        sys.exit(1)
    else:
        print(item + " is " + val)

def check_var_regex(item, val):
    result = mot[item].get()
    if not re.match(val, result):
        print("ERROR: " + item + " is not " + val + " (" + result + ")")
        sys.exit(1)
    else:
        print(item + " is " + result)

def set_and_check_var(item, val):
    result = mot[item].set(val)
    print(result)
    assert(result == item + " set " + val)
    check_var(item, val)

def check_var_is_float(item):
    try:
        val = float(mot[item].get())
    except ValueError:
        print("ERROR: " + item + " is not a float")
        sys.exit(1)
    print(item + " is a float (" + str(val) + ")")

def set_and_check_near(item, val, tol):
    result = mot[item].set(str(val))
    print(result)
    assert(result == item + " set " + str(val))
    check_near(item, val, tol)

def check_near(item, val, tol):
    try:
        assert(abs(float(mot[item].get()) - val) < tol)
    except ValueError:
        print("ERROR: " + item + " is not a float")
        sys.exit(1)
    print(item + " is near " + str(val) + " (" + str(float(mot[item].get())) + ")")

def check_var_is_int(item):
    try:
        val = int(mot[item].get())
    except ValueError:
        print("ERROR: " + item + " is not an int")
        sys.exit(1)
    print(item + " is an int (" + str(val) + ")")

def check_var_is_hex(item):
    try:
        val = int(mot[item].get(), 16)
    except ValueError:
        print("ERROR: " + item + " is not a hex")
        sys.exit(1)
    print(item + " is a hex (" + str(val) + ")")

m = motor.MotorManager()
m.get_motors_by_name(['motor9'])
print(m)

mot = m.motors()[0]
mot.set_timeout_ms(10) # default timeout

set_and_check_var("mled_cur", "0")
check_var_is_float("mai_phase")
check_var_is_float("mai_scale")
check_var_is_float("mcos_off")
check_var_is_float("msc_gain")
check_var_is_float("msc_phase")
check_var_is_float("mai_phases")
check_var_is_float("mai_scales")
check_var_is_float("mcos_offs")
check_var_is_float("msc_gains")
check_var_is_float("msc_phases")

check_var_is_int("merr")
check_var_is_int("mwarn")
check_var("mcrc_cnt", "0")

check_var_is_int("mraw")
check_var_is_hex("mrawh")
check_var_is_int("mvalue")

mdiag  = mot["mdiag"].get()
print(mdiag)
assert(re.match(r"^\d{16}$", mdiag))


#"auto_ana"
set_and_check_var("mecc_correction", "1")
set_and_check_var("mecc_correction", "0")
set_and_check_var("mran_tol", "4")
set_and_check_var("mran_tol", "5")
set_and_check_near("mecc_um", 0, 0.001)
set_and_check_near("mecc_um", 1, 0.001)
set_and_check_var("mlow", "1")
set_and_check_var("mlow", "0")
set_and_check_var("mac_count", "1")
set_and_check_var("mac_count", "8")

fm = r"[-+]?[0-9]*\.?[0-9]+"
match_cals = f"^cos_off: {fm}, sin_off: {fm}, sc_gain: {fm}, sc_phase: {fm}, ai_phase: {fm}, ai_scale: {fm}"
mcals = mot["mcals"].get()
print(mcals)
match = re.match(match_cals, mcals)
assert(match)

match_cal = match_cals + f", ecc_amp: {fm}, ecc_phase: {fm}$"
mcal = mot["mcal"].get()
print(mcal)
match = re.match(match_cal, mcal)
assert(match)

check_near("mdisk_um", 10700, 1)
set_and_check_var("mipo_filt1", "ea")
check_var("mipo_filt2", "04")
check_near("mtemp", 30, 20)


set_and_check_var("mipo_filt1", "6e")
set_and_check_var("mac_count", "0")
check_var("mauto_ana", "ok")
time.sleep(2)
check_var("mcmd_result", "command: 255, result: 128")
check_var("mauto_dig", "ok")
time.sleep(.1)
check_var("mcmd_result", "command: 0, result: 0")
check_var("mreadj_dig", "ok")
time.sleep(2)
check_var("mcmd_result", "command: 0, result: 0")
check_var("mauto_ecc", "ok")
time.sleep(.2)
check_var_regex("mcmd_result", r"command: \d{1,3}, result: \d{1,3}")
mot.set_timeout_ms(25)
check_var("mconf_write", "conf write success")

mot.set_timeout_ms(10)
check_var("mconf_write_no_check", "ok")
time.sleep(.1)
check_var("mcmd_result", "command: 0, result: 0")

set_and_check_var("mled_cur", "1")

check_var_is_int("menc")

