#include <stdbool.h>
#include "protocol.h"

#ifndef UART_LINK_H
#define UART_LINK_H


bool send_frame(const CanFrame*);

#endif