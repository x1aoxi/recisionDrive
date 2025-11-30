#ifndef __UART_EXAMPLE_H
#define __UART_EXAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

#define UART_RX_BUF_SIZE 256

// 串口接收缓冲区
extern uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
extern uint16_t uart_rx_len;

// 函数声明
void UART_Example_Init(void);
void UART_Example_SendString(const char* str);
void UART_Example_Process(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* __UART_EXAMPLE_H */ 