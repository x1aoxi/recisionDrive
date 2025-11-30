#ifndef __UART_APP_H__
#define __UART_APP_H__

#include "MyDefine.h"

#define DEBUG_UART &huart1

void Uart_Init(void);
void Uart1_Task(void);
void Uart5_Task(void);
void Uart6_Task(void);

#endif
