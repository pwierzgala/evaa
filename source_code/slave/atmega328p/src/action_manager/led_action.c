#include "led_action.h"
#include "gpio.h"
#include "message_sender.h"

action_manager_return_t led_power(frame_command_t command,
                                  void *arg,
                                  uint32_t *arg_size)
{
  (void)arg_size;  // to disable warnings

  switch(command)
  {
    case ACK:
      send_ack_message(LED_POWER, arg, arg_size);
      break;
    case GET:
        break;
    case SET:
        if(*(uint8_t*)arg == 0) {
            set_gpio_low(&PORTD, 0x07);
        } else {
            set_gpio_high(&PORTD, 0x07);
        }
        break;
    case RESPONSE:
        send_response_message(LED_POWER, arg, arg_size);
        break;
    default:
        return ACTION_MANAGER_FAILURE;
  }
    return ACTION_MANAGER_SUCCESS;
}
