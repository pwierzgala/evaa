#include "humidity_action.h"
#include "gpio.h"
#include <avr/io.h>
#include <util/delay.h>
#include "humidity_sensor.h"
#include "message_sender.h"

action_manager_return_t read_humidity_sensor(frame_command_t command,
                                         void *arg,
                                         uint32_t *arg_size)
{
    switch (command)
    {
        case ACK:
            send_ack_message(HUMIDITY_SENSOR_DATA, arg, arg_size);
            break;
        case GET:
        {
            DHT_Data_t hum_and_temp_data;

            read_humidity_and_temperature(&hum_and_temp_data);
            set_result((uint8_t *)&hum_and_temp_data, (uint32_t)sizeof(DHT_Data_t));
            break;
        }
        case SET:
            break;
        case RESPONSE:
            send_response_message(HUMIDITY_SENSOR_DATA, arg, arg_size);
            break;
        default:
            return ACTION_MANAGER_FAILURE;
    }

    return ACTION_MANAGER_SUCCESS;
}