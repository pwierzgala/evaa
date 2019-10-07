#ifndef _HEADER_FRAME_DECODER_
#define _HEADER_FRAME_DECODER_

#include <stdbool.h>
#include "frame_receiver.h"
#include "crc8.h"

typedef enum
{
    FRAME_DECODER_SUCCESS           =  0,
    FRAME_DECODER_UNKNOWN_COMMAND   = -1,
    FRAME_DECODER_UNKNOWN_DATA_TYPE = -2,
    FRAME_DECODER_WRONG_CRC         = -3,
    FRAME_DECODER_WRONG_DEVICE_ID   = -4
} frame_decoder_return_t;

typedef enum
{
    ACK                             = 0x00,
    GET                             = 0x01,
    SET                             = 0x02,
    RESPONSE                        = 0x03,
    COMMAND_LAST
} frame_command_t;

typedef enum
{
    ERROR                           = 0x00,
    TEMPERATURE_1_WIRE_SENSOR       = 0x01,
    LIGHT_INTENSITY_SENSOR          = 0x02,
    HUMIDITY_SENSOR                 = 0x03,
    POMP_MOTOR_DATA                 = 0x04,
    ELEVATOR_MOTOR_DATA             = 0x05,
    PRESSURE_SENSOR                 = 0x06,
    LED_POWER                       = 0x07,
    WATER_LEVEL_SENSOR              = 0x08,
    BLUE_PLANT_LIGHT                = 0x09,
    RED_PLANT_LIGHT                 = 0x0A,
    PH_SENSOR                       = 0x0B,
    WEIGHT_SENSOR                   = 0x0C,
    VENTILATION_LEVEL               = 0x0D,
    DATA_TYPE_LAST

} frame_data_type_t;

/*
 * Function checks CRC and Device ID
 */
frame_decoder_return_t check_frame_integrity(evaa_protocol_frame_t frame);

/*
 * Function checks CRC correctness
 */
bool check_frame_crc(evaa_protocol_frame_t frame);

/*
 * Function checks that Device ID in frame agrees with Device ID of slave device
 */
bool check_frame_device_id(evaa_protocol_frame_t frame);


char *frame_result_to_str(frame_decoder_return_t result);

#endif // _HEADER_FRAME_DECODER_
