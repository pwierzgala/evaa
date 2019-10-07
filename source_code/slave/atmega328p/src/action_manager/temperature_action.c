#include "temperature_action.h"
#include "temperature_sensor.h"
#include "message_sender.h"

action_manager_return_t temperature_1_wire_sensor(frame_command_t command,
                                                  void *arg,
                                                  uint32_t *arg_size)
{
  switch (command)
  {
    case ACK:
      send_ack_message(TEMPERATURE_1_WIRE_SENSOR, arg, arg_size);
    break;
    case GET:
    {
      temperature_data_t temp_result;
      measure_temperature();
      read_rom_address(&temp_result);
      read_scratchpad(&temp_result);

      set_result((uint8_t *)&temp_result, (uint32_t)sizeof(temperature_data_t));
    }
    break;
    case SET:
      break;
    case RESPONSE:
      send_response_message(TEMPERATURE_1_WIRE_SENSOR, arg, arg_size);
    break;
    default:
      return ACTION_MANAGER_FAILURE;
  }
    return ACTION_MANAGER_SUCCESS;
}
