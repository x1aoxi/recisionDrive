#include "uart_app.h"

/* 串口 1 */
extern uint8_t uart1_rx_dma_buffer[BUFFER_SIZE]; // DMA 读取缓冲区

extern uint8_t uart1_ring_buffer_input[BUFFER_SIZE]; // 环形缓冲区对应的线性数组
extern struct rt_ringbuffer uart1_ring_buffer; // 环形缓冲区

extern uint8_t uart1_data_buffer[BUFFER_SIZE]; // 数据处理缓冲区

/* 串口 5 */
extern uint8_t uart5_rx_dma_buffer[BUFFER_SIZE]; // DMA 读取缓冲区

extern uint8_t uart5_ring_buffer_input[BUFFER_SIZE]; // 环形缓冲区对应的线性数组
extern struct rt_ringbuffer uart5_ring_buffer; // 环形缓冲区

extern uint8_t uart5_data_buffer[BUFFER_SIZE]; // 数据处理缓冲区

/* 串口 6 */
extern uint8_t uart6_rx_dma_buffer[BUFFER_SIZE]; // DMA 读取缓冲区

extern uint8_t uart6_ring_buffer_input[BUFFER_SIZE]; // 环形缓冲区对应的线性数组
extern struct rt_ringbuffer uart6_ring_buffer; // 环形缓冲区

extern uint8_t uart6_data_buffer[BUFFER_SIZE]; // 数据处理缓冲区

void Uart_Init(void)
{
  Uart_Printf(DEBUG_UART, "Uart_Init ......\r\n");
  
  /* 串口 1 */
  rt_ringbuffer_init(&uart1_ring_buffer, uart1_ring_buffer_input, BUFFER_SIZE);
  
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_dma_buffer, BUFFER_SIZE); // 启动读取中断
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // 关闭 DMA 的"半满中断"功能
  
  /* 串口 5 */
  rt_ringbuffer_init(&uart5_ring_buffer, uart5_ring_buffer_input, BUFFER_SIZE);
  
  HAL_UARTEx_ReceiveToIdle_DMA(&huart5, uart5_rx_dma_buffer, BUFFER_SIZE); // 启动读取中断
  __HAL_DMA_DISABLE_IT(&hdma_uart5_rx, DMA_IT_HT); // 关闭 DMA 的"半满中断"功能
  
  /* 串口 6 */
  rt_ringbuffer_init(&uart6_ring_buffer, uart6_ring_buffer_input, BUFFER_SIZE);
  
  HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uart6_rx_dma_buffer, BUFFER_SIZE); // 启动读取中断
  __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT); // 关闭 DMA 的"半满中断"功能
  
}

/* 串口 1 */
void Uart1_Task(void)
{
  uint16_t uart_data_len = rt_ringbuffer_data_len(&uart1_ring_buffer);
  if(uart_data_len > 0)
  {
    rt_ringbuffer_get(&uart1_ring_buffer, uart1_data_buffer, uart_data_len);
    uart1_data_buffer[uart_data_len] = '\0';
    /* 数据解析 */
    Uart_Printf(DEBUG_UART, "UART1 Ringbuffer:%s\r\n", uart1_data_buffer);
    
    memset(uart1_data_buffer, 0, uart_data_len);
  }
}

/* 串口 5 */
void Uart5_Task(void)
{
  uint16_t uart_data_len = rt_ringbuffer_data_len(&uart5_ring_buffer);
  if(uart_data_len > 0)
  {
    rt_ringbuffer_get(&uart5_ring_buffer, uart5_data_buffer, uart_data_len);
    uart5_data_buffer[uart_data_len] = '\0';
    /* 数据解析 */
//    Uart_Printf(DEBUG_UART, "UART5 Ringbuffer:%s\r\n", uart5_data_buffer);

    int left, right;
    if(sscanf((char*)uart5_data_buffer, "(%d,%d)\r\n)", &left, &right) == 2)
    {
//      Uart_Printf(DEBUG_UART, "%s\r\n", uart5_data_buffer);
    }
    
    memset(uart5_data_buffer, 0, uart_data_len);
  }
}

/* 串口 6 */
void Uart6_Task(void)
{
  uint16_t uart_data_len = rt_ringbuffer_data_len(&uart6_ring_buffer);
  if(uart_data_len > 0)
  {
    rt_ringbuffer_get(&uart6_ring_buffer, uart6_data_buffer, uart_data_len);
    uart6_data_buffer[uart_data_len] = '\0';
    /* 数据解析 */
    Uart_Printf(DEBUG_UART, "UART6 Ringbuffer:%s\r\n", uart6_data_buffer);
    
    memset(uart6_data_buffer, 0, uart_data_len);
  }
}

