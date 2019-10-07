import time

from serial_thread import SerialThread

# An example usage:
# you create a thread that sits on a specific device and then you issue a
# commands to them, remember that you need to give a delay between the
# commands in order to perform them correctly
if __name__ == "__main__":
    thread = SerialThread("/dev/ttyUSB0", log_file="log.txt", baud_rate=9600)
    thread.start()

    thread.led_on()
    time.sleep(4)
    thread.led_off()
    time.sleep(1)
