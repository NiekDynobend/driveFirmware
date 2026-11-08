#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>


#define FRAME_SIZE 13
#define DATA_SIZE  8
/* ---------- message IDs (lower id = higher priority, like CAN) ---------- */
#define MSG_EMERGENCY   0x080u   /* drive -> PC */
#define MSG_STATUS      0x180u   /* drive -> PC */
#define MSG_COMMAND     0x200u   /* PC -> drive */
#define MSG_PING        0x700u   /* PC -> drive */
#define MSG_PONG        0x701u   /* drive -> PC, echoes the ping counter */

/* ---------- PING / PONG, len 1 ----------
   byte 0 : counter (PC increments, drive echoes it back in PONG) */
#define PING_LEN            1

/* ---------- COMMAND (PC -> drive), len 3 ----------
   byte 0-1 : controlword (little endian)
   byte 2   : counter                                      */
#define COMMAND_LEN         3
#define CW_DISABLE          0x0000u   /* 0 = safe */
#define CW_ENABLE           0x0001u

/* ---------- STATUS (drive -> PC), len 5 ----------
   byte 0-1 : statusword
   byte 2-3 : error_code
   byte 4   : counter_echo (last command counter received) */
#define STATUS_LEN          5
#define SW_DISABLED         0x0000u
#define SW_ENABLED          0x0001u
#define SW_FAULT            0x0008u


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