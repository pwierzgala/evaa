#ifndef _HEADER_TEMPERATURE_SENSOR_
#define _HEADER_TEMPERATURE_SENSOR_

#include "gpio.h"

#define ONE_WIRE_PIN 2
#define ONE_WIRE_PORT &PORTD

typedef struct {
  unsigned char address[8];
  int value;
} temperatureData_t;

void measureTemperature(void);
int readROMAddress(temperatureData_t*);
int readScratchpad(temperatureData_t*);

#endif // _HEADER_TEMPERATURE_SENSOR_
