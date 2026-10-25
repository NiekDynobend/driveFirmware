
#include <stdbool.h>
#include <stdio.h>
#include "frame.h"
#include "protocol.h"



static const uint8_t start_byte = 0xAA;
static const uint8_t crc = 0xFF;
static const int data_len = 8;

bool encodeFrame(const CanFrame* frame, uint8_t* encoded_frame)
{
    if(frame==NULL){
        printf("Encoder received CanFrame==Null");
        return false;
    }
    if(encoded_frame==NULL){
        printf("encoder received encoded_frame==NULL");
        return false;
    }

    encoded_frame[0] = start_byte;
    encoded_frame[1] = (uint8_t)(frame->id);
    encoded_frame[2] = (uint8_t)((frame->id)>>8);
    encoded_frame[3] = frame->len;
    
    for(int idx=0; idx<data_len; idx++){
        encoded_frame[4+idx] = frame->data[idx];
    }
    encoded_frame[12] = crc;
    return true;
}

bool decodeFrame(const uint8_t* encoded_frame, CanFrame* frame)
{
    if(encoded_frame==NULL || frame==NULL){
        printf("Decoder: either encoded frame or canFrame is NULL.");
        return false;
    }
    if(encoded_frame[0]!=start_byte){
        printf("Decoder: First byte of encoded frame is not equal to start byte");
        return false;
    }
    if(encoded_frame[3]>data_len){
        printf("Decoder: data length is larger than max size of 8.");
        return false;
    }

    frame->id = (uint16_t)(encoded_frame[1] | (encoded_frame[2] << 8));
    frame->len = encoded_frame[3];
    for(int idx=0; idx<8; idx++){
        frame->data[idx] = encoded_frame[4+idx];
    }
    return true;
    
}

void init_frame_decoder(FrameDecoder* decoder)
{
    decoder->buffer_idx = 0;
    decoder->state = PARSER_WAIT_SOF;
}

bool frameDecoderFeed(FrameDecoder* frame_decoder, uint8_t byte)
{
    switch (frame_decoder->state)
    {
    case PARSER_WAIT_SOF:
        if(byte==start_byte){
            frame_decoder->buffer_idx=0;
            frame_decoder->buffer[frame_decoder->buffer_idx] = byte;
            frame_decoder->buffer_idx++;
            frame_decoder->state = PARSER_ID_LO;
            return false;
        }
        break;
    
    case PARSER_ID_LO:
            frame_decoder->buffer[frame_decoder->buffer_idx] = byte;
            frame_decoder->buffer_idx++;
            frame_decoder->state = PARSER_ID_HI;

        break;

    case PARSER_ID_HI:
            frame_decoder->state = PARSER_LEN;
            frame_decoder->buffer[frame_decoder->buffer_idx] = byte;
            frame_decoder->buffer_idx++;

        break;
    
    case PARSER_LEN:
            frame_decoder->state = PARSER_DATA;
            frame_decoder->buffer[frame_decoder->buffer_idx] = byte;
            frame_decoder->buffer_idx++;

        break;
    
    case PARSER_DATA:

            frame_decoder->state = PARSER_CRC;
            frame_decoder->buffer[frame_decoder->buffer_idx] = byte;
            frame_decoder->buffer_idx++;

        break;
    
    case PARSER_CRC:
            frame_decoder->state = PARSER_WAIT_SOF;
            frame_decoder->buffer[frame_decoder->buffer_idx] = byte;
            return true;
        break;

    default:
        return false;
        break;
    }
    return false;
}