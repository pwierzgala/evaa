#ifndef _HEADER_FINITE_STATE_MACHINE_
#define _HEADER_FINITE_STATE_MACHINE_

#define SUCCESS 0
#define FAILURE 1

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

typedef char unsigned (*event)(void);

typedef struct fsm_t fsm_t;
struct fsm_t
{
  int current_state;
  int next_state;
  int transient;

  event event_clbk[6];

  void (*set_current_state)(fsm_t *fsm, fsm_states state);
  void (*set_next_state)(fsm_t *fsm, fsm_states state);
  void (*set_transient)(fsm_t *fsm, fsm_states state);

  void (*run)();
  void (*make_transition)();

};

fsm_t *get_instance_fsm();
void init_fsm(fsm_t *fsm);

void set_current_state(fsm_t *fsm, fsm_states state);
void set_next_state(fsm_t *fsm, fsm_states state);
void set_transient(fsm_t *fsm, fsm_states state);

char unsigned idle_event();
char unsigned receive_message_event();
char unsigned send_response_event();
char unsigned parse_message_event();
char unsigned handle_message_event();
char unsigned handle_result_event();

#endif
