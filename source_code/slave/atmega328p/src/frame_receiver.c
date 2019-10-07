#include "frame_receiver.h"
#include "crc8.h"
#include "timer_16b.h"

#include "usart.h"


bool is_frame_ready_to_read(rx_buffer_t *rx_buffer);

static bool is_frame_complete(frame_slot_t *frame);
static bool is_frame_free(frame_slot_t *frame);
static bool is_frame_empty(frame_slot_t *frame);


static char unsigned frame_counter = 0;

rx_buffer_t *get_buffer()
{
  static rx_buffer_t rx_buffer;
  return &rx_buffer;
}

//########## BUFFER STATUS BEGIN  ##########

bool is_frame_ready_to_read(rx_buffer_t *rx_buffer)
{
  return rx_buffer->frame[rx_buffer->read_frame_index].ready;
}

static bool is_frame_free(frame_slot_t *frame)
{
  return false == frame->ready && is_frame_empty(frame);
}

static bool is_frame_empty(frame_slot_t *frame)
{
  return 0 == frame->data_byte_index;
}

static bool is_frame_complete(frame_slot_t *frame)
{
  return (sizeof(evaa_protocol_frame_t) == frame->data_byte_index);
}

char unsigned count_used_frames()
{
  return frame_counter;
}

char unsigned is_buffer_full()
{
  return RX_BUFFER_SLOTS == count_used_frames();
}

void inc_frame_counter()
{
  ++frame_counter;
}

void dec_frame_counter()
{
  --frame_counter;
}

//########## BUFFER STATUS END  ##########

//########## BUFFER OPERATIONS BEGIN  ##########

frame_receiver_return_t init_rx_buffer(rx_buffer_t *rx_buffer)
{
    memset(rx_buffer, 0, sizeof(rx_buffer_t));
    return FRAME_RECEIVER_SUCCESS;
}

void clear_pointed_slot(frame_slot_t *slot)
{
  if(slot->data_byte_index < sizeof(evaa_protocol_frame_t))
  {
    memset(&slot->data, 0, sizeof(evaa_protocol_frame_t));
    slot->ready           = 0;
    slot->data_byte_index = 0;
  }
}

void clear_slot()
{
  rx_buffer_t *buffer = get_buffer();
  clear_pointed_slot(&buffer->frame[buffer->write_frame_index]);
}

frame_slot_t *get_frame(uint16_t frame_index)
{
  return &get_buffer()->frame[frame_index];
}

void check_and_wrap_index(uint16_t *frame_index)
{
  // jump to the first slot after the last (circularity)
  if(*frame_index >= RX_BUFFER_SLOTS)
  {
    *frame_index = 0;
  }
}

void set_status_for_master_notify(frame_slot_t *frame_slot)
{
  if(is_buffer_full() && is_frame_complete(frame_slot))
    get_buffer()->device_status_code = STATUS_BUSY;
  else
    get_buffer()->device_status_code = STATUS_OK;
}

frame_receiver_return_t write_byte_to_rx_buffer(rx_buffer_t *rx_buffer, uint8_t data_byte)
{
  uint16_t *frame_index     = &rx_buffer->write_frame_index;
  frame_slot_t *frame_slot  = get_frame(*frame_index);
  uint16_t *data_byte_index = &frame_slot->data_byte_index;

  if(is_buffer_full() && is_frame_complete(frame_slot))
    return FRAME_RECEIVER_BUFFER_FULL;

  frame_slot->data.single_byte[*data_byte_index] = data_byte;
  *data_byte_index += 1;

  // clean after amount of time elapsed
  if(1 == *data_byte_index)
    start_timer_16b(TIM_16B_PRESCALER_256);

  // end of receiving of a single frame; move to next buffer frame slot
  if(is_frame_complete(frame_slot))
  {
    stop_timer_16b();
    frame_slot->ready = true;
    inc_frame_counter();

    *frame_index += 1;
  }

  check_and_wrap_index(frame_index);
  return FRAME_RECEIVER_SUCCESS;
}

frame_receiver_return_t read_frame_from_rx_buffer(rx_buffer_t *rx_buffer, evaa_protocol_frame_t *frame_buffer)
{
  if(NULL == frame_buffer)
  {
    return FRAME_RECEIVER_WRONG_FRAME_BUFFER;
  }

  uint16_t *frame_index    = &rx_buffer->read_frame_index;
  frame_slot_t *frame_slot = get_frame(*frame_index);
  *frame_index += 1;

  if(is_frame_free(frame_slot))
  {
    return FRAME_RECEIVER_NO_FRAME_TO_READ;
  }

  set_status_for_master_notify(frame_slot);

  // copy frame to buffer
  memcpy(frame_buffer, &frame_slot->data, sizeof(evaa_protocol_frame_t));

  // set slot's ready flag to false and clean frame's CRC8;
  // protection against retrying of used frame parsing
  frame_slot->ready           = false;
  frame_slot->data.crc8       = 0;
  frame_slot->data_byte_index = 0;

  dec_frame_counter();

  // increment frame_slot_index; set to 0 when higher than available number of slots
  check_and_wrap_index(frame_index);
  return FRAME_RECEIVER_SUCCESS;
}

//########## BUFFER OPERATIONS END  ##########

evaa_protocol_frame_t create_response(uint8_t  command,
                                      uint8_t  number_of_chunks,
                                      uint8_t  chunk_index,
                                      uint8_t  data_type,
                                      uint32_t data)
{
  evaa_protocol_frame_t resp;

  resp.device_id        = 0x06;
  resp.sensor_id        = 0x06;
  resp.number_of_chunks = number_of_chunks;
  resp.chunk_index      = chunk_index;
  resp.command          = command;
  resp.data_type        = data_type;
  resp.data             = data;
  resp.crc8             = dallas_crc_8((char unsigned *)&resp, sizeof(evaa_protocol_frame_t) - sizeof(resp.crc8));

  return resp;
}
