#ifndef _HEADER_HUMIDITY_ACTION_
#define _HEADER_HUMIDITY_ACTION_

#include "action_manager.h"

action_manager_return_t read_humidity_sensor(frame_command_t command,
                                  void *arg,
                                  uint32_t *arg_size);
    
#endif // _HEADER_HUMIDITY_ACTION_