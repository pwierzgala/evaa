import unittest
from flask_rest_api_app import *


class FlaskTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.test_controllers = init_controllers()

    def test_dummy(self):
        self.assertEqual(2+2, 4)

    def test_hello(self):
        self.assertTrue(hello().startswith("EVAA REST API"))

    def test_set_light_height(self):
        self.test_controllers[0].set_light_height(height=100)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\x05d\x00\x00\x00a')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_light_too_high(self):
        self.test_controllers[0].set_light_height(height=110)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\x05n\x00\x00\x00z')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_light_too_low(self):
        self.test_controllers[0].set_light_height(height=-10)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\x05\x00\x00\x00\x00'
                                    b'\xff')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_red_light_intensity(self):
        self.test_controllers[0].set_red_light_intensity(percentage=50)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\t\x7f\x00\x00\x00'
                                    b'\xec')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_red_light_intensity_too_high(self):
        self.test_controllers[0].set_red_light_intensity(percentage=130)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\t\xff\x00\x00\x005')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_red_light_intensity_too_low(self):
        self.test_controllers[0].set_red_light_intensity(percentage=-100)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\t\x00\x00\x00\x00'
                                    b'\xde')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_blue_light_intensity(self):
        self.test_controllers[0].set_blue_light_intensity(percentage=10)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\t\x19\x00\x00\x00u')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_blue_light_intensity_too_high(self):
        self.test_controllers[0].set_blue_light_intensity(percentage=123)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\t\xff\x00\x00\x005')
        self.assertEqual(expected_result, self.test_controllers[0].message)

    def test_set_blue_light_intensity_too_low(self):
        self.test_controllers[0].set_blue_light_intensity(percentage=-5)
        expected_result = bytearray(b'\x00\x00\x01\x00\x02\t\x00\x00\x00\x00'
                                    b'\xde')
        self.assertEqual(expected_result, self.test_controllers[0].message)


if __name__ == '__main__':
    unittest.main()
