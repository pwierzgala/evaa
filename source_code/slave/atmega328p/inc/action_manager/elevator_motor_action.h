#ifndef _HEADER_ELEVATOR_MOTOR_ACTION_
#define _HEADER_ELEVATOR_MOTOR_ACTION_

#include "action_manager.h"

action_manager_return_t elevator_motor_data(frame_command_t command,
                                            void *arg,
                                            uint32_t *arg_size);

#endif // _HEADER_ELEVATOR_MOTOR_ACTION_
