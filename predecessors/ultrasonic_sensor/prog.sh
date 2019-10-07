sudo avrdude -c arduino -p atmega328p -b 57600 -P /dev/ttyUSB0 -vv -U flash:w:main.hex
