#include "plant_light_action.h"
#include "pwm_8b.h"
#include "gpio.h"
#include "message_sender.h"

#define PLANT_LIGHT_ENABLE_PIN 5

action_manager_return_t blue_plant_light(frame_command_t command,
                                         void *arg,
                                         uint32_t *arg_size)
{
  (void)arg_size;  // to disable warnings

  switch(command)
  {
    case ACK:
      send_ack_message(BLUE_PLANT_LIGHT, arg, arg_size);
      break;
    case GET:
      break;
    case SET:
      {
        uint8_t duty_reg_val = *(uint8_t*)arg;
        if (0 == duty_reg_val)
        {
          pwm_8b_stop();
          set_gpio_low(&PORTD, PLANT_LIGHT_ENABLE_PIN);
        }
        else
        {
          pwm_8b_set_duty(*(uint8_t*)arg);
          set_gpio_high(&PORTD, PLANT_LIGHT_ENABLE_PIN);
          pwm_8b_start();
        }
      }
      break;
    case RESPONSE:
      send_response_message(BLUE_PLANT_LIGHT, arg, arg_size);
      break;
    default:
        return ACTION_MANAGER_FAILURE;
  }

  return ACTION_MANAGER_SUCCESS;
}

action_manager_return_t red_plant_light(frame_command_t command,
                                         void *arg,
                                         uint32_t *arg_size)
{
  (void)arg_size;  // to disable warnings

  switch(command)
  {
    case ACK:
      send_ack_message(RED_PLANT_LIGHT, arg, arg_size);
      break;
    case GET:
      break;
    case SET:
      break;
    case RESPONSE:
      send_response_message(RED_PLANT_LIGHT, arg, arg_size);
      break;
    default:
        return ACTION_MANAGER_FAILURE;
  }
  return ACTION_MANAGER_SUCCESS;
}
