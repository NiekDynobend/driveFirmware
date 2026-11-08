#include "commands.h"
#include "protocol.h"
#include "uart_link.h"


static void pong(uint8_t counter)
{
    CanFrame tx = { .id = MSG_PONG, .len = PING_LEN };
    tx.data[0] = counter;            // echo the ping counter
    send_frame(&tx);
}


void handle_command(const CanFrame* rx)
{
    switch(rx->id){
    case MSG_PING:
        pong(rx->data[0]);
        board_toggle_led();
        break;
    case MSG_COMMAND:
        // ...
        break;
    default:
        break;
    }
}