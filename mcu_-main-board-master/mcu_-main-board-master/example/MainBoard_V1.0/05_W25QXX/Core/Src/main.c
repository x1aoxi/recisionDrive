/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author         : MCU Electronics Technology
  * @version        : V1.0
  * @date           : 2024-01-01
  * @note           : W25QXX Flash Test Demo
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#include "driver_w25qxx.h"
#include "driver_w25qxx_interface.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEST_BUFFER_SIZE     256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t bsp_uart0_rx_buff[USART_BUFFER_SIZE];

static w25qxx_handle_t gs_handle;            /**< w25qxx handle */

static uint8_t gs_test_buffer[TEST_BUFFER_SIZE];  // 测试缓冲区
static uint8_t gs_buffer_input[600];         /**< input buffer */
static uint8_t gs_buffer_output[600];        /**< output buffer */
static const uint32_t gsc_size[] = {0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000};        /**< flash size */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/**
 * @brief     basic example init
 * @param[in] type is the chip type
 * @param[in] interface is the chip interface
 * @param[in] dual_quad_spi_enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t w25qxx_basic_init(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_W25QXX_LINK_INIT(&gs_handle, w25qxx_handle_t);
    DRIVER_W25QXX_LINK_SPI_QSPI_INIT(&gs_handle, w25qxx_interface_spi_qspi_init);
    DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(&gs_handle, w25qxx_interface_spi_qspi_deinit);
    DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(&gs_handle, w25qxx_interface_spi_qspi_write_read);
    DRIVER_W25QXX_LINK_DELAY_MS(&gs_handle, w25qxx_interface_delay_ms);
    DRIVER_W25QXX_LINK_DELAY_US(&gs_handle, w25qxx_interface_delay_us);
    DRIVER_W25QXX_LINK_DEBUG_PRINT(&gs_handle, w25qxx_interface_debug_print);
    
    /* set chip type */
    res = w25qxx_set_type(&gs_handle, type);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    
    /* set chip interface */
    res = w25qxx_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
       
        return 1;
    }
    
    /* set dual quad spi */
    res = w25qxx_set_dual_quad_spi(&gs_handle, dual_quad_spi_enable);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set dual quad spi failed.\n");
        (void)w25qxx_deinit(&gs_handle);
       
        return 1;
    }
    
    /* chip init */
    res = w25qxx_init(&gs_handle);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: init failed.\n");
       
        return 1;
    }
    else
    {
        if (type >= W25Q256)
        {
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
        }
        
        return 0;
    }
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t w25qxx_basic_deinit(void)
{
    if (w25qxx_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t w25qxx_basic_power_down(void)
{
    if (w25qxx_power_down(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t w25qxx_basic_wake_up(void)
{
    if (w25qxx_release_power_down(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example chip erase
 * @return status code
 *         - 0 success
 *         - 1 chip erase failed
 * @note   none
 */
uint8_t w25qxx_basic_chip_erase(void)
{
    if (w25qxx_chip_erase(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example get the manufacturer && device id information
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id failed
 * @note       none
 */
uint8_t w25qxx_basic_get_id(uint8_t *manufacturer, uint8_t *device_id)
{
    if (w25qxx_get_manufacturer_device_id(&gs_handle, manufacturer, device_id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     basic example write
 * @param[in] addr is the write address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t w25qxx_basic_write(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_write(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example read
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_basic_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_read(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//    if(huart->Instance == USART1)
//    {
//        // Add string terminator to the end of received data
//        bsp_uart0_rx_buff[Size] = '\0';
//        
//        // Echo received data with proper format
//        printf("[UART] Recv: %s", bsp_uart0_rx_buff);
//        
//        // If the received data doesn't end with \r\n, add it
//        if (Size < 2 || bsp_uart0_rx_buff[Size-2] != '\r' || bsp_uart0_rx_buff[Size-1] != '\n') {
//            printf("\r\n");
//        }
//        
//        // Restart DMA reception
//        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, bsp_uart0_rx_buff, USART_BUFFER_SIZE);
//        __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
//    }
//}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

// Redirect printf to UART
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/**
 * @brief  W25QXX测试程序
 * @return 测试结果
 *         - 0 成功
 *         - 1 失败
 */
static uint8_t w25qxx_test(void)
{
    uint8_t res;
    uint8_t manufacturer, device_id;
    uint16_t i;
    
    /* 初始化W25QXX */
    res = w25qxx_basic_init(W25Q32, W25QXX_INTERFACE_SPI, W25QXX_BOOL_FALSE);
    if (res != 0)
    {
        printf("W25QXX init failed!\r\n");
        return 1;
    }
    printf("W25QXX init success!\r\n");
    
    /* 获取制造商ID和设备ID */
    res = w25qxx_basic_get_id(&manufacturer, &device_id);
    if (res != 0)
    {
        printf("W25QXX get id failed!\r\n");
        return 1;
    }
    printf("W25QXX manufacturer is 0x%02X, device id is 0x%02X\r\n", 
           manufacturer, device_id);
    
    /* 准备测试数据 */
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        gs_test_buffer[i] = i;
    }
    
    /* 擦除芯片 */
    printf("W25QXX erase chip...\r\n");
    res = w25qxx_basic_chip_erase();
    if (res != 0)
    {
        printf("W25QXX chip erase failed!\r\n");
        return 1;
    }
    printf("W25QXX chip erase success!\r\n");
    
    /* 写入测试数据 */
    printf("W25QXX write test data...\r\n");
    res = w25qxx_basic_write(0x000000, gs_test_buffer, TEST_BUFFER_SIZE);
    if (res != 0)
    {
        printf("W25QXX write failed!\r\n");
        return 1;
    }
    printf("W25QXX write success!\r\n");
    
    /* 清空测试缓冲区 */
    memset(gs_test_buffer, 0, TEST_BUFFER_SIZE);
    
    /* 读取测试数据 */
    printf("W25QXX read test data...\r\n");
    res = w25qxx_basic_read(0x000000, gs_test_buffer, TEST_BUFFER_SIZE);
    if (res != 0)
    {
        printf("W25QXX read failed!\r\n");
        return 1;
    }
    
    /* 验证数据 */
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        if (gs_test_buffer[i] != i)
        {
            printf("W25QXX verify failed at %d!\r\n", i);
            return 1;
        }
    }
    printf("W25QXX read and verify success!\r\n");
    
    /* 进入掉电模式 */
    res = w25qxx_basic_power_down();
    if (res != 0)
    {
        printf("W25QXX power down failed!\r\n");
        return 1;
    }
    printf("W25QXX power down success!\r\n");
    
    /* 延时1秒 */
    HAL_Delay(1000);
    
    /* 唤醒 */
    res = w25qxx_basic_wake_up();
    if (res != 0)
    {
        printf("W25QXX wake up failed!\r\n");
        return 1;
    }
    printf("W25QXX wake up success!\r\n");
    
    return 0;
}


/**
 * @brief     read test
 * @param[in] type is the chip type
 * @param[in] interface is the chip interface
 * @param[in] dual_quad_spi_enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t w25qxx_read_test(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable)
{
    uint8_t res;
    w25qxx_info_t info;
    
    /* link interface function */
    DRIVER_W25QXX_LINK_INIT(&gs_handle, w25qxx_handle_t);
    DRIVER_W25QXX_LINK_SPI_QSPI_INIT(&gs_handle, w25qxx_interface_spi_qspi_init);
    DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(&gs_handle, w25qxx_interface_spi_qspi_deinit);
    DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(&gs_handle, w25qxx_interface_spi_qspi_write_read);
    DRIVER_W25QXX_LINK_DELAY_MS(&gs_handle, w25qxx_interface_delay_ms);
    DRIVER_W25QXX_LINK_DELAY_US(&gs_handle, w25qxx_interface_delay_us);
    DRIVER_W25QXX_LINK_DEBUG_PRINT(&gs_handle, w25qxx_interface_debug_print);
    
    /* get information */
    res = w25qxx_info(&info);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        w25qxx_interface_debug_print("w25qxx: chip is %s.\n", info.chip_name);
        w25qxx_interface_debug_print("w25qxx: manufacturer is %s.\n", info.manufacturer_name);
        w25qxx_interface_debug_print("w25qxx: interface is %s.\n", info.interface);
        w25qxx_interface_debug_print("w25qxx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        w25qxx_interface_debug_print("w25qxx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        w25qxx_interface_debug_print("w25qxx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        w25qxx_interface_debug_print("w25qxx: max current is %0.2fmA.\n", info.max_current_ma);
        w25qxx_interface_debug_print("w25qxx: max temperature is %0.1fC.\n", info.temperature_max);
        w25qxx_interface_debug_print("w25qxx: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* set chip type */
    res = w25qxx_set_type(&gs_handle, type);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    
    /* set chip interface */
    res = w25qxx_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
       
        return 1;
    }
    
    /* set dual quad spi */
    res = w25qxx_set_dual_quad_spi(&gs_handle, dual_quad_spi_enable);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set dual quad spi failed.\n");
        (void)w25qxx_deinit(&gs_handle);
       
        return 1;
    }
    
    /* chip init */
    res = w25qxx_init(&gs_handle);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: init failed.\n");
       
        return 1;
    }
    
    /* start read test */
    w25qxx_interface_debug_print("w25qxx: start read test.\n");
    
    if (interface == W25QXX_INTERFACE_SPI)
    {
        uint32_t size;
        uint32_t addr, step, j;
        
        size = gsc_size[type - W25Q80];
        step = size / 16;
        
        /* w25qxx_write/w25qxx_read test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* w25qxx_sector_erase_4k test */
        addr = (rand() % 10) * 4 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
        res = w25qxx_sector_erase_4k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_only_spi_read */
        res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_32k test */
        addr = (rand() % 10) * 32 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_32k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_only_spi_read */
        res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_64k test */
        addr = (rand() % 10) * 64 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_64k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_only_spi_read */
        res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* get sfdp */
        w25qxx_interface_debug_print("w25qxx: get sfdp.\n");
        memset(gs_buffer_output, 0, sizeof(uint8_t) * 256);
        res = w25qxx_get_sfdp(&gs_handle, (uint8_t *)gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get sfdp failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j += 8)
        {
            w25qxx_interface_debug_print("w25qxx: sdfp[%d-%d] is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                         j , j + 7,
                                         gs_buffer_output[j + 0], gs_buffer_output[j + 1], gs_buffer_output[j + 2],
                                         gs_buffer_output[j + 3], gs_buffer_output[j + 4], gs_buffer_output[j + 5],
                                         gs_buffer_output[j + 6], gs_buffer_output[j + 7]);
        }
        
        /*  security register1 write and read test */
        w25qxx_interface_debug_print("w25qxx: security register1 write and read test.\n");
        
        /* security register1 write and read test */
        res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_input);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: security register1 check passed.\n");
        
        /*  security register2 write and read test */
        w25qxx_interface_debug_print("w25qxx: security register2 write and read test.\n");
        
        /* security register2 write and read test */
        res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_input);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: security register2 check passed.\n");

        /*  security register3 write and read test */
        w25qxx_interface_debug_print("w25qxx: security register3 write and read test.\n");
        
        /* security register3 write and read test */
        res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_input);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: security register3 check passed.\n");
        
        #if (W25QXX_ENABLE_ERASE_READ_TEST == 1)
            /* start chip erasing */
            w25qxx_interface_debug_print("w25qxx: start chip erasing.\n");
            
            /* chip erase */
            w25qxx_interface_debug_print("w25qxx: w25qxx_chip_erase test.\n");
            res = w25qxx_chip_erase(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: chip erase failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        #endif
        
        if (type >= W25Q256)
        {
            /* set address mode 4 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 4 byte.\n");
            
            /* set address mode 4 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_write/w25qxx_read test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
            
            for (addr = 0; addr < size; addr += step)
            {
                for (j = 0; j < 600; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: write failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: read failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 600; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: %d/%d successful.\n", addr, size);
            }
            
            /* w25qxx_sector_erase_4k test */
            addr = (rand() % 10) * 4 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
            res = w25qxx_sector_erase_4k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_only_spi_read */
            res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_32k test */
            addr = (rand() % 10) * 32 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_32k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_only_spi_read */
            res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_64k test */
            addr = (rand() % 10) * 64 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_64k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_only_spi_read */
            res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* get sfdp */
            w25qxx_interface_debug_print("w25qxx: get sfdp.\n");
            memset(gs_buffer_output, 0, sizeof(uint8_t) * 256);
            res = w25qxx_get_sfdp(&gs_handle, (uint8_t *)gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: get sfdp failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j += 8)
            {
                w25qxx_interface_debug_print("w25qxx: sdfp[%d-%d] is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                             j , j + 7,
                                             gs_buffer_output[j + 0], gs_buffer_output[j + 1], gs_buffer_output[j + 2],
                                             gs_buffer_output[j + 3], gs_buffer_output[j + 4], gs_buffer_output[j + 5],
                                             gs_buffer_output[j + 6], gs_buffer_output[j + 7]);
            }
            
            /*  security register1 write and read test */
            w25qxx_interface_debug_print("w25qxx: security register1 write and read test.\n");
            
            /* security register1 write and read test */
            res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_input);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: security register1 check passed.\n");
            
            /*  security register2 write and read test */
            w25qxx_interface_debug_print("w25qxx: security register2 write and read test.\n");
            
            /* security register2 write and read test */
            res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_input);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: security register2 check passed.\n");

            /*  security register3 write and read test */
            w25qxx_interface_debug_print("w25qxx: security register3 write and read test.\n");
            
            /* security register3 write and read test */
            res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_input);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: security register3 check passed.\n");
            
            /* set address mode 3 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 3 byte.\n");
            
            /* set address mode 3 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_3_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
        }
    }
    else
    {
        uint32_t size;
        uint32_t addr, step, j;
        
        size = gsc_size[type - W25Q80];
        step = size / 16;
        
        /* w25qxx_write/w25qxx_read test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* w25qxx_sector_erase_4k test */
        addr = (rand() % 10) * 4 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
        res = w25qxx_sector_erase_4k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_32k test */
        addr = (rand() % 10) * 32 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_32k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_64k test */
        addr = (rand() % 10) * 64 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_64k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        #if (W25QXX_ENABLE_ERASE_READ_TEST == 1)
            /* start chip erasing */
            w25qxx_interface_debug_print("w25qxx: start chip erasing.\n");
            
            /* chip erase */
            w25qxx_interface_debug_print("w25qxx: w25qxx_chip_erase test.\n");
            res = w25qxx_chip_erase(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: chip erase failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        #endif
        
        if (type >= W25Q256)
        {
            /* set address mode 4 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 4 byte.\n");
            
            /* set address mode 4 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_write/w25qxx_read test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
            
            for (addr = 0; addr < size; addr += step)
            {
                for (j = 0; j < 600; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: write failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: read failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 600; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
            }
            
            /* w25qxx_sector_erase_4k test */
            addr = (rand() % 10) * 4 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
            res = w25qxx_sector_erase_4k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_32k test */
            addr = (rand() % 10) * 32 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_32k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_64k test */
            addr = (rand() % 10) * 64 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_64k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* set address mode 3 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 3 byte.\n");
            
            /* set address mode 3 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_3_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
        }
        
        if (dual_quad_spi_enable != 0)
        {
            /* enter to spi mode */
            w25qxx_interface_debug_print("w25qxx: enter to spi mode.\n");
            
            /* exit qspi */
            res = w25qxx_exit_qspi_mode(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: exit qspi mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* set chip interface spi */
            res = w25qxx_set_interface(&gs_handle, W25QXX_INTERFACE_SPI);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
               
                return 1;
            }
            
            /* random data */
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_sector_erase_4k */
            res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, 0x00000000, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* fast read dual output */
            res = w25qxx_fast_read_dual_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read dual output failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast_read_dual_output check passed.\n");
            
            /* fast read quad output */
            res = w25qxx_fast_read_quad_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read quad output failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast_read_quad_output check passed.\n");
            
            /* fast read dual io */
            res = w25qxx_fast_read_dual_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read dual io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_dual_io check passed.\n");
            
            /* fast read quad io */
            res = w25qxx_fast_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_quad_io check passed.\n");
            
            /* word read quad io */
            res = w25qxx_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: word read quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_word_read_quad_io check passed.\n");
            
            /* octal read quad io */
            res = w25qxx_octal_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: octal read quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_octal_word_read_quad_io check passed.\n");
            
            /* random data */
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_sector_erase_4k */
            res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_page_program_quad_input */
            res = w25qxx_page_program_quad_input(&gs_handle, 0x00000000, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program quad input failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* fast read */
            res = w25qxx_fast_read(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_page_program_quad_input check passed.\n");
            
            if (type >= W25Q256)
            {
                /* set address mode 4 byte */
                w25qxx_interface_debug_print("w25qxx: set address mode 4 byte.\n");
                
                /* set address mode 4 byte */
                res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* random data */
                for (j = 0; j < 256; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                
                /* w25qxx_sector_erase_4k */
                res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* w25qxx_page_program */
                res = w25qxx_page_program(&gs_handle, 0x00000000, gs_buffer_input, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* fast read dual output */
                res = w25qxx_fast_read_dual_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read dual output failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: fast_read_dual_output check passed.\n");
                
                /* fast read quad output */
                res = w25qxx_fast_read_quad_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read quad output failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: fast_read_quad_output check passed.\n");
                
                /* fast read dual io */
                res = w25qxx_fast_read_dual_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read dual io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_dual_io check passed.\n");
                
                /* fast read quad io */
                res = w25qxx_fast_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_quad_io check passed.\n");
                
                /* word read quad io */
                res = w25qxx_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: word read quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_word_read_quad_io check passed.\n");
                
                /* octal read quad io */
                res = w25qxx_octal_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: octal read quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_octal_word_read_quad_io check passed.\n");
                
                /* random data */
                for (j = 0; j < 256; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                
                /* w25qxx_sector_erase_4k */
                res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* w25qxx_page_program_quad_input */
                res = w25qxx_page_program_quad_input(&gs_handle, 0x00000000, gs_buffer_input, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: page program quad input failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* fast read */
                res = w25qxx_fast_read(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_page_program_quad_input check passed.\n");
                
                /* set address mode 3 byte */
                w25qxx_interface_debug_print("w25qxx: set address mode 3 byte.\n");
                
                /* set address mode 3 byte */
                res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_3_BYTE);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
            }
            
            /* enter to qspi mode */
            w25qxx_interface_debug_print("w25qxx: enter to qspi mode.\n");
            
            /* enter qspi */
            res = w25qxx_enter_qspi_mode(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: enter qspi mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* set chip interface spi */
            res = w25qxx_set_interface(&gs_handle, W25QXX_INTERFACE_QSPI);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
               
                return 1;
            }
        }
        
        /* w25qxx_set_read_parameters test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_read_parameters test.\n");
        
        /* 8 dummy max 80MHz test */
        w25qxx_interface_debug_print("w25qxx: set 8 dummy max 80MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_8_80MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* 6 dummy max 80MHz test */
        w25qxx_interface_debug_print("w25qxx: set 6 dummy max 80MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_6_80MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* 4 dummy max 55MHz test */
        w25qxx_interface_debug_print("w25qxx: set 4 dummy max 55MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_4_55MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* 2 dummy max 33MHz test */
        w25qxx_interface_debug_print("w25qxx: set 2 dummy max 33MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_2_33MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
    }
    
    /* finish read test */
    w25qxx_interface_debug_print("w25qxx: finish read test.\n");
    (void)w25qxx_deinit(&gs_handle);
    
    return 0;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  printf("************************************************\r\n");
  printf("*     Welcome to MCU Electronics Technology     *\r\n");
  printf("*          Mcore_MainBoard V1.0                *\r\n");
  printf("************************************************\r\n");
  printf("UART Configuration:\r\n");
  printf("- Baud Rate: 115200\r\n");
  printf("- Data Bits: 8\r\n");
  printf("- Stop Bits: 1\r\n");
  printf("- Parity: None\r\n");
  printf("\r\nStarting W25QXX test...\r\n\r\n");
  
  w25qxx_type_t chip_type = W25Q32;
  w25qxx_interface_t interface = W25QXX_INTERFACE_SPI;
  /* 运行W25QXX测试程序 */
  if (w25qxx_read_test(chip_type, interface, W25QXX_BOOL_FALSE) == 0)
//  if (w25qxx_test() == 0)
  {
      printf("\r\nW25QXX test passed!\r\n");
  }
  else
  {
      printf("\r\nW25QXX test failed!\r\n");
  }

  // Start DMA reception
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, bsp_uart0_rx_buff, USART_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);  // Disable DMA half transfer interrupt
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);  // Toggle LED to indicate system is running
    HAL_Delay(500);  // Blink every 500ms
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
