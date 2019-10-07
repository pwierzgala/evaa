#ifndef _HEADER_FRAME_RECEIVER_
#define _HEADER_FRAME_RECEIVER_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef FRAME_RECEIVER_TEST
#include <stdio.h>
#include "usart.h"
#endif // FRAME_RECEIVER_TEST

#define ANYSIZE                        1
#define RX_BUFFER_SLOTS                5

typedef enum
{
    FRAME_RECEIVER_SUCCESS            =  0,
    FRAME_RECEIVER_TIMEOUT            = -1,
    FRAME_RECEIVER_RX_BUFFER_FULL     = -2,
    FRAME_RECEIVER_NO_FRAME_TO_READ   = -3,
    FRAME_RECEIVER_WRONG_FRAME_BUFFER = -4
} frame_receiver_return_t;

typedef union
{
    struct
    {
        uint8_t  device_id;
        uint8_t  sensor_id;                    // a.k.a plant_id
        uint8_t  number_of_chunks;
        uint8_t  chunk_index;
        uint8_t  command;
        uint8_t  data_type;
        uint32_t data;
        uint8_t  crc8;
    } __attribute__((packed, aligned(1)));

    uint8_t single_byte[ANYSIZE];
} evaa_protocol_frame_t;

typedef struct
{
    evaa_protocol_frame_t data;
    uint16_t data_byte_index;
    bool ready;
} frame_slot_t;

typedef struct
{
    frame_slot_t frame[RX_BUFFER_SLOTS];
    uint16_t write_frame_index;
    uint16_t read_frame_index;
} rx_buffer_t;


rx_buffer_t *get_buffer();

frame_receiver_return_t init_rx_buffer(rx_buffer_t *pRx_buffer);

frame_receiver_return_t write_byte_to_rx_buffer(rx_buffer_t *pRx_buffer,
                                                uint8_t data_byte);

frame_receiver_return_t read_frame_from_rx_buffer(rx_buffer_t *pRx_buffer,
                                                  evaa_protocol_frame_t *pFrame_buffer);

bool is_frame_ready(rx_buffer_t *pRx_buffer);

#ifdef FRAME_RECEIVER_TEST
void print_rx_buffer(rx_buffer_t *pRx_buffer);
#endif // FRAME_RECEIVER_TEST

#endif // _HEADER_FRAME_RECEIVER_
