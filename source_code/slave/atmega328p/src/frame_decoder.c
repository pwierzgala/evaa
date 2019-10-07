#include "frame_decoder.h"

frame_decoder_return_t check_frame_integrity(evaa_protocol_frame_t frame)
{
    if(false == check_frame_crc(frame)) {
        return FRAME_DECODER_WRONG_CRC;
    }

    if(false == check_frame_device_id(frame)) {
        return FRAME_DECODER_WRONG_DEVICE_ID;
    }

    if(frame.command >= COMMAND_LAST) {
        return FRAME_DECODER_UNKNOWN_COMMAND;
    }

     if(frame.data_type >= DATA_TYPE_LAST) {
        return FRAME_DECODER_UNKNOWN_DATA_TYPE;
    }

    return FRAME_DECODER_SUCCESS;
}

bool check_frame_crc(evaa_protocol_frame_t frame)
{
    uint8_t crc8 = dallas_crc_8((uint8_t*)&frame, sizeof(evaa_protocol_frame_t) - sizeof(frame.crc8));

    if(frame.crc8 == crc8)
        return true;

    return false;
}

bool check_frame_device_id(evaa_protocol_frame_t frame)
{
    (void)frame; // unused at this moment
    return true; // dummy return
}

char *frame_result_to_str(frame_decoder_return_t result)
{
  switch(result)
  {
    case FRAME_DECODER_WRONG_CRC:
      return "Invalid crc";
    case FRAME_DECODER_WRONG_DEVICE_ID:
      return "Invalid device id";
    case FRAME_DECODER_UNKNOWN_COMMAND:
      return "Unknown command";
    case FRAME_DECODER_UNKNOWN_DATA_TYPE:
      return "Unknown data type";
    case FRAME_DECODER_SUCCESS:
      return "Success";
    default:
      return "Unknown error";
  }

  return "Very sad story :(";
}
