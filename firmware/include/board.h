#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>


void board_init(void);   //*initialize board
uint32_t board_millis(void); //* milliseconds since start 

void board_set_ld(bool on);

void board_toggle_led(void);

#endif