#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "protocol.h"

#define FRAME_SIZE 13

typedef enum {
    PARSER_WAIT_SOF,
    PARSER_ID_LO,
    PARSER_ID_HI,
    PARSER_LEN,
    PARSER_DATA,
    PARSER_CRC
}ParserState;

typedef struct {
    ParserState state;
    uint8_t buffer[FRAME_SIZE];
    uint8_t buffer_idx;
}FrameDecoder;

// puts the frame decoder in its starting state
// call before the rfirst frame DecoderFeed
void init_frame_decoder(FrameDecoder*);


//decodes a canframe into a sequence of 13 bytes ready to be send on the wire
bool encodeFrame(const CanFrame*, uint8_t*);

bool decodeFrame(const uint8_t*, CanFrame*);

bool frameDecoderFeed(FrameDecoder*, uint8_t);




#endif