#!/usr/bin/env python3

import motor
import unittest

path = None
class TestMotor(unittest.TestCase):
    m = None
    f = None

    @classmethod
    def connect(cls):
        cls.m = motor.MotorManager()
        cls.m.get_motors_uart_by_devpath(["/dev/ttyACM0"], True)
        cls.m.set_auto_count()

    @classmethod
    def setUpClass(cls):
        cls.connect()

    @classmethod
    def tearDownClass(cls):
        cls.m.clear_commands()
        cls.m.set_command_mode(motor.ModeDesired.Open)
        cls.m.write_saved_commands()

    def tearDown(self):
        self.m.clear_commands()
        self.m.set_command_mode(motor.ModeDesired.Open)
        self.m.write_saved_commands()

    def test_3v3(self):
        v3v3 = self.m.motors()[0]["3v3"].get()
        v3v3 = float(v3v3)
        self.assertAlmostEqual(v3v3, 3.3, delta=0.1)

if __name__ == "__main__":
    unittest.main()
