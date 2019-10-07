#include "humidity_sensor.h"
#include "pcint.h"
#include "timer_16b.h"

#include <string.h>

#define TIMEOUT 10000

static unsigned char buffer[5];

volatile short unsigned singal_length[86];
volatile uint8_t bit_counter;

void dht_sensor_handler()
{
    singal_length[bit_counter++] = get_counter_and_reset();
}

// send initial signal to DHT11 sensor
static void trigger_dht_sensor(void)
{
    set_gpio_output(&PORTC, DHT_PIN);

    set_gpio_low(&PORTC, DHT_PIN);
    _delay_ms(20);

    set_gpio_input(&PORTC, DHT_PIN);
    _delay_us(20);

    set_gpio_high(&PORTC, DHT_PIN);
    _delay_us(20);
}

// Read the relative humidity and temperature from DHT11 sensor.
// return measured data
static uint8_t* get_sensor_data(void)
{
	// buffer variables to receive data
    uint8_t cnt = 7;
    uint8_t idx = 0;

    uint16_t timeout = TIMEOUT;

    // clear buffer
    for (int i=0; i< 5; i++) buffer[i] = 0;

    pcint1_enable();
    _delay_ms(10);
    start_timer_16b();
    trigger_dht_sensor();

    while(bit_counter < 86)
    {
        timeout--;
        if(timeout == 0)
            return 0;
    }

    pcint1_disable();
    stop_timer_16b();
    
    // parse 40 bits of sensor data
    for (int i = 6; i < 85; i+=2)
    {
        // signal length over 60 means logic 1, otherwise 0
        if(singal_length[i] > 60)
            buffer[idx] |= (1 << cnt);

        if (cnt == 0)   // next byte?
        {
            cnt = 7;   
            idx++;
        }
        else cnt--;
    }

    // checksum test
    if (buffer[0]+buffer[1]+buffer[2]+buffer[3] != buffer[4])
        return 0;

    return (uint8_t *)buffer;
}

void read_humidity_and_temperature(DHT_Data_t *dhtData)
{
    uint8_t *data;
    data = get_sensor_data();

    if(data)
        memcpy(dhtData, data, sizeof(DHT_Data_t));
    else
        memset(dhtData, 0xFF, sizeof(DHT_Data_t));
}
