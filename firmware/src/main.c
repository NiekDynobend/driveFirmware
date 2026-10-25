#include "board.h"
#include "frame.h"


int main(void)
{
    // Define frame decoder
    FrameDecoder frame_decoder;
    CanFrame can_frame;
    uint8_t received_byte;
    bool decode_result = false;
    init_frame_decoder(&frame_decoder);
    // init board
    board_init();

    // timer
    uint32_t last = 0;
    const uint32_t update_time = 500;
    while(1)
    {
        uint32_t now = board_millis();
        if(now-last>update_time){
            board_toggle_led();
            last = now;
        }

        if(frameDecoderFeed(&frame_decoder, received_byte)){
            decode_result = decodeFrame(&frame_decoder.buffer, &can_frame);
        }
        if(decode_result){
            printf("Message succesfully received and decoded.");
            decode_result = false;
        }

        

    }
}