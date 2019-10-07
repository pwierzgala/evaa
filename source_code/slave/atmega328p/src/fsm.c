#include "fsm.h"
#include "gpio.h"
#include "usart.h"
#include "utils.h"
#include "frame_decoder.h"
#include "action_manager.h"
#include "command.h"
#include "pwm_8b.h"
#include "timer_16b.h"

#define TRANSITION_TIME_MS 80
#define DEBUG


fsm_states state_table[STATES_NUMBER][3] =
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

char unsigned idle_event(fsm_event_t *fsm_event)
{
  (void)fsm_event; // to disable warning
  return SUCCESS;
}

char unsigned receive_message_event(fsm_event_t *fsm_event)
{
  if(FRAME_RECEIVER_SUCCESS != read_frame_from_rx_buffer(get_buffer(), &fsm_event->frame))
  {
    return FAILURE;
  }

  fsm_event->send_ack = 1;

  return SUCCESS;
}

char unsigned send_response_event(fsm_event_t *fsm_event)
{
  if(fsm_event->send_ack)
  {
    fsm_event->result.data[0] = get_buffer()->device_status_code;
    fsm_event->result.byte_counter = 1;

    send_response(fsm_event->frame.data_type, ACK, &fsm_event->result);
    fsm_event->send_ack = 0;
  }
  else if(fsm_event->send_response)
  {
    send_response(fsm_event->frame.data_type, RESPONSE, get_result());
    fsm_event->send_response = 0;
    clear_result();
  }

  return SUCCESS;
}

char unsigned parse_message_event(fsm_event_t *fsm_event)
{
  frame_decoder_return_t result = check_frame_integrity(fsm_event->frame);
  if(FRAME_DECODER_SUCCESS != result)
  {
    fsm_event->is_valid_frame = 0;
    return FAILURE;
  }
  fsm_event->is_valid_frame = 1;
  return SUCCESS;
}

void prepare_event_handling()
{
  usart_int_disable();            // disable usart
  stop_timer_16b();               // stop if running
  clear_slot();                   // clear slot if not empty
  (void)get_counter_and_reset();  // get timer value and reset
}

char unsigned handle_message_event(fsm_event_t *fsm_event)
{
#ifndef DEBUG
  prepare_event_handling();
#endif

  action_manager_return_t rc = ACTION_MANAGER_SUCCESS;
  uint32_t data_size = sizeof(fsm_event->frame.data);

  rc = get_action(fsm_event->frame.data_type)(fsm_event->frame.command,
                                              &fsm_event->frame.data,
                                              &data_size);

  if(ACTION_MANAGER_FAILURE == rc) {
    return FAILURE;
  }

  return SUCCESS;
}

char unsigned handle_result_event(fsm_event_t *fsm_event)
{
  //set_result(fsm_event->result.data, fsm_event->result.byte_counter);

  if(get_result()->byte_counter > 0)
    fsm_event->send_response = 1;

  usart_int_enable();
  // Place in which we can do something with result
  return SUCCESS;
}

void set_current_state(fsm_t *fsm, fsm_states state)
{
  fsm->current_state = state;
}

void set_next_state(fsm_t *fsm, fsm_states state)
{
  for(int i = 0; i < STATES_NUMBER; ++i)
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

void make_transition(fsm_t *fsm)
{
  if(fsm->transient != fsm->next_state)
  {
    // Create 1 extra state which not exist in state table
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

void kickstart(fsm_t *fsm)
{
  if(IDLE == fsm->current_state)
    fsm->start_flag = 1;
}

void run()
{
  char unsigned rc = FAILURE;
  fsm_t *fsm       = get_instance_fsm();

  while(1)
  {

    if(fsm->start_flag)
    {
      set_current_state(fsm, START);
      set_next_state(fsm, START);
      fsm->start_flag = 0;
    }

    if(START != fsm->current_state)
    {
      rc = fsm->event_clbk[fsm->current_state](&(fsm->fsm_event));
      if(FAILURE == rc)
      {
        fsm->current_state = IDLE;
        fsm->next_state    = IDLE;
        fsm->transient     = IDLE;
      }
    }

    if((IDLE == fsm->current_state) && (count_used_frames()))
    {
      fsm->start_flag = 1;
    }

    make_transition(fsm);

    _delay_ms(TRANSITION_TIME_MS);
  }
}

fsm_t *get_instance_fsm()
{
  static fsm_t fsm;
  return &fsm;
}

void fsm_init(fsm_t *fsm)
{
  fsm->current_state     = IDLE;
  fsm->next_state        = IDLE;
  fsm->transient         = IDLE;
  fsm->start_flag        = 0;

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

  fsm->fsm_event.is_valid_frame          = 0;
  fsm->fsm_event.send_ack                = 0;
  fsm->fsm_event.send_response           = 0;
}
