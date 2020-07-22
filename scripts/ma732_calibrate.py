#!/usr/bin/env python
from subprocess import *
from io import BytesIO
import numpy as np

def run_timeout(*args, **kwargs):
    try: 
        kwargs["capture_output"] = True
        run(*args, **kwargs)
    except TimeoutExpired as e:
        return e.stdout

# get mgt motor and joint
run(["sudo", "motor_util", "--no-list", "--set_api", "mbct"], capture_output=False)
#mgt = run_timeout(["sudo", "motor_util", "--no-list", "read"], timeout=.5)
#print(mgt)

# set voltage commutation 

# collect data
data = run_timeout(["motor_util", "--no-list", "read"], timeout=.01)

d = np.genfromtxt(BytesIO(data), names=True, delimiter=",")
print(np.average(d["joint_position0"]))

# process

# upload params

# set triangle mode

# collect data

# process

