#ifndef _HEADER_FINITE_STATE_MACHINE_
#define _HEADER_FINITE_STATE_MACHINE_

#define SUCCESS 0
#define FAILURE 1
#define WAITING 2

#define STATES_NUMBER 7

#include "frame_receiver.h"

typedef enum fsm_states_t
{
  IDLE = 0,
  WAITING_FOR_GODOT = IDLE,
  RECV_MSG,
  SEND_RESP,
  PARSE_MSG,
  HNDL_MSG,
  GET_RSLT,
  START  //It MUST be last
} fsm_states;

typedef struct result_buffer_t result_buffer_t;
typedef struct fsm_event_t fsm_event_t;
typedef struct fsm_t fsm_t;
typedef char unsigned (*event)(fsm_event_t *fsm_event);

struct result_buffer_t
{
  uint8_t data[40];
  uint32_t byte_counter;
};

struct fsm_event_t
{
  char unsigned is_valid_frame: 1;
  char unsigned send_ack:       1;
  char unsigned send_response:  1;

  evaa_protocol_frame_t frame;
  result_buffer_t result;
};

struct fsm_t
{
  int current_state;
  int next_state;
  int transient;

  char unsigned start_flag;

  fsm_event_t fsm_event;

  event event_clbk[STATES_NUMBER - 1];

  void (*set_current_state)(fsm_t *fsm, fsm_states state);
  void (*set_next_state)(fsm_t *fsm, fsm_states state);
  void (*set_transient)(fsm_t *fsm, fsm_states state);

  void (*run)();
  void (*make_transition)();
};

fsm_t *get_instance_fsm();
void fsm_init(fsm_t *fsm);

void set_current_state(fsm_t *fsm, fsm_states state);
void set_next_state(fsm_t *fsm, fsm_states state);
void set_transient(fsm_t *fsm, fsm_states state);

char unsigned idle_event(fsm_event_t *fsm_event);
char unsigned receive_message_event(fsm_event_t *fsm_event);
char unsigned send_response_event(fsm_event_t *fsm_event);
char unsigned parse_message_event(fsm_event_t *fsm_event);
char unsigned handle_message_event(fsm_event_t *fsm_event);
char unsigned handle_result_event(fsm_event_t *fsm_event);

void kickstart(fsm_t *fsm);

void prepare_event_handling();

#endif
