#include "temperature_action.h"
#include "gpio.h"
#include <avr/io.h>
#include <util/delay.h>
#include <temperature_sensor.h>

action_manager_return_t read_temperature_sensor(frame_command_t command,
                                         void *arg,
                                         uint32_t *arg_size)
{
    
    switch (command)
    {
    case ACK:
        break;
    case GET:
            *arg_size = sizeof(temperatureData_t);
            measureTemperature();
            readROMAddress(arg);
            readScratchpad(arg);
        break;
    case SET:
        break;
    case RESPONSE:
        break;
    default:
        return ACTION_MANAGER_FAILURE;
    }

    return ACTION_MANAGER_SUCCESS;
}
