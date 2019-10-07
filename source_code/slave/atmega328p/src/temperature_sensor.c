//#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "crc8.h"
#include "temperature_sensor.h"

// DS18B20 ROM commands
#define DS18B20_ROM_READ            0x33
#define DS18B20_ROM_SKIP            0xCC

// DS18B20 Function Commands
#define DS18B20_CMD_CONVERT         0x44
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE

temperature_data_t temperature_data;

static uint8_t digital_read(uint8_t pin) // temporary solution before gpio.h rework
{
    uint8_t portBuf = PINB;
    return (portBuf & (1 << pin)) >> pin;
}

static uint8_t one_wire_reset(void)
{
  uint8_t presence_impulse;
  set_gpio_output(ONE_WIRE_PORT, ONE_WIRE_PIN); // pinMode out
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(480);

  set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN); // pinMode in
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(70);

  if (digital_read(ONE_WIRE_PIN)) {
    presence_impulse = 0;
  }
  else {
    presence_impulse = 1;
  }
   _delay_us(470);
  return presence_impulse;
}

static void send_bit(uint8_t bit)
{
  set_gpio_output(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(5);

  if (bit)
  {
    set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN);
    set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  }

  _delay_us(80);
  set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN);
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
}

static uint8_t read_bit(void)
{
  set_gpio_output(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(2);

  set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN);
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(10);

  return digital_read(ONE_WIRE_PIN);
}

static void send_byte(uint8_t byte)
{
  for (uint8_t i = 0; i < 8; ++i)
  {
    send_bit((byte >> i) & 0x01);
  }
   _delay_us(100);
}

static uint8_t read_byte(void)
{
  uint8_t byte = 0;
  for (uint8_t i = 0; i < 8; ++i)
  {
    byte |= read_bit() << i;
    _delay_us(15);
  }
  return byte;
}

void measure_temperature(void){
  one_wire_reset();
  send_byte(DS18B20_ROM_SKIP);
  send_byte(DS18B20_CMD_CONVERT);
  _delay_ms(750); // wait for conversion minimum 750ms
}

int read_rom_address(temperature_data_t *temp_buf) {
  unsigned char crc = 0;
  unsigned char rom_code[8];
  one_wire_reset();
  send_byte(DS18B20_ROM_READ); // read ROM with address: 8 bytes

  for (int i = 0; i<8; ++i)
  {
    rom_code[i] = read_byte();
  }

  crc = dallas_crc_8(rom_code, 7);

  if(crc == rom_code[7])
  {
    memcpy(temp_buf->address, rom_code, 8);
    return 0;
  }
  return -1;
}

int read_scratchpad(temperature_data_t *temp_buf) {
  unsigned char crc = 0;
  unsigned char scratchpad[9];

  one_wire_reset();
  send_byte(DS18B20_ROM_SKIP); // skip ROM -> next command adressed to all devices
  send_byte(DS18B20_CMD_READ_SCRATCHPAD); // read ScratchPad 9 bytes; Byte 0 -> Temperature LSB; Byte 1 -> Temperature MSB;

  for (int i = 0; i < 9; ++i)
  {
    scratchpad[i] = read_byte();
  }

  crc = dallas_crc_8(scratchpad, 8);

  if(crc == scratchpad[8])
  {
    temp_buf->value = (float)((((scratchpad[1]) << 8) + scratchpad[0]) / 16.0) * 1000;
    return 0;
  }
  return -1; //  CRC wrong
}
