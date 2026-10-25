#include "board.h"
#include "stm32f1xx_hal.h"


//-------------------- Purpose of board_init()
// when the stm is started up, it runs in default mode. e.g. the following things are in default"
// internal frequency : 8MHz
//  perhiphals are switched off: a perhiphal is a piece of hardware on the board next to the processor,
// gpio,: pins high/los, usart: send and receive serial bytes, timers: for gernating pwm signals ans reading encoder pulses
// adc measure anlog voltage, spi/i2c. So with the init the selected pehiphals can be enabled
// perhipheral: traces back to ancient Greek Peripheria meaning "circumference or outher surface" pperi "around , pherein: carry
// literally "a carrying around"
//* all pins are inputs at start up, they drive nothing

/*
 * Hardware hierarchy
 *
 * BOARD (Blue Pill PCB)
 * |-- external components: 8 MHz crystal, voltage regulator, LEDs,
 * |                        buttons, headers, USB
 * `-- CHIP (microcontroller STM32F103C8T6)
 *     |-- CORE (ARM Cortex-M3)            <- designed by ARM
 *     |   |-- CPU         : executes instructions
 *     |   |-- NVIC        : interrupt controller
 *     |   |-- SysTick     : timer for the 1 ms tick
 *     |   `-- debug unit  : what the Pico talks to over SWD
 *     |-- MEMORY
 *     |   |-- flash (64/128 KB) : program
 *     |   `-- SRAM  (20 KB)     : variables
 *     |-- CLOCK SYSTEM (RCC)
 *     |   |-- HSI (internal 8 MHz), input for external crystal (HSE)
 *     |   `-- PLL + dividers -> SYSCLK and bus clocks
 *     |-- BUSES: AHB, APB1, APB2          <- internal wiring
 *     `-- PERIPHERALS                     <- designed by ST
 *         |-- GPIO ports A, B, C
 *         |-- USART1-3, SPI, I2C, CAN, USB
 *         |-- timers (TIM1-4), ADC
 *         `-- DMA, RTC, watchdog
 */

static bool clock_hse = false; /* true once crystal + pll are running  hse: high speed external*/

static void clock_init(void);
static void gpio_init(void);

void board_init(void)
{
    //prepare HAL 
    // HAL: Hardware Abstration Layer
    // HAL is a C library from ST, which gives functions for writing to hardware registers
    //e.g without HAL youd write set it 12 in register GPIOB->BSRR. With the AHL you write HAL_GPIO_WritePin(GPIOB, GPIO_PIN_, GPIO_PIN_SET)
    // CMSIS Common MicroController Software Interfae Standard: its a standard for ARM processor core 
    //   ┌──────────────────────────────────────────────────────┐
    //│ Your code (board.c, uart_link.c, drive_sm.c, ...)    │
    //├──────────────────────────────────────────────────────┤
    //│ HAL  ← readable functions per peripheral              │
    //│ (LL  ← "Low Layer": thinner, faster alternative)     │
    //├──────────────────────────────────────────────────────┤
    //│ CMSIS ← register names (GPIOB->ODR, USART1->DR),      │
    //│         ARM core functions (interrupts, NVIC)        │
    //├──────────────────────────────────────────────────────┤
    //│ Hardware: registers in the chip 

    HAL_Init(); //starts the HAL
    clock_init();// sets frequency of the chip(not board)
    gpio_init(); // set pins in safestate

}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

uint32_t board_millis(void)
{
    return HAL_GetTick();
}

void board_set_led(bool on)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void board_toggle_led(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
}

/*-------------Private Helper ----------------------------*/
void clock_init()
{
    RCC_OscInitTypeDef osc = {0};
    osc.PLL.PLLState = RCC_PLL_ON;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK){
        return;
    }
    clock_hse = true;
}

void gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //what is push pull?
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; //low frequency sped
    
    //initialize pin with settings
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}