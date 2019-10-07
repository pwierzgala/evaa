#include "elevator_motor_action.h"
#include "timer_8b.h"
#include "gpio.h"
#include "message_sender.h"

#define LEFT              0
#define RIGHT             1
#define DIRECTION_PIN     0
#define LIFT_TRIGGER_PIN  3
#define STOP              0
#define START             255


action_manager_return_t elevator_motor_data(frame_command_t command,
                                            void *arg,
                                            uint32_t *arg_size)
{
    uint8_t dir  = *(uint8_t*)arg;
    uint8_t move = *((uint8_t*)arg + 1);

    switch(command) {
    case ACK:
      send_ack_message(ELEVATOR_MOTOR_DATA, arg, arg_size);
    break;
    case GET:
        break;
    case SET:
        if(LEFT == dir)
          set_gpio_low(&PORTB, DIRECTION_PIN);
        else
          set_gpio_high(&PORTB, DIRECTION_PIN);

        if(STOP == move)
          stop_tim_8b();
        else if (START == move)
          start_tim_8b();

        break;
    case RESPONSE:
        send_response_message(ELEVATOR_MOTOR_DATA, arg, arg_size);
        break;
    default:
        return ACTION_MANAGER_FAILURE;
    }

    return ACTION_MANAGER_SUCCESS;
}

