#!/usr/bin/env python
from numpy import *
from scipy.fftpack import *
from matplotlib.pyplot import *

def circular_filt(x, y, nbins=1000, modx=2*pi, ffilt=.2, magfilt=.1):
    xm = mod(x,modx)
    xi = linspace(0,modx, nbins+1)
    ibins = digitize(xm, xi)
    bc = bincount(ibins)
    bs = bincount(ibins,weights=y)
    yavg = bs/bc
    yavg = yavg[1:]
    plot(yavg)
    show()
    #yavgz = yavg - mean(yavg)

    f = fft(yavg)
    freq = fftfreq(nbins)
    f_filt = f
    f_filt[fabs(freq)>ffilt] = 0
    f_filt[absolute(f_filt)<magfilt] = 0
    #print(f_filt)
    f_filt[0] = 0 # remove average
    return ifft(f_filt)
