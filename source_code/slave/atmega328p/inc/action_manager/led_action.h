#ifndef _HEADER_LED_ACTION_
#define _HEADER_LED_ACTION_

#include "action_manager.h"

action_manager_return_t led_power(frame_command_t command,
                                  void *arg,
                                  uint32_t *arg_size);

#endif // _HEADER_LED_ACTION_
