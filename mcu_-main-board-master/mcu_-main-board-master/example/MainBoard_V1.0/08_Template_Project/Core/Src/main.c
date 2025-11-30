/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "math.h"

#include "elog.h"
#include "MultiTimer.h"
#include "bsp_board.h"

#include "bsp_err_check.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
MultiTimer button_timer, idle_timer, rgb_timer, aht20_timer, icm20608_timer;  // 定义按键扫描定时器和RGB定时器
static float current_hue = 0.0f;  // HSV色调值 (0-360)
static const float saturation = 1.0f;  // 饱和度设为最大
static const float value = 0.1f;  // 亮度设为50%

// 定义数据存储结构体
typedef struct {
    uint32_t timestamp;
    float temperature;
    uint8_t humidity;
} sensor_data_t;

#define SECTOR_SIZE 4096
#define MAX_RECORDS_PER_SECTOR (SECTOR_SIZE / sizeof(sensor_data_t))
static uint32_t current_sector = 0;
static uint32_t record_count = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void hsv_to_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint64_t system_get_tick(void)
{
    return uwTick;
}

// IDLE扫描定时器回调函数
void idle_timer_callback(MultiTimer* timer, void *userData)
{
    elog_i("[system tick]", "%d", uwTick);
    multiTimerStart(timer, 1000, idle_timer_callback, userData);  // 重新启动定时器，10ms周期
}

// 按键扫描定时器回调函数
void button_timer_callback(MultiTimer* timer, void *userData)
{
    button_periodic_handler();  // 调用按键扫描处理函数
    multiTimerStart(timer, 10, button_timer_callback, userData);  // 重新启动定时器，10ms周期
}

// HSV转RGB的函数实现
void hsv_to_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2.0f) - 1));
    float m = v - c;
    float rt = 0, gt = 0, bt = 0;

    if (h >= 0 && h < 60) {
        rt = c; gt = x; bt = 0;
    } else if (h >= 60 && h < 120) {
        rt = x; gt = c; bt = 0;
    } else if (h >= 120 && h < 180) {
        rt = 0; gt = c; bt = x;
    } else if (h >= 180 && h < 240) {
        rt = 0; gt = x; bt = c;
    } else if (h >= 240 && h < 300) {
        rt = x; gt = 0; bt = c;
    } else {
        rt = c; gt = 0; bt = x;
    }

    *r = (uint8_t)((rt + m) * 255);
    *g = (uint8_t)((gt + m) * 255);
    *b = (uint8_t)((bt + m) * 255);
}

// RGB LED颜色循环定时器回调函数
void rgb_timer_callback(MultiTimer* timer, void *userData)
{
    uint8_t r, g, b;
    
    // 更新色调值
    current_hue += 1.0f;
    if (current_hue >= 360.0f) {
        current_hue = 0.0f;
    }
    
    // 转换HSV到RGB
    hsv_to_rgb(current_hue, saturation, value, &r, &g, &b);
    
    // 设置RGB LED颜色
    bsp_led_set_color(r, g, b);
    
    // 重新启动定时器，20ms更新一次
    multiTimerStart(timer, 20, rgb_timer_callback, userData);
}

void aht20_timer_callback(MultiTimer* timer, void *userData)
{
    float temperature = 0;
    uint8_t humidity = 0;
    HAL_StatusTypeDef status;
    sensor_data_t sensor_data;
    
    // 读取AHT20传感器数据
    bsp_aht20_read(&temperature, &humidity);
    
    // 填充数据结构
    sensor_data.timestamp = HAL_GetTick();
    sensor_data.temperature = temperature;
    sensor_data.humidity = humidity;
    
    // 计算存储地址
    uint32_t write_addr = current_sector * SECTOR_SIZE + (record_count * sizeof(sensor_data_t));
    
    // 如果当前扇区已满，擦除下一个扇区
    if (record_count >= MAX_RECORDS_PER_SECTOR) {
        current_sector++;
        record_count = 0;
        write_addr = current_sector * SECTOR_SIZE;
        
        status = bsp_w25qxx_chip_erase();
        if (status != HAL_OK) {
            elog_e("storage", "Failed to erase chip");
            goto restart_timer;
        }
    }
    
    // 写入数据
    status = bsp_w25qxx_write(write_addr, (uint8_t*)&sensor_data, sizeof(sensor_data_t));
    if (status != HAL_OK) {
        elog_e("storage", "Failed to write data");
        goto restart_timer;
    }
    
    record_count++;
    elog_i("storage", "Saved sensor data: Temp=%.2f°C, Hum=%d%%, Addr:0x%08X", 
           temperature, humidity, write_addr);

restart_timer:
    multiTimerStart(timer, 3000, aht20_timer_callback, userData);
}

// 数据读取定时器回调函数
void read_storage_callback(void *handle, void *arg)
{
    sensor_data_t sensor_data;
    HAL_StatusTypeDef status;
    
    // 遍历当前扇区的所有记录
    for (uint32_t i = 0; i < record_count; i++) {
        uint32_t read_addr = current_sector * SECTOR_SIZE + (i * sizeof(sensor_data_t));
        
        // 读取数据
        status = bsp_w25qxx_read(read_addr, (uint8_t*)&sensor_data, sizeof(sensor_data_t));
        if (status != HAL_OK) {
            elog_e("storage", "Failed to read data at addr 0x%08X", read_addr);
            continue;
        }
        
        // 打印读取的数据
        elog_i("storage", "Record[%d] Time:%lu Temp:%.2f°C Hum:%d%% Addr:0x%08X",
               i, sensor_data.timestamp, sensor_data.temperature, 
               sensor_data.humidity, read_addr);
    }
    
    elog_i("storage", "Data reading completed. Total records: %d", record_count);
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
//  MX_I2C1_Init();
  MX_SDIO_SD_Init();
  MX_USART1_UART_Init();
//  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  elog_init();
  elog_set_text_color_enabled(true);
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
  elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG);
  elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
  elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
  elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_TIME | ELOG_FMT_P_INFO | ELOG_FMT_T_INFO));
  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);
  elog_start();

  /* 初始化板级设备 */
  bsp_board_init();

  /* 测试RGB LED */
  bsp_led_set_color(255, 0, 0);  // 红色
  HAL_Delay(1000);
  bsp_led_set_color(0, 255, 0);  // 绿色
  HAL_Delay(1000);
  bsp_led_set_color(0, 0, 255);  // 蓝色
  HAL_Delay(1000);
  bsp_led_set_color(0, 0, 0);    // 关闭

  // 初始化并启动定时器
  multiTimerInstall(system_get_tick);
  multiTimerStart(&button_timer, 10, button_timer_callback, NULL);
	multiTimerStart(&aht20_timer, 3000, aht20_timer_callback, NULL);
  multiTimerStart(&idle_timer, 1000, idle_timer_callback, NULL);
  multiTimerStart(&rgb_timer, 20, rgb_timer_callback, NULL);
  
	BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_DOUBLE_CLICK, read_storage_callback, NULL));     // 双击
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      multiTimerYield();  // 运行定时器调度
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
     ex: printf("Wrong parameters value: file %s on line %d\r\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
