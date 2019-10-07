#ifndef _HEADER_TEMPERATURE_ACTION_
#define _HEADER_TEMPERATURE_ACTION_

#include "action_manager.h"

#define ONE_WIRE_BUS 2

struct {
  unsigned char address[8];
  float value;
} temperatureData;

action_manager_return_t read_temperature_sensor(frame_command_t command,
                                  void *arg,
                                  uint32_t *arg_size);
    
#endif // _HEADER_TEMPERATURE_ACTION_
