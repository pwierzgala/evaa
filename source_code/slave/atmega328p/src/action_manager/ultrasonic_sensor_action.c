#include "ultrasonic_sensor_action.h"
#include "ultrasonic_sensor.h"
#include "message_sender.h"
#include <util/delay.h>

action_manager_return_t water_level_sensor(frame_command_t command,
                                           void *arg,
                                           uint32_t *arg_size)
{
    switch (command)
    {
    case ACK:
        send_ack_message(WATER_LEVEL_SENSOR, arg, arg_size);
        break;
    case GET:
            trigger_us_sensor();
            _delay_ms(350);
            *((uint16_t*)(arg)) = get_distance();
            *arg_size = sizeof(arg);
        break;
    case SET:
        break;
    case RESPONSE:
        send_response_message(WATER_LEVEL_SENSOR, arg, arg_size);
        break;
    default:
        return ACTION_MANAGER_FAILURE;
    }

    return ACTION_MANAGER_SUCCESS;
}
