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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "driver_icm20608.h"
#include "driver_icm20608_interface.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_R_Pin GPIO_PIN_0
#define LED_G_Pin GPIO_PIN_1
#define LED_B_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOA

#define LED_R_ON()    HAL_GPIO_WritePin(LED_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET)
#define LED_R_OFF()   HAL_GPIO_WritePin(LED_GPIO_Port, LED_R_Pin, GPIO_PIN_SET)
#define LED_G_ON()    HAL_GPIO_WritePin(LED_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET)
#define LED_G_OFF()   HAL_GPIO_WritePin(LED_GPIO_Port, LED_G_Pin, GPIO_PIN_SET)
#define LED_B_ON()    HAL_GPIO_WritePin(LED_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET)
#define LED_B_OFF()   HAL_GPIO_WritePin(LED_GPIO_Port, LED_B_Pin, GPIO_PIN_SET)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
icm20608_handle_t icm20608;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LEDR_Blink(void)
{
    // 红灯闪烁
    LED_R_ON();
    LED_G_OFF();
    LED_B_OFF();
    HAL_Delay(300);
    LED_R_OFF();
    HAL_Delay(300);
}

void LEDG_Blick(void)
{
    // 绿灯闪烁
    LED_R_OFF();
    LED_G_ON();
    LED_B_OFF();
    HAL_Delay(100);
    LED_G_OFF();
    HAL_Delay(100);
}

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

uint8_t icm20608_basic_test(void)
{
  HAL_Delay(100);  /* 等待传感器上电稳定 */

  /* ICM20608测试 */
  uint8_t res;
  icm20608_info_t info;
    
  /* 1. 配置ICM20608句柄 */
  DRIVER_ICM20608_LINK_INIT(&icm20608, icm20608_handle_t);
  DRIVER_ICM20608_LINK_IIC_INIT(&icm20608, icm20608_interface_iic_init);
  DRIVER_ICM20608_LINK_IIC_DEINIT(&icm20608, icm20608_interface_iic_deinit);
  DRIVER_ICM20608_LINK_IIC_READ(&icm20608, icm20608_interface_iic_read);
  DRIVER_ICM20608_LINK_IIC_WRITE(&icm20608, icm20608_interface_iic_write);
  DRIVER_ICM20608_LINK_DELAY_MS(&icm20608, icm20608_interface_delay_ms);
  DRIVER_ICM20608_LINK_DEBUG_PRINT(&icm20608, icm20608_interface_debug_print);
    
  /* 2. 获取芯片信息 */
  res = icm20608_info(&info);
  if (res)
  {
      printf("icm20608: get info failed, code: %d\r\n", res);
      LEDR_Blink();
      return 1;
  }
  icm20608_interface_debug_print("icm20608: chip is %s.\r\n", info.chip_name);
  icm20608_interface_debug_print("icm20608: manufacturer is %s.\r\n", info.manufacturer_name);
  icm20608_interface_debug_print("icm20608: interface is %s.\r\n", info.interface);
  icm20608_interface_debug_print("icm20608: driver version is 0x%x.\r\n", info.driver_version);
  icm20608_interface_debug_print("icm20608: supply voltage min is %0.1fV.\r\n", info.supply_voltage_min_v);
  icm20608_interface_debug_print("icm20608: supply voltage max is %0.1fV.\r\n", info.supply_voltage_max_v);
  icm20608_interface_debug_print("icm20608: max current is %0.2fmA.\r\n", info.max_current_ma);
  icm20608_interface_debug_print("icm20608: temperature min is %0.1fC.\r\n", info.temperature_min);
  icm20608_interface_debug_print("icm20608: temperature max is %0.1fC.\r\n", info.temperature_max);

  /* 设置IIC地址 */
  icm20608.iic_addr = ICM20608_ADDRESS_AD0_LOW;  /* AD0接GND时的地址 */

  /* 3. 初始化ICM20608 */
  res = icm20608_init(&icm20608);
  if (res)
  {
      icm20608_interface_debug_print("icm20608: init failed, code: %d\r\n", res);
      LEDR_Blink();
      return 1;
  }
  icm20608_interface_debug_print("icm20608: init success.\r\n");
  
  LEDG_Blick();
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
  uint8_t res = 0;
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  icm20608_basic_test();
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    int16_t raw_temp;
    float temp;
    int16_t raw_accel[3];
    float accel[3];
    int16_t raw_gyro[3];
    float gyro[3];
    
    /* 读取温度 */
    res = icm20608_read_temperature(&icm20608, &raw_temp, &temp);
    if (res == 0)
    {
        printf("icm20608: temperature is %0.2fC.\r\n", temp);
    }
    
    /* 读取加速度 */
    res = icm20608_read_accelerometer(&icm20608, raw_accel, accel);
    if (res == 0)
    {
        printf("icm20608: acc x is %0.2fg.\r\n", accel[0]);
        printf("icm20608: acc y is %0.2fg.\r\n", accel[1]);
        printf("icm20608: acc z is %0.2fg.\r\n", accel[2]);
    }
    
    /* 读取角速度 */
    res = icm20608_read_gyroscope(&icm20608, raw_gyro, gyro);
    if (res == 0)
    {
        printf("icm20608: gyro x is %0.2fdps.\r\n", gyro[0]);
        printf("icm20608: gyro y is %0.2fdps.\r\n", gyro[1]);
        printf("icm20608: gyro z is %0.2fdps.\r\n", gyro[2]);
    }
    
    LEDG_Blick();
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
