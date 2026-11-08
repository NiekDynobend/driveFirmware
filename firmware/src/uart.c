#include <stdint.h>
#include <stdbool.h>
#include "uart.h"
#include "stm32f1xx_hal.h"

static UART_HandleTypeDef huart1;
// huart1                     (UART_HandleTypeDef, your static in uart.c)
//  ├─ Init { BaudRate, ... }  settings
//  ├─ Instance ──────────────► USART1 register block (USART_TypeDef, at a fixed address)
//  │                            ├─ SR    status register (RXNE, ORE, TXE, ...)
//  │                            ├─ DR    data register ◄── the received byte
//  │                            ├─ BRR   baud rate
//  │                            └─ CR1, CR2, CR3   control registers
//  └─ ...                     HAL state

void uart_init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK){
        return;
    }

}

//  idle   start  b0  b1  b2  b3  b4  b5  b6  b7  stop   idle
//  ‾‾‾‾‾‾‾|___|‾‾‾|___|‾‾‾|___|‾‾‾|___|‾‾‾|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
//   1       0    0   1   0   1   0   1   0   1    1      1
//         |←────────────── ≈ 87 µs at 115200 ─────────→|
//         each bit ≈ 8.7 µs

size_t uart_read(uint8_t* buff, size_t max){
    if(buff==NULL){
        return 0;
    }
    if(max==0){
        return 0;
    }
    size_t result = 0;
    uint8_t received_data;

    bool rxne_status = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE);

    if(rxne_status){
        received_data = (uint8_t)huart1.Instance->DR;
        buff[0] = received_data;

        result = 1;
    }else{
        result = 0;
    }
    return result;
}


void uart_write(const uint8_t* data, size_t len)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)data, (uint16_t)len, 10);  // 10 ms timeout
}


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance == USART1){
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitTypeDef g = {0};
        g.Pin   = GPIO_PIN_9;               // TX
        g.Mode  = GPIO_MODE_AF_PP;
        g.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &g);

        g.Pin  = GPIO_PIN_10;               // RX
        g.Mode = GPIO_MODE_INPUT;
        g.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &g);
    }
}