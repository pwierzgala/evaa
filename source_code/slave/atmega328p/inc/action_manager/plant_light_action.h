#ifndef _HEADER_PLANT_LIGHT_ACTION_
#define _HEADER_PLANT_LIGHT_ACTION_

#include "action_manager.h"

action_manager_return_t blue_plant_light(frame_command_t command,
                                        void *arg,
                                        uint32_t *arg_size);

action_manager_return_t red_plant_light(frame_command_t command,
                                        void *arg,
                                        uint32_t *arg_size);
#endif // _HEADER_PLANT_LIGHT_ACTION__
