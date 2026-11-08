#include "board.h"
#include "frame.h"
#include "uart.h"
#include "commands.h"

int main(void)
{
    FrameDecoder decoder;
    CanFrame rx;
    uint8_t byte;

    board_init();
    uart_init();                      // was missing: USART1 is never set up
    init_frame_decoder(&decoder);

    while(1)
    {
        if(uart_read(&byte, 1)){                           // only when a byte arrived
            if(frameDecoderFeed(&decoder, byte) &&
               decodeFrame(decoder.buffer, &rx)){          // only when a full frame is in
                board_toggle_led();
                handle_command(&rx);                       // once per frame
            }
        }
    }
}