#include "uart.h"
#include "protocol.h"
#include "board.h"

#ifndef COMMAND_H
#define COMMAND_H

static void pong(uint8_t);

void handle_command(const CanFrame*);


#endif