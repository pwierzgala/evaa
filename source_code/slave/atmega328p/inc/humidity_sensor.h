#include "gpio.h"
#include <util/delay.h>

#define DHT_PIN  1
#define DHT_PORT &PORTC

typedef struct {
    uint8_t hum_int;
    uint8_t hum_decimal;
    uint8_t temp_int;
    uint8_t temp_decimal;
} DHT_Data_t;

void read_humidity_and_temperature(DHT_Data_t*);
void dht_sensor_handler(void);