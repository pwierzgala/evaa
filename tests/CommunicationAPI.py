import time

import RPi.GPIO as GPIO
import serial


class CommunicationAPI:
    ROBOT_LIBRARY_SCOPE = 'GLOBAL'

    def __init__(self):
        self.action_counter = 0
        self.serial_device = serial.Serial()
        self.command = b''

    def configure_port(self, port, baud_rate, timeout=6):
        """
        Configures connection parameters.

        :param port: device port, either a name or a path
        :param baud_rate: speed used to communicate with device
        :param timeout: time after which a failed communication is assumed
        :type port: str
        :type baud_rate: int
        :type timeout: int
        """
        self.serial_device = serial.Serial(port, baud_rate)
        self.serial_device.timeout = timeout

    def send_command(self):
        """ Opens a port and sends a built up command. """
        self.open_connection()
        self.serial_device.reset_input_buffer()
        self.serial_device.write(self.command)
        self.command = b''

    def open_connection(self):
        """ Opens connection with device if it's not yet opened. """
        if not self.serial_device.is_open:
            self.serial_device.open()

    def close_connection(self):
        """ Closes connection with the device if it's not yet closed. """
        if self.serial_device.is_open:
            self.serial_device.close()

    def read(self, size=None):
        self.serial_device.readline()

    def read_data(self, size=None):
        """
        Reads the data sent by the external device until timeout/expected
        size is exceeded or when termination sequence is found.
        :param size: size of the expected data read
        :type size: int
        :return: received data in form of bytes
        :rtype: bytes
        """
        data = self.serial_device.read_until(size=size)
        print(data)
        return data

    def turn_on_led(self):
        """ Appends turning led on message to the built command. """
        self.command = self.command + \
                       B'\x06\x06\x06\x06\x02\x07\x01\x00\x00\x00\xc8'

    def turn_off_led(self):
        """ Appends turning led off message to the built command. """
        self.command = self.command + \
                       B'\x06\x06\x06\x06\x02\x07\x00\x00\x00\x00\x47'

    def switch_led_state(self, led_pin):
        """
        Switches led state, depending on the last sequence of the command or
        actual led state.

        :param led_pin: GPIO pin number of the led
        :type led_pin: int
        """
        #led_pin = int(led_pin)
        if self.command:
            if self.command[-1:] == b'\x16':
                self.turn_on_led()
            else:
                self.turn_off_led()
        else:
            if self.get_input_gpio_state(led_pin):
                self.turn_off_led()
            else:
                self.turn_on_led()

    def set_gpio_state(self, pin, type=GPIO.IN, pull_up_down=GPIO.PUD_DOWN):
        """
        Sets pin state and it's pull resistor state.

        :param pin: number of the GPIO pin to configure
        :param type: purpose of the pin, either input or output
        :param pull_up_down: pull resistor, either connected to DC or GND
        :type pin: int
        :type type: GPIO.IN or GPIO.OUT
        :type pull_up_down: GPIO.PUD_UP or GPIO.PUD_DOWN or GPIO.PUD_OFF
        """
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pin, type, pull_up_down=pull_up_down)

    def get_input_gpio_state(self, pin):
        """
        Retrieves pin state in a form of a bolean type.

        :param pin: number of the GPIO pin to get input from.
        :type pin: int

        :return: True if pin state is HIGH, otherwise False
        :rtype: bool
        """
        #pin = int(pin)
        return True if GPIO.input(pin) else False

    def count_when_pin_state_changes(self, pin):
        """
        Connects an increasing counter event in case of any change of the
        pin state.
        :param pin: number of the GPIO pin to connect event to
        :type pin: int
        """
        GPIO.add_event_detect(pin, GPIO.BOTH, self.increase_counter)

    def reset_pin_notifiers(self, pin):
        """
        Disconnects every event to perform when pin state changes.
        :param pin: number of the GPIO pin to disconnect events from
        :type pin: int
        """
        GPIO.remove_event_detect(pin)

    def reset_counter(self):
        """ Resets the GPIO pin's state's changes' counter to 0."""
        self.action_counter = 0

    def increase_counter(self, _):
        """ Increments the GPIO pin's state's changes' counter. """
        self.action_counter = self.action_counter + 1

    def get_counter(self):
        """
        Returns GPIO pin's state's changes' counter.
        :return: GPIO pin's state's changes' counter.
        :rtype: int
        """
        return self.action_counter

    def spam_led_on_off(self, led_pin, period=0.1, duration=10):
        """
        A custom function to spam the device with led on/off commands.
        :param led_pin: number of the GPIO pin
        :param period: every what time (in seconds) a message shall be sent
        :param duration: for how long (in seconds) the spamming should be
        performed
        :type led_pin: int
        :type period: int
        :type duration: int
        """
        end_time = time.time() + duration

        while time.time() < end_time:
            started = time.time()
            self.switch_led_state(led_pin)
            self.switch_led_state(led_pin)
            self.send_command()
            delay = period - (time.time() - started)
            time.sleep(max(0.0, delay))


if __name__ == "__main__":

    api = CommunicationAPI()
    api.configure_port("/dev/ttyUSB0", 9600, 5)
    api.set_gpio_state(14)
    api.count_when_pin_state_changes(14)

    api.switch_led_state(14)
    api.switch_led_state(14)
    api.switch_led_state(14)

    api.open_connection()
    api.send_command()
    data = api.serial_device.readlines()
    print(data)
