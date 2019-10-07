#include "ph_sensor_action.h"
#include "ph_sensor.h"
#include "message_sender.h"

action_manager_return_t read_ph_sensor(frame_command_t command,
                                       void *arg,
                                       uint32_t *arg_size)
{
  switch (command)
  {
    case ACK:
      send_ack_message(PH_SENSOR_DATA, arg, arg_size);
    break;
    case GET:
    {
      int ph = get_ph();
      set_result((uint8_t *)&ph, (uint32_t)sizeof(int));
    }
    break;
    case SET:
      break;
    case RESPONSE:
      send_response_message(PH_SENSOR_DATA, arg, arg_size);
    break;
    default:
      return ACTION_MANAGER_FAILURE;
  }
    return ACTION_MANAGER_SUCCESS;
}
