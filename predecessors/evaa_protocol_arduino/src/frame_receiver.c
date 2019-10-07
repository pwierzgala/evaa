#include "frame_receiver.h"

static bool is_frame_complete(uint16_t data_byte_index);
bool is_frame_ready(rx_buffer_t *rx_buffer);

static bool is_slot_free(frame_slot_t *frame);
static bool is_slot_empty(frame_slot_t *frame);


rx_buffer_t *get_buffer()
{
  static rx_buffer_t rx_buffer;
  return &rx_buffer;
}

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
  if(is_slot_free(&buffer->frame_overflow))
    clear_pointed_slot(&buffer->frame_overflow);
  else
    clear_pointed_slot(&buffer->frame[buffer->write_frame_index]);
}

char unsigned count_used_slots(rx_buffer_t *rx_buffer)
{
  char unsigned counter = 0;
  for(short i = 0; i < RX_BUFFER_SLOTS; ++i)
  {
    counter += (rx_buffer->frame[i].ready == true);
  }

  return counter;
}

char unsigned is_buffer_full()
{
  return RX_BUFFER_SLOTS == count_used_slots(get_buffer());
}

frame_receiver_return_t write_byte_to_rx_buffer(rx_buffer_t *rx_buffer, uint8_t data_byte)
{
  uint16_t *frame_slot_index = &rx_buffer->write_frame_index;
  frame_slot_t *frame_slot;

  // buffer is full but we still have to send correct response...
  if(is_buffer_full())
    frame_slot = &rx_buffer->frame_overflow;
  else
    frame_slot = &rx_buffer->frame[*frame_slot_index];

  uint16_t *data_byte_index  = &frame_slot->data_byte_index;

  // if slot pointing by frame_slot_index is full
  // we got nothing to do then
  if(!is_slot_free(frame_slot))
  {
    return FRAME_RECEIVER_RX_SLOT_FULL;
  }

  frame_slot->data.single_byte[*data_byte_index] = data_byte;
  // if we received byte we should increment index
  *data_byte_index += 1;

  // clean after amount of time elapsed
  if(1 == *data_byte_index)
  {
    start_timer_16b();
  }

  // end of receiving of a single frame; move to next buffer frame slot
  if(is_frame_complete(*data_byte_index))
  {
    stop_timer_16b();

    frame_slot->ready  = true;
    *data_byte_index   = 0;
    *frame_slot_index += 1;
  }

  // jump to the first slot after the last (circularity)
  if(*frame_slot_index >= RX_BUFFER_SLOTS)
  {
    *frame_slot_index = 0;
  }

  return FRAME_RECEIVER_SUCCESS;
}

frame_receiver_return_t read_frame_from_rx_buffer(rx_buffer_t *rx_buffer, evaa_protocol_frame_t *frame_buffer)
{
  if(NULL == frame_buffer)
  {
    return FRAME_RECEIVER_WRONG_FRAME_BUFFER;
  }

  uint16_t *frame_slot_index = &rx_buffer->read_frame_index;
  frame_slot_t *frame_slot   = &rx_buffer->frame[*frame_slot_index];

  if(is_slot_free(frame_slot))
  {
    return FRAME_RECEIVER_NO_FRAME_TO_READ;
  }

  // copy frame to buffer
  memcpy(frame_buffer, &frame_slot->data, sizeof(evaa_protocol_frame_t));

  // set slot's ready flag to false and clean frame's CRC8;
  // protection against retrying of used frame parsing
  frame_slot->ready           = false;
  frame_slot->data.crc8       = 0;
  frame_slot->data_byte_index = 0;

  // increment frame_slot_index; set to 0 when higher than available number of slots
  *frame_slot_index += 1;
  if(*frame_slot_index >= RX_BUFFER_SLOTS)
  {
    *frame_slot_index = 0;
  }

  return FRAME_RECEIVER_SUCCESS;
}

#ifdef FRAME_RECEIVER_TEST
void print_rx_buffer(rx_buffer_t *pRx_buffer)
{
    char str_buf[20];

    for(int i = 0; i < RX_BUFFER_SLOTS; i++) {
        sprintf(str_buf, "Slot %d: ", i);
        usart_send_str((unsigned char*)str_buf);
        usart_send_data((unsigned char*)&pRx_buffer->frame[i].data,
                        sizeof(evaa_protocol_frame_t));
        if(pRx_buffer->frame[i].ready) {
            usart_send_str((unsigned char*)" ready = true\n\r");
        } else {
            usart_send_str((unsigned char*)" ready = false\n\r");
        }
    }
}
#endif // FRAME_RECEIVER_TEST

bool is_frame_ready(rx_buffer_t *rx_buffer)
{
  return rx_buffer->frame[rx_buffer->read_frame_index].ready;
}

static bool is_slot_free(frame_slot_t *frame)
{
  return false == frame->ready && is_slot_empty(frame);
}

static bool is_slot_empty(frame_slot_t *frame)
{
  return 0 == frame->data_byte_index;
}

static bool is_frame_complete(uint16_t data_byte_index)
{
  return (data_byte_index == sizeof(evaa_protocol_frame_t));
}
