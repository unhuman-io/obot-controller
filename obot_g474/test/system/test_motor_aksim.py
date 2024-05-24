#!/usr/bin/env python3

import motor
import time
import unittest
import sys
import numpy as np

path = None
class TestMotor(unittest.TestCase):
    m = None
    motor = None

    @classmethod
    def setUpClass(cls):
        cls.m = motor.MotorManager()
        if (path):
            cls.m.get_motors_by_path([path])
        cls.motor = cls.m.motors()[0]

    @classmethod
    def tearDownClass(cls):
        cls.m.set_command_mode(motor.ModeDesired.Open)
        cls.m.write_saved_commands()

    def tearDown(self):
        self.m.set_commands([motor.Command()])
        self.m.set_command_mode(motor.ModeDesired.Open)
        self.m.write_saved_commands()
    
    def test_imu(self):
        self.motor["imu_read"].get()
        time.sleep(0.01)
        val = self.motor["ax"].get()
        print("ax: " + val)
        self.assertAlmostEqual(float(val), -1.0, delta=.05)
        val = self.motor["ay"].get()
        print("ay: " + val)
        self.assertAlmostEqual(float(val), 0.0, delta=.05)
        val = self.motor["az"].get()
        print("az: " + val)
        self.assertAlmostEqual(float(val), 0.0, delta=.05)
        for item in ["gx", "gy", "gz"]:
            val = self.motor[item].get()
            print(item + ": " + val)
            self.assertAlmostEqual(float(val), 0.0, delta=.05)


if __name__ == "__main__":
    if(len(sys.argv) > 1 and sys.argv[1] == '-p'):
        path = sys.argv[2]
        del(sys.argv[1:3])
    unittest.main()