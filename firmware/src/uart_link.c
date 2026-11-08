#include "uart_link.h"


bool send_frame(const CanFrame* f)
{
    uint8_t out[FRAME_SIZE];
    if(!encodeFrame(f, out)){
        return false;
    }
    uart_write(out, FRAME_SIZE);
    return true;
}