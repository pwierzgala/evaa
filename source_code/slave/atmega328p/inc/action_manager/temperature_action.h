#ifndef _HEADER_TEMPERATURE_ACTION_
#define _HEADER_TEMPERATURE_ACTION_

#include "action_manager.h"

struct {
  unsigned char address[8];
  float value;
} temperature_data;

action_manager_return_t temperature_1_wire_sensor(frame_command_t command,
                                                  void *arg,
                                                  uint32_t *arg_size);

#endif // _HEADER_TEMPERATURE_ACTION_
