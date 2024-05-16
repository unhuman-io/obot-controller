#!/usr/bin/env python

import motor
import sys

def check_var(item, val):
    if mot[item].get() != val:
        print("ERROR: " + item + " is not " + val)
        sys.exit(1)
    else:
        print(item + " is " + val)

m = motor.MotorManager()
m.get_motors_by_name(['motor9'])
print(m)

mot = m.motors()[0]
mot["mled_cur"] = "1"
check_var("mled_cur", "1")


