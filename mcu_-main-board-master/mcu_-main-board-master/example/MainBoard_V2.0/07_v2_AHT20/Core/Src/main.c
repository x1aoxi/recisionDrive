/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "ebtn.h"
#include "multiTimer.h"
#include "elog.h"
#include "driver_aht20.h"
#include "driver_aht20_interface.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
MultiTimer mt_led, mt_key;
static aht20_handle_t gs_handle;        /**< aht20 handle */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MT_KEY_TIME       5   // 按键扫描时间
#define MT_AHT_TIME       200 // 陀螺仪读取时间
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
typedef enum {
    USER_BUTTON_0 = 0,
    USER_BUTTON_MAX,
} user_button_t;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t current_color = 0; // 0=红, 1=绿, 2=蓝

/* 按键参数配置(消抖时间), 连击间隔时间(连击判断), 长按开始时间(长按判断), 长按间隔时间,
   长按保持时间, 长按结束时间, 长按保持间隔时间 */
static const ebtn_btn_param_t defaul_ebtn_param = EBTN_PARAMS_INIT(20, 0, 20, 300, 200, 1000, 5);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static ebtn_btn_t btns[] = {
    EBTN_BUTTON_INIT(USER_BUTTON_0, &defaul_ebtn_param),
};

uint64_t bsp_timer_tick(void)
{
    return (uint64_t)HAL_GetTick();
}

uint8_t prv_btn_get_state(struct ebtn_btn *btn)
{
    switch(btn->key_id) {
        case USER_BUTTON_0:
            return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
        default:
            return 0;
    }
}

void prv_btn_event(struct ebtn_btn *btn, ebtn_evt_t evt)
{
    // 按键K1
    if((btn->key_id == USER_BUTTON_0) && evt == EBTN_EVT_ONCLICK &&(ebtn_click_get_count(btn) == 1)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        elog_i("main", "Btn click");
    }
    else if((btn->key_id == USER_BUTTON_0) && (ebtn_click_get_count(btn) == 2)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        elog_i("main", "Btn Double click");
    }
    else if((btn->key_id == USER_BUTTON_0) && evt == EBTN_EVT_KEEPALIVE) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        elog_i("main", "Btn Hold");
    }
}

void app_btn_init(void)
{
    ebtn_init(btns, EBTN_ARRAY_SIZE(btns), NULL, 0, prv_btn_get_state, prv_btn_event);
}

void mt_cb_key(MultiTimer* timer, void* userData)
{
    ebtn_process(HAL_GetTick());
    multiTimerStart(timer, MT_KEY_TIME, mt_cb_key, NULL);
}


/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t aht20_basic_init(void)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_AHT20_LINK_INIT(&gs_handle, aht20_handle_t);
    DRIVER_AHT20_LINK_IIC_INIT(&gs_handle, aht20_interface_iic_init);
    DRIVER_AHT20_LINK_IIC_DEINIT(&gs_handle, aht20_interface_iic_deinit);
    DRIVER_AHT20_LINK_IIC_READ_CMD(&gs_handle, aht20_interface_iic_read_cmd);
    DRIVER_AHT20_LINK_IIC_WRITE_CMD(&gs_handle, aht20_interface_iic_write_cmd);
    DRIVER_AHT20_LINK_DELAY_MS(&gs_handle, aht20_interface_delay_ms);
    DRIVER_AHT20_LINK_DEBUG_PRINT(&gs_handle, aht20_interface_debug_print);
    
    /* aht20 init */
    res = aht20_init(&gs_handle);
    if (res != 0)
    {
        aht20_interface_debug_print("aht20: init failed.\n");
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *temperature points to a converted temperature buffer
 * @param[out] *humidity points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t aht20_basic_read(float *temperature, uint8_t *humidity)
{
    uint32_t temperature_raw;
    uint32_t humidity_raw;
    
    /* read temperature and humidity */
    if (aht20_read_temperature_humidity(&gs_handle, (uint32_t *)&temperature_raw, temperature, 
                                       (uint32_t *)&humidity_raw, humidity) != 0)
    {
        return 1;
    }
    else
    {
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
uint8_t aht20_basic_deinit(void)
{
    /* deinit aht20 and close bus */
    if (aht20_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t aht20_read_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint32_t temperature_raw;
    uint32_t humidity_raw;
    float temperature;
    uint8_t humidity;
    aht20_info_t info;
   
    /* link interface function */
    DRIVER_AHT20_LINK_INIT(&gs_handle, aht20_handle_t);
    DRIVER_AHT20_LINK_IIC_INIT(&gs_handle, aht20_interface_iic_init);
    DRIVER_AHT20_LINK_IIC_DEINIT(&gs_handle, aht20_interface_iic_deinit);
    DRIVER_AHT20_LINK_IIC_READ_CMD(&gs_handle, aht20_interface_iic_read_cmd);
    DRIVER_AHT20_LINK_IIC_WRITE_CMD(&gs_handle, aht20_interface_iic_write_cmd);
    DRIVER_AHT20_LINK_DELAY_MS(&gs_handle, aht20_interface_delay_ms);
    DRIVER_AHT20_LINK_DEBUG_PRINT(&gs_handle, aht20_interface_debug_print);

    /* get aht20 information */
    res = aht20_info(&info);
    if (res != 0)
    {
        aht20_interface_debug_print("aht20: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print aht20 information */
        aht20_interface_debug_print("aht20: chip is %s.\n", info.chip_name);
        aht20_interface_debug_print("aht20: manufacturer is %s.\n", info.manufacturer_name);
        aht20_interface_debug_print("aht20: interface is %s.\n", info.interface);
        aht20_interface_debug_print("aht20: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        aht20_interface_debug_print("aht20: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        aht20_interface_debug_print("aht20: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        aht20_interface_debug_print("aht20: max current is %0.2fmA.\n", info.max_current_ma);
        aht20_interface_debug_print("aht20: max temperature is %0.1fC.\n", info.temperature_max);
        aht20_interface_debug_print("aht20: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start basic read test */
    aht20_interface_debug_print("aht20: start read test.\n");
    
    /* aht20 init */
    res = aht20_init(&gs_handle);
    if (res != 0)
    {
        aht20_interface_debug_print("aht20: init failed.\n");
       
        return 1;
    }
    
    /* delay 2000 ms for read */
    aht20_interface_delay_ms(2000);
    for (i = 0; i < times; i++)
    {
        /* read temperature and humidity */
        res = aht20_read_temperature_humidity(&gs_handle, (uint32_t *)&temperature_raw, (float *)&temperature, (uint32_t *)&humidity_raw, (uint8_t *)&humidity);
        if (res != 0)
        {
            aht20_interface_debug_print("aht20: read failed.\n");
            (void)aht20_deinit(&gs_handle);
           
            return 1;
        }
    
        /* print result */
        aht20_interface_debug_print("aht20: temperature: %.01fC.\n", temperature);
        aht20_interface_debug_print("aht20: humidity: %d%%.\n", humidity);

        /* delay 2000 ms*/
        aht20_interface_delay_ms(2000);
    }

    /* finish basic read test and exit */
    aht20_interface_debug_print("aht20: finish read test.\n");
    (void)aht20_deinit(&gs_handle);
    
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
  multiTimerInstall(bsp_timer_tick);
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
  MX_I2C1_Init();
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
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  app_btn_init();
  multiTimerStart(&mt_key,      MT_KEY_TIME,     mt_cb_key,        NULL);
  
  aht20_read_test(10);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    multiTimerYield();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
