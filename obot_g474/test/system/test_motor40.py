#!/usr/bin/env python

import motor

m = motor.MotorManager()
m.get_motors_by_name(['motor40'])
print(m)



