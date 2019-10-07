#ifndef _HEADER_ACTION_MANAGER_
#define _HEADER_ACTION_MANAGER_

#include <stdint.h>
#include "frame_decoder.h"

#include "fsm.h"

typedef enum
{
    ACTION_MANAGER_SUCCESS =  0,
    ACTION_MANAGER_FAILURE = -1
} action_manager_return_t;

typedef action_manager_return_t (*action_callback)(frame_command_t command,
                                                   void *arg,
                                                   uint32_t *arg_size);

action_callback get_action(frame_data_type_t data_type);
action_manager_return_t send_response(frame_data_type_t data_type, uint8_t command, result_buffer_t *result);

bool set_result(uint8_t *data, uint32_t size);
//Do not release memory returned by get_result() !!!!
result_buffer_t *get_result();

void clear_result();

action_manager_return_t do_nothing(frame_command_t command,
                                   void *arg,
                                   uint32_t *arg_size);

#endif // _HEADER_ACTION_MANAGER_
