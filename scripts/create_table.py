#!/usr/bin/env python

# requirements scipy matplotlib numpy

import numpy as np
from numpy import *
from scipy.fftpack import *
from matplotlib.pyplot import *

from circular_filt import *
import argparse
import os

# from https://stackoverflow.com/questions/27138751/preventing-plot-joining-when-values-wrap-in-matplotlib-plots
def unlink_wrap(dat, lims=[-np.pi, np.pi], thresh = 0.5):
    """
    Iterate over contiguous regions of `dat` (i.e. where it does not
    jump from near one limit to the other).

    This function returns an iterator object that yields slice
    objects, which index the contiguous portions of `dat`.

    This function implicitly assumes that all points in `dat` fall
    within `lims`.

    """
    jump = np.nonzero(np.abs(np.diff(dat)) > ((lims[1] - lims[0]) * thresh))[0]
    lasti = 0
    for ind in jump:
        yield slice(lasti, ind + 1)
        lasti = ind + 1
    yield slice(lasti, len(dat))


class Table:
    def __init__(self, args_in=None):
        parser = argparse.ArgumentParser(description='Process motor data for a table')
        parser.add_argument('infile', nargs='?', type=argparse.FileType('r'), default=sys.stdin)
        parser.add_argument('outfile', nargs='?', type=argparse.FileType('w'), default=sys.stdout)
        parser.add_argument('-s', '--table-size', dest="table_size", type=int, default=512)
        parser.add_argument('-i', '--index-pos',dest="index_pos", default=0)
        parser.add_argument('-c', '--cpr',dest="cpr",default=8192)
        parser.add_argument('-n', '--gear-ratio',dest="gear_ratio",type=float,default=1.)
        parser.add_argument('-e', '--encoder',dest="encoder_table", action="store_true", default=False)
        parser.add_argument('-j', '--joint',dest="joint_table", action="store_true", default=False)
        parser.add_argument('--estimate-velocity', dest="estimate_velocity", action="store_true", default=False)
        parser.add_argument('-f', '--filter',dest="filter", type=float,default=None)
        self.args = parser.parse_args(args_in)

        self.data = genfromtxt(self.args.infile, delimiter=',', names=True)
        if (self.args.encoder_table):
            self.table = self.parse_encoder_table(nbins=self.args.table_size,index_pos=self.args.index_pos,cpr=self.args.cpr, estimate_velocity=self.args.estimate_velocity, filter=self.args.filter)
        elif (self.args.joint_table):
            self.table = self.parse_joint_table(nbins=self.args.table_size,index_pos=self.args.index_pos,n=self.args.gear_ratio)
        else:
            self.table = self.parse_cogging_table(nbins=self.args.table_size,index_pos=self.args.index_pos,cpr=self.args.cpr)
        self.save(self.args.outfile)

    def velocity_average(self, t, data):
        velocity = np.diff(data)/np.diff(t)
        # simple
        velocity_avg = mean(velocity)
        pos_avg = t*velocity_avg
        # N = 10000
        # velocity_avg = np.convolve(velocity, np.ones(N)/N, mode='same')
        # #velocity_avg = np.insert(velocity_avg,0,velocity_avg[0])
        # pos_avg = np.diff(t)*velocity_avg
        # pos_avg = np.insert(pos_avg,0,0)

        # figure()
        # plot(velocity_avg)
        # show()
        return pos_avg

    def parse_encoder_table(self, nbins=32, index_pos=0, cpr=8192, estimate_velocity=False, filter=2):
        if not filter:
            filter = 2
        e = self.data["motor_encoder0"]-float(index_pos)
        vpos = None
        if estimate_velocity:
            vpos = self.velocity_average(self.data["t_seconds0"], e*2*pi/float(cpr))
        else:
            vpos = self.data["motor_position0"]
        vpos = unwrap(vpos, 2*pi)
        y = vpos - e*2*pi/float(cpr)
        x = e*2*pi/float(cpr)     
        # i = where(abs(vpos) > .9*max(vpos))
        # x = delete(x, i) 
        # y = delete(y, i)

        return self.create_table(x, -y, nbins, filter)

    def parse_cogging_table(self, nbins=32, index_pos=0, cpr=8192):
        e = self.data["motor_encoder0"]-float(index_pos)
        iq = self.data["iq0"] 
        y = iq
        x = e*2*pi/float(cpr)
        xavg = x-mean(x)     
        i = where(abs(xavg) > .9*max(xavg))
        x = delete(x, i) 
        y = delete(y, i)

        return self.create_table(x, y, nbins, 100)

    def parse_joint_table(self, nbins=32, index_pos=0, n=1.):
        e = self.data["joint_position0"]
        m = self.data["motor_position0"] 
        y = m/n - e
        x = e    
        i = where(abs(e) > .9*max(e))
        x = delete(x, i) 
        y = delete(y, i)
        return self.create_table(x, y, nbins, 2)


    def create_table(self, x, y, nbins, ffilt):
        yfilt = circular_filt(x, y-mean(y), nbins=nbins, ffilt=ffilt)
        xfilt = linspace(0, 2*pi, nbins+1)[:-1]

        yfiltd = fft_derivative(xfilt,yfilt)

        pchip = pchip_coeff(xfilt, yfilt, yfiltd)
        xcalc = linspace(0,2*pi,1000)
        ycalc = pchip_calc(pchip, xcalc)

        # figure()
        # plot(x, y)
        # xlabel('raw x')
        # ylabel('raw y')

        figure()
        xmod = mod(x,2*pi)
        ymod = y-mean(y)
        for slc in unlink_wrap(xmod, [0, 2*pi]):
            plot(xmod[slc], ymod[slc], 'b-')
        plot(xfilt,yfilt)
        plot(xfilt, pchip[:,0], '*')
        plot(xcalc, ycalc,'.')
        
        xlabel('xfilt % 2*pi')
        ylabel('yfilt')
        title('table value y vs input x')
        #legend(['raw','filtered','cubic points','cubic spline'])
        grid()
        show()
        return pchip

    def save(self, outfile):
        outfile.write("{")
        savetxt(outfile, self.table, delimiter=',', newline="},\n{")
        # remove extra {
        outfile.seek(0, os.SEEK_END)
        size = outfile.tell()
        outfile.truncate(size-1)

if __name__ == "__main__":
    t = Table()
