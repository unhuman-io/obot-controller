#!/usr/bin/env python
from subprocess import *
from io import BytesIO
import numpy as np
from numpy import *
from time import sleep
from circular_filt import *
from matplotlib.pyplot import *

def run_timeout(*args, **kwargs):
    try: 
        kwargs["capture_output"] = True
        run(*args, **kwargs)
    except TimeoutExpired as e:
        return e.stdout

angles = np.linspace(0,2*pi,3)
# get mgt motor and joint
for angle in angles:
    run(["motor_util", "set", "--mode", "position", "--position", str(angle)], capture_output=True)
    sleep(.1)
    run(["motor_util", "set", "--mode", "open"], capture_output=True)
    a = run(["sudo", "motor_util", "--no-list", "--set_api", "mmgt"], capture_output=True)
    sleep(.1)
    run(["sudo", "motor_util", "--no-list", "--set_api", "jmgt"], capture_output=True)
    sleep(.1)

    print(a.stdout)

#mgt = run_timeout(["sudo", "motor_util", "--no-list", "read"], timeout=.5)
#print(mgt)

# set voltage commutation 

run(["motor_util", "set", "--mode", "position_tuning", "--position", "157", "--reserved", ".5"], capture_output=True)
sleep(.5)

# collect data
data = run_timeout(["motor_util", "--no-list", "read", "--frequency", "5000"], timeout=2)
run(["motor_util", "set", "--mode", "open"], capture_output=True)

d = np.genfromtxt(BytesIO(data), names=True, delimiter=",")
print(np.average(d["joint_position0"]))

# process
# find phase shift and amplitude at position frequency
data = d["motor_position0"]
dataselinds = logical_and(fabs(data)<.8*max(data), diff(data)>0)
datasel = data[dataselinds]
k = 1
N = size(data)
motor_phase_freq = sum(data*exp(-k*2*pi*1j*arange(N)/N))
motor_ideal = 1/N*motor_phase_freq*exp(2*pi*1*1j*k*arange(N)/N)
motor_ideal = real(motor_ideal[dataselinds])*2
plot(motor_ideal)
plot(datasel)
show()
plot(mod(motor_ideal,2*pi), datasel-motor_ideal)
show()
calib = circular_filt(motor_ideal,datasel-motor_ideal,nbins=100)
plot(calib)
show()

# upload params

# set triangle mode

# collect data

# process

