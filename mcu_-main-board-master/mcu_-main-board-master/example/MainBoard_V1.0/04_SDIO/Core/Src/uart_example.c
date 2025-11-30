#include "uart_example.h"

// 定义接收缓冲区
uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
uint16_t uart_rx_len = 0;
uint8_t temp_buf[1]; // 用于DMA接收单个字符

// 初始化串口示例
void UART_Example_Init(void)
{
    printf("UART Example Initialize...\r\n");
    // 启动DMA接收
    HAL_UART_Receive_DMA(&huart1, temp_buf, 1);
}

// 发送字符串
void UART_Example_SendString(const char* str)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

// 串口数据处理
void UART_Example_Process(void)
{
    if(uart_rx_len > 0)
    {
        // 在这里处理接收到的数据
        printf("Received: %s\r\n", uart_rx_buf);
        
        // 如果收到"hello"，回复"world"
        if(strstr((char*)uart_rx_buf, "hello"))
        {
            UART_Example_SendString("world\r\n");
        }
        
        // 清空接收缓冲区
        memset(uart_rx_buf, 0, UART_RX_BUF_SIZE);
        uart_rx_len = 0;
    }
}

// UART接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        // 将接收到的数据存入缓冲区
        if(uart_rx_len < UART_RX_BUF_SIZE)
        {
            uart_rx_buf[uart_rx_len++] = temp_buf[0];
            
            // 如果收到回车换行，表示一帧数据接收完成
            if(temp_buf[0] == '\n' || temp_buf[0] == '\r')
            {
                uart_rx_buf[uart_rx_len] = '\0';  // 添加字符串结束符
            }
        }
        else
        {
            uart_rx_len = 0; // 缓冲区满，重新开始接收
        }
        
        // 继续接收下一个字节
        HAL_UART_Receive_DMA(&huart1, temp_buf, 1);
    }
} 