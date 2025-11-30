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
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ebtn.h"

#define LOG_TAG    "MAIN"  // 定义日志标签
#include "elog.h"          // 包含elog头文件
#include "multiTimer.h"
#include "string.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
MultiTimer mt_led, mt_key;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MT_KEY_TIME       5   // 按键扫描时间
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

FATFS fs;                       /* FatFs 文件系统对象 */
FIL file;                       /* 文件对象 */
FRESULT f_res;                  /* 文件操作结果 */
UINT fnum;                      /* 文件成功读写数量 */
BYTE ReadBuffer[1024] = {0};    /* 读缓冲区 */
BYTE WriteBuffer[] =            /* 写缓冲区 */
    "This is MCU Electronics Technology STM32 working with FatFs\r\n"
    "Board: Mcore_MainBoard V1.0\r\n";

HAL_SD_CardInfoTypeDef SDCardInfo;  /* SD卡信息结构体 */
char sd_info_buffer[100];           /* SD卡信息显示缓冲区 */
const TCHAR *FILE_PATH = "0:/test.txt";  /* 文件路径（注意：使用正斜杠） */


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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
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
  multiTimerStart(&mt_key,     MT_KEY_TIME,     mt_cb_key,        NULL);
  
  log_i("");
  log_i("************************************************");
  log_i("*        MCU Electronics Technology Co.         *");
  log_i("*          Mcore_MainBoard V1.0                *");
  log_i("*     Website: https://gitee.com/Ahypnis        *");
  log_i("************************************************");
  log_i("UART Configuration:");
  log_i("- Baud Rate: 115200");
  log_i("- Data Bits: 8");
  log_i("- Stop Bits: 1");
  log_i("- Parity: None");
  log_i("");

  log_i("=================== SD Card Test ===================");
  
  // 初始化SD卡
  log_i("Step 1: Initializing SD Card");
  if(HAL_SD_Init(&hsd) != HAL_OK)
  {
      log_e(">> SD Card initialization failed!");
      Error_Handler();
  }

  // 获取并显示SD卡信息
  log_i("");
  log_i("Step 2: Reading SD Card Information");
  if(HAL_SD_GetCardInfo(&hsd, &SDCardInfo) == HAL_OK)
  {
      log_i(">> SD Card Information:");
      log_i("- Card Type: %s",
          SDCardInfo.CardType == CARD_SDSC ? "SDSC Card" :
          SDCardInfo.CardType == CARD_SDHC_SDXC ? "SDHC/SDXC Card" : "Unknown Card");
      log_i("- Card Version: %d", (int)SDCardInfo.CardVersion);
      log_i("- Card Class: %d", (int)SDCardInfo.Class);
      log_i("- Card Capacity: %d MB", (int)(SDCardInfo.BlockNbr / 2048)); // 转换为MB
      log_i("- Block Size: %d Bytes", (int)SDCardInfo.BlockSize);
      log_i("- Block Numbers: %d", (int)SDCardInfo.BlockNbr);

      // 验证卡信息是否有效
      if(SDCardInfo.BlockSize == 0 || SDCardInfo.BlockNbr == 0)
      {
          log_w(">> Warning: Invalid card parameters detected!");
      }
  }
  else
  {
      log_e(">> Failed to get SD card information!");
      Error_Handler();
  }
  log_i("");
  
  // Start DMA reception
  log_i("=============== FatFs Test Start ================");

  //在外部 SD 卡挂载文件系统，文件系统挂载时会对 SD 卡初始化
  log_i("Step 3: Mounting File System");
  f_res = f_mount(&fs, "0:", 1);
    
  /*----------------------- 格式化测试 ---------------------------*/
  if(f_res == FR_NO_FILESYSTEM)
  {
      log_w("No file system detected, formatting SD card...");
      /* 格式化 */
      BYTE opt = FM_FAT32 | FM_SFD;  /* FAT32格式 + SFD分区方式 */
      DWORD au = 0;                  /* 自动选择簇大小 */
      BYTE work[512];                /* 工作缓冲区 */

      f_res = f_mkfs("0:", opt, au, work, sizeof(work));
      if(f_res == FR_OK)
      {
          log_i(">> Format successful!");
          /* 格式化后，先取消挂载 */
          f_res = f_mount(NULL, "0:", 1);
          /* 重新挂载 */
          f_res = f_mount(&fs, "0:", 1);
      }
      else
      {
          log_e(">> Format failed! Error code: %d", f_res);
          log_e("Error details: %s",
              f_res == FR_NOT_READY ? "SD card not ready or not present" :
              f_res == FR_DISK_ERR ? "Hard error occurred in low level disk I/O" :
              f_res == FR_WRITE_PROTECTED ? "SD card is write protected" : "Unknown error");
          Error_Handler();
      }
  }
  else if(f_res != FR_OK)
  {
      log_e(">> Mount failed! Error code: %d", f_res);
      log_e("Error details: %s",
          f_res == FR_NOT_READY ? "SD card not ready or not present" :
          f_res == FR_DISK_ERR ? "Hard error occurred in low level disk I/O" :
          f_res == FR_INVALID_DRIVE ? "Invalid drive number" : "Unknown error");
      Error_Handler();
  }
  else
  {
      log_i(">> Mount successful!");

      // 显示可用空间信息
      DWORD fre_clust, fre_sect, tot_sect;
      FATFS *fs_ptr = &fs;
      f_res = f_getfree("0:", &fre_clust, &fs_ptr);
      if(f_res == FR_OK) {
          tot_sect = (fs_ptr->n_fatent - 2) * fs_ptr->csize;
          fre_sect = fre_clust * fs_ptr->csize;
          log_i("SD Card Space Info:");
          log_i("- Total Space: %lu MB", tot_sect / 2048);
          log_i("- Free Space: %lu MB", fre_sect / 2048);
          log_i("- Used Space: %lu MB", (tot_sect - fre_sect) / 2048);
      }
  }
  log_i("");
    
  /*----------------------- 文件系统测试：写测试 -----------------------------*/
  /* 打开文件，如果文件不存在则创建它 */
  log_i("Step 4: File Write Test");
  log_i("Creating file: %s", FILE_PATH);

  f_res = f_open(&file, FILE_PATH, FA_CREATE_ALWAYS | FA_WRITE);
  if(f_res == FR_OK)
  {
      log_i(">> File created successfully");
      /* 将指定存储区内容写入到文件内 */
      f_res = f_write(&file, WriteBuffer, strlen((char *)WriteBuffer), &fnum);
      if(f_res == FR_OK)
      {
          log_i(">> Write successful! (%d bytes)", fnum);
          log_i("Written content:");
          log_i("%s", WriteBuffer);
      }
      else
      {
          log_e(">> Write failed! Error code: %d", f_res);
          log_e("Error details: %s",
              f_res == FR_DISK_ERR ? "Hard error occurred in low level disk I/O" :
              f_res == FR_INT_ERR ? "Internal error occurred" :
              f_res == FR_DENIED ? "Access denied" : "Unknown error");
      }
      /* 确保数据写入到磁盘 */
      f_sync(&file);
      /* 不再读写，关闭文件 */
      f_close(&file);
  }
  else
  {
      log_e(">> File creation failed! Error code: %d", f_res);
      const char* error_msg =
          f_res == FR_DISK_ERR ? "Hard error occurred in low level disk I/O" :
          f_res == FR_INT_ERR ? "Internal error occurred" :
          f_res == FR_NOT_READY ? "SD card not ready" :
          f_res == FR_NO_FILE ? "Could not find the file" :
          f_res == FR_NO_PATH ? "Could not find the path" :
          f_res == FR_INVALID_NAME ? "Invalid file name" :
          f_res == FR_DENIED ? "Access denied" :
          f_res == FR_EXIST ? "File exists" :
          f_res == FR_WRITE_PROTECTED ? "Write protected" : "Unknown error";
      log_e("Error details: %s", error_msg);
  }
  log_i("");
    
  /*------------------- 文件系统测试：读测试 ------------------------------------*/
  log_i("Step 5: File Read Test");
  log_i("Opening file: %s", FILE_PATH);

  f_res = f_open(&file, FILE_PATH, FA_OPEN_EXISTING | FA_READ);
  if(f_res == FR_OK)
  {
      log_i(">> File opened successfully");
      f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
      if(f_res == FR_OK)
      {
          ReadBuffer[fnum] = '\0';  // 确保字符串正确终止
          log_i(">> Read successful! (%d bytes)", fnum);
          log_i("Read content:");
          log_i("%s", ReadBuffer);
      }
      else
      {
          log_e(">> Read failed! Error code: %d", f_res);
          log_e("Error details: %s",
              f_res == FR_DISK_ERR ? "Hard error occurred in low level disk I/O" :
              f_res == FR_INT_ERR ? "Internal error occurred" :
              f_res == FR_DENIED ? "Access denied" : "Unknown error");
      }
  }
  else
  {
      log_e(">> File open failed! Error code: %d", f_res);
      const char* error_msg =
          f_res == FR_DISK_ERR ? "Hard error occurred in low level disk I/O" :
          f_res == FR_INT_ERR ? "Internal error occurred" :
          f_res == FR_NOT_READY ? "SD card not ready" :
          f_res == FR_NO_FILE ? "Could not find the file" :
          f_res == FR_NO_PATH ? "Could not find the path" :
          f_res == FR_INVALID_NAME ? "Invalid file name" :
          f_res == FR_DENIED ? "Access denied" : "Unknown error";
      log_e("Error details: %s", error_msg);
  }
  /* 不再读写，关闭文件 */
  f_close(&file);
  /* 不再使用文件系统，取消挂载文件系统 */
  f_mount(NULL, "0:", 1);

  log_i("");
  log_i("================ Test Complete ==================");
  log_i("System is ready. Please input any characters...");
  log_i("");
  
  
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
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
