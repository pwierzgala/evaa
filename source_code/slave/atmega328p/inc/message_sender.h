#ifndef _HEADER_MESSAGE_SENDER_
#define _HEADER_MESSAGE_SENDER_

#include <stdint.h>
/*
      char unsigned bytes_to_send     = *((char unsigned *)arg_size);
      char unsigned amount_of_chunks  = get_chunk_count(bytes_to_send);
      for(int i = 0; i < amount_of_chunks; ++i)
      {
        uint32_t chunk = get_chunk_by_number(arg, i);
        evaa_protocol_frame_t response = create_response();
        usart_send_data((char unsigned *)&response, sizeof(evaa_protocol_frame_t));
      }
    }
*/
uint8_t get_chunk_count(uint32_t bytes_to_send);
uint32_t get_chunk_by_number(void *data, uint8_t chunk_number);

uint8_t send_ack_message(uint8_t data_type, void *data, uint32_t *size);
uint8_t send_response_message(uint8_t data_type, void *data, uint32_t *size);

#endif
