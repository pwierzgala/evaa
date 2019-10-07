rm -rf build
mkdir build
cd build
avr-gcc -g -Os -Wall -Wextra -pedantic -mmcu=atmega328p \
    -I../inc/ -c ../src/utils.c ../src/timer_8b.c ../src/timer_16b.c \
    ../src/int.c ../src/gpio.c ../src/usart.c ../src/frame_receiver.c \
    ../src/main.c \
    $@
avr-gcc -g -mmcu=atmega328p -o main.elf utils.o timer_8b.o timer_16b.o \
    int.o gpio.o usart.o frame_receiver.o main.o \
    $@
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avr-size --format=avr --mcu=atmega328p main.elf
