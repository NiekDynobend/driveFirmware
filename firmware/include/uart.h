#include <stdint.h>
#include <stdint.h>
#include <stddef.h>    

#ifndef UART_H
#define UART_H


void uart_init(void);
void uart_write(const uint8_t* data, size_t len);
size_t uart_read(uint8_t* buff, size_t max);   // returns number of bytes read, 0 = nothing

#endif