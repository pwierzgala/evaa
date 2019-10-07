#ifndef _HEADER_PH_SENSOR_ACTION_
#define _HEADER_PH_SENSOR_ACTION_

#include "action_manager.h"

action_manager_return_t read_ph_sensor(frame_command_t command,
                                       void *arg,
                                       uint32_t *arg_size);

#endif // _HEADER_TEMPERATURE_ACTION_
