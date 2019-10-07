#ifndef _HEADER_TEMPERATURE_SENSOR_
#define _HEADER_TEMPERATURE_SENSOR_

#include "gpio.h"

#define ONE_WIRE_PIN 1
#define ONE_WIRE_PORT &PORTB

typedef struct {
  unsigned char address[8];
  int value;
} temperature_data_t;

void measure_temperature(void);
int read_rom_address(temperature_data_t *);
int read_scratchpad(temperature_data_t *);

#endif // _HEADER_TEMPERATURE_SENSOR_
