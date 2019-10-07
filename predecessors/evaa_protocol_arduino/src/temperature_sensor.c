#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "crc8.h"
#include "temperature_sensor.h"

// #define HIGH 0x1
// #define LOW  0x0

// #define INPUT 0x0
// #define OUTPUT 0x1

temperatureData_t temperatureData;

static unsigned char digitalRead(int pin) // temporary solucion before gpio.h rework
{
    unsigned char portBuf = PIND; 
    return (portBuf&(1<<pin))>>pin;
}

static unsigned char oneWireReset(void)
{
  unsigned char presenceImpulse;
  set_gpio_output(ONE_WIRE_PORT, ONE_WIRE_PIN); // pinMode out
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(480);
  set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN); //pinMode in
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
   _delay_us(70);

  if (digitalRead(ONE_WIRE_PIN)) {
    presenceImpulse = 0;
  } 
  else {
    presenceImpulse = 1; 
  }
   _delay_us(470);
  return presenceImpulse;
}

static void sendBit(char sBit)
{
   set_gpio_output(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(5);
  if (sBit == 1) {
    set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN);
    set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  }
   _delay_us(80);
  set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN);
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
}

static unsigned char readBit(void)
{
  unsigned char rBit = 0;
  set_gpio_output(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(2);
  set_gpio_input(ONE_WIRE_PORT, ONE_WIRE_PIN);
  set_gpio_low(ONE_WIRE_PORT, ONE_WIRE_PIN);
  _delay_us(10);
  if (digitalRead(ONE_WIRE_PIN)) {
    rBit  = 1;
  }
  return rBit;
}

static void sendByte(unsigned char sByte){
  unsigned char sBit = 0;
  for (unsigned char i = 0; i<8; ++i) {
    sBit = sByte>>i;
    sBit &= 0x01;
    sendBit(sBit); 
  }
   _delay_us(100);
}

static unsigned char readByte(void){
  unsigned char rByte = 0;
  for (unsigned char i = 0; i<8; ++i){
    if (readBit()){
      rByte |= 0x01<<i;
    }
     _delay_us(15);
  }
  return rByte;
}

void measureTemperature(void){
  oneWireReset();
  sendByte(0xCC);
  sendByte(0x44);
  _delay_ms(750); // wait for conversion
}

int readROMAddress(temperatureData_t *tempBuf) {
  unsigned char crc = 0;
  unsigned char ROMCode[8];
  oneWireReset();
  sendByte(0x33); // read ROM with address: 8 bytes
  for (int i = 0; i<8; ++i) {
    ROMCode[i] = readByte();
  }
  crc = dallas_crc_8(ROMCode, 7);
  if( crc == ROMCode[7]) { // CRC correct
    memcpy(tempBuf->address, ROMCode, 8);
    return 0;
  } 
  return 1;
}

int readScratchpad(temperatureData_t *tempBuf) {
  unsigned char crc = 0;
  unsigned char Scratchpad[9];
  Scratchpad[0] = 0;
  Scratchpad[1] = 0;
  tempBuf->value = -123;
  oneWireReset();
  sendByte(0xCC); // skip ROM -> next command adressed to all devices
  sendByte(0xBE); // read ScratchPad 9 bytes
  for (int i = 0; i<9; ++i) {
    Scratchpad[i] = readByte();
  } 
  crc = dallas_crc_8(Scratchpad, 8);
  if( crc == Scratchpad[8] ){ // CRC correct
    tempBuf->value = (float)((((Scratchpad[1]) <<8) + Scratchpad[0])/16.0)*1000;
    return 0;
  }
  //tempBuf->value = Scratchpad[0];

  return 1; //  CRC wrong
}
