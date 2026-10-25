#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct 
{
    uint16_t id;
    uint8_t len;
    uint8_t data[8];
} CanFrame;

typedef struct 
{
    uint16_t controlword;
    uint8_t counter;
    uint16_t statusword;
    uint16_t error_code;
    uint8_t counter_echo;
} Command;




#endif 