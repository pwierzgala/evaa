#include <string.h>

#include "action_manager.h"
#include "temperature_action.h"
#include "led_action.h"
#include "plant_light_action.h"
#include "elevator_motor_action.h"
#include "lift_engine_action.h"
#include "ultrasonic_sensor.h"
#include "ultrasonic_sensor_action.h"
#include "ph_sensor_action.h"
#include "humidity_action.h"

// C feature: square brackets define index value
static action_callback action[] = {
    [LED_POWER]                 = led_power,
    [BLUE_PLANT_LIGHT]          = blue_plant_light,
    [ELEVATOR_MOTOR_DATA]       = elevator_motor_data,
    [TEMPERATURE_1_WIRE_SENSOR] = temperature_1_wire_sensor,
    [WATER_LEVEL_SENSOR]        = water_level_sensor,
    [PH_SENSOR_DATA]            = read_ph_sensor,
    [HUMIDITY_SENSOR_DATA]      = read_humidity_sensor,
    [DATA_TYPE_LAST]            = do_nothing
};

static result_buffer_t result_buff = { 0 };

action_callback get_action(frame_data_type_t data_type)
{
    if(DATA_TYPE_LAST < data_type || ERROR > data_type) {
        return do_nothing;
    }

    action_callback action_func = action[data_type];

    if(NULL == action_func) {
        return do_nothing;
    }
    return action_func;
}

action_manager_return_t send_response(frame_data_type_t data_type, uint8_t command, result_buffer_t *result)
{
  //command - ACK or RESPONSE
  return get_action(data_type)(command, result->data, &result->byte_counter);
}

bool set_result(uint8_t *data, uint32_t size)
{
  if(NULL == data)
    return false;

  if(size > 40)
    return false;

  memcpy(result_buff.data, data, size);
  result_buff.byte_counter = size;

  return true;
}

//Do not release memory returned by get_result() !!!!
result_buffer_t *get_result()
{
  return &result_buff;
}

void clear_result()
{
  memset(&result_buff, 0, sizeof(result_buffer_t));
}

action_manager_return_t do_nothing(frame_command_t command,
                                   void *arg,
                                   uint32_t *arg_size)
{
    (void)command;       // to disable warnings
    arg = (uint8_t*)arg; // to disable warnings
    (void)arg_size;      // to disable warnings
    return ACTION_MANAGER_SUCCESS;
}
