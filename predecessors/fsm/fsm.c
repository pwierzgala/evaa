#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fsm.h"

fsm_states state_table[7][3] =
{
  //CURR              NEXT               TRAN
  {WAITING_FOR_GODOT, WAITING_FOR_GODOT, WAITING_FOR_GODOT},
  {START,             RECV_MSG,          RECV_MSG},
  {RECV_MSG,          PARSE_MSG,         SEND_RESP},
  {PARSE_MSG,         HNDL_MSG,          HNDL_MSG},
  {HNDL_MSG,          GET_RSLT,          GET_RSLT},
  {GET_RSLT,          SEND_RESP,         SEND_RESP},
  {SEND_RESP,         IDLE,              IDLE}
};

char unsigned idle_event()
{
  printf("waiting for godot\n");
  return SUCCESS;
}

char unsigned receive_message_event()
{
  printf("receive_message_event\n");
  return SUCCESS;
}

char unsigned send_response_event()
{
  printf("send_response_event\n");
  return SUCCESS;
}

char unsigned parse_message_event()
{
  printf("parse message\n");
  return SUCCESS;
}

char unsigned handle_message_event()
{
  printf("handle message\n");
  return SUCCESS;
}

char unsigned handle_result_event()
{
  printf("result message\n");
  return SUCCESS;
}

void set_current_state(fsm_t *fsm, fsm_states state)
{
  fsm->current_state = state;
}

void set_next_state(fsm_t *fsm, fsm_states state)
{
  for(int i = 0; i < 7; ++i)
  {
    if(state == state_table[i][0])
    {
      fsm->next_state = state_table[i][1];
      fsm->transient  = state_table[i][2];
    }
  }
}

void set_transient(fsm_t *fsm, fsm_states state)
{
  fsm->transient = state;
}

char const *state_to_string(fsm_states state)
{
  switch(state)
  {
    case IDLE:
      return "idle";
    case RECV_MSG:
      return "receive message";
    case SEND_RESP:
      return "send response";
    case PARSE_MSG:
      return "parse message";
    case HNDL_MSG:
      return "handle mesage";
    case START:
      return "Let the work begin !!!";
    default:
      return "unknown state";
  }

  return "Weird error occure :/";
}

void make_transition(fsm_t *fsm)
{
  if(fsm->transient != fsm->next_state)
  {
    set_next_state(fsm, fsm->current_state);
    set_current_state(fsm, fsm->transient);
    set_transient(fsm, fsm->next_state);
  }
  else
  {
    set_current_state(fsm, fsm->next_state);
    set_next_state(fsm, fsm->current_state);
  }
}

void run()
{
  srand(0);
  char unsigned rc = SUCCESS;
  fsm_t *fsm = get_instance_fsm();

  while(1)
  {
    if(rand() % 100 == 93)
    {
      set_next_state(fsm, START);
      set_current_state(fsm, START);
    }

    if(START != fsm->current_state)
    {
      rc = fsm->event_clbk[fsm->current_state]();
      if(rc != SUCCESS)
        fsm->next_state = IDLE;
    }

    make_transition(fsm);

    sleep(1);
  }
}

fsm_t *get_instance_fsm()
{
  static fsm_t fsm;
  return &fsm;
}

void init_fsm(fsm_t *fsm)
{
  fsm->current_state     = IDLE;
  fsm->next_state        = IDLE;
  fsm->transient         = IDLE;

  fsm->event_clbk[0]     = idle_event;
  fsm->event_clbk[1]     = receive_message_event;
  fsm->event_clbk[2]     = send_response_event;
  fsm->event_clbk[3]     = parse_message_event;
  fsm->event_clbk[4]     = handle_message_event;
  fsm->event_clbk[5]     = handle_result_event;

  fsm->set_current_state = set_current_state;
  fsm->set_next_state    = set_next_state;
  fsm->set_transient     = set_transient;

  fsm->run               = run;
  fsm->make_transition   = make_transition;
}

