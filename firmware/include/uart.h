#include <stdint.h>

#ifndef UART_H
#define UART_H




void uart_init();

void uart_write();


// Checks if bytes came in on rx and returns how many, 0 means nothing arrived
size_t uart_read(uint8_t* , size_t);

#endif