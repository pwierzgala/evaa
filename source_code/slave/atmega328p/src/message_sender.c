#include "message_sender.h"
#include "frame_decoder.h"
#include "usart.h"

#define DATA_BYTES_IN_FRAME 4

uint8_t get_chunk_count(uint32_t bytes_to_send)
{
  return bytes_to_send / DATA_BYTES_IN_FRAME + (bytes_to_send % 4 > 0);
}

uint8_t get_alignment_count(uint32_t bytes_to_send)
{
  return (bytes_to_send > 4) * bytes_to_send % 4; // 0 - 3
}

uint8_t get_offset(uint8_t chunk_number, uint8_t alignment)
{
  // 4 size of data field
  return 4 * chunk_number - alignment;
}

uint32_t remove_unnecessary_bytes(uint32_t *data, uint8_t alignment)
{
  // 8 bit in byte
  // rotate right because avr is little endian
  return *data >> 8 * alignment;
}

uint32_t get_aligned_chunk_by_number(void *data, uint8_t chunk_number, uint8_t alignment)
{
  uint8_t *aligned_data = ((uint8_t *)data + get_offset(chunk_number, alignment));
  uint32_t chunk = *(uint32_t *)aligned_data;
  chunk = remove_unnecessary_bytes(&chunk, alignment);
  return chunk;
}

uint8_t send_response_frame(frame_command_t command, uint8_t data_type, void *data, uint32_t *size)
{
  uint32_t bytes_to_send     = *size;
  uint8_t amount_of_chunks  = get_chunk_count(bytes_to_send);
  uint8_t alignment         = get_alignment_count(bytes_to_send);

  uint8_t i;
  for(i = 0; i < amount_of_chunks; ++i)
  {
    uint8_t bytes_to_align = (i == (amount_of_chunks - 1)) * alignment;
    uint32_t chunk_data = get_aligned_chunk_by_number(data, i, bytes_to_align);
    evaa_protocol_frame_t response = create_response(command, amount_of_chunks, i, data_type, chunk_data);
    usart_send_data((uint8_t *)&response, sizeof(evaa_protocol_frame_t));
  }

  return i;  //number of chunks sended
}

uint8_t send_ack_message(uint8_t data_type, void *data, uint32_t *size)
{
  return send_response_frame(ACK, data_type, data, size);
}

uint8_t send_response_message(uint8_t data_type, void *data, uint32_t *size)
{
  return send_response_frame(RESPONSE, data_type, data, size);
}

