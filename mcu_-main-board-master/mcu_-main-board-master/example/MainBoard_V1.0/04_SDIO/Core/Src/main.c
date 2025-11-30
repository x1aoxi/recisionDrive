/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author         : MCU Electronics Technology
  * @version        : V1.0
  * @date           : 2024-01-01
  * @note           : UART DMA Demo with IDLE detection
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
#include "string.h"
#include "ff.h"
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
uint8_t bsp_uart0_rx_buff[USART_BUFFER_SIZE];

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
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        // Add string terminator to the end of received data
        bsp_uart0_rx_buff[Size] = '\0';
        
        // Echo received data with proper format
        printf("[UART] Recv: %s", bsp_uart0_rx_buff);
        
        // If the received data doesn't end with \r\n, add it
        if (Size < 2 || bsp_uart0_rx_buff[Size-2] != '\r' || bsp_uart0_rx_buff[Size-1] != '\n') {
            printf("\r\n");
        }
        
        // Restart DMA reception
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, bsp_uart0_rx_buff, USART_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    }
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
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  printf("\r\n");
  printf("************************************************\r\n");
  printf("*        MCU Electronics Technology Co.         *\r\n");
  printf("*          Mcore_MainBoard V1.0                *\r\n");
  printf("*     Website: https://gitee.com/Ahypnis        *\r\n");
  printf("************************************************\r\n");
  printf("UART Configuration:\r\n");
  printf("- Baud Rate: 115200\r\n");
  printf("- Data Bits: 8\r\n");
  printf("- Stop Bits: 1\r\n");
  printf("- Parity: None\r\n");
  printf("\r\n");

  printf("=================== SD Card Test ===================\r\n");
  
  // 初始化SD卡
  printf("Step 1: Initializing SD Card\r\n");
  if(HAL_SD_Init(&hsd) != HAL_OK)
  {
      printf(">> SD Card initialization failed!\r\n");
      Error_Handler();
  }
  
  // 获取并显示SD卡信息
  printf("\r\nStep 2: Reading SD Card Information\r\n");
  if(HAL_SD_GetCardInfo(&hsd, &SDCardInfo) == HAL_OK)
  {
      printf(">> SD Card Information:\r\n");
      printf("- Card Type: ");
      switch(SDCardInfo.CardType)
      {
          case CARD_SDSC: printf("SDSC Card\r\n"); break;
          case CARD_SDHC_SDXC: printf("SDHC/SDXC Card\r\n"); break;
          default: printf("Unknown Card\r\n");
      }
      printf("- Card Version: %d\r\n", (int)SDCardInfo.CardVersion);
      printf("- Card Class: %d\r\n", (int)SDCardInfo.Class);
      printf("- Card Capacity: %d MB\r\n", (int)(SDCardInfo.BlockNbr / 2048)); // 转换为MB
      printf("- Block Size: %d Bytes\r\n", (int)SDCardInfo.BlockSize);
      printf("- Block Numbers: %d\r\n", (int)SDCardInfo.BlockNbr);
      
      // 验证卡信息是否有效
      if(SDCardInfo.BlockSize == 0 || SDCardInfo.BlockNbr == 0)
      {
          printf(">> Warning: Invalid card parameters detected!\r\n");
      }
  }
  else
  {
      printf(">> Failed to get SD card information!\r\n");
      Error_Handler();
  }
  printf("\r\n");
  
  // Start DMA reception
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, bsp_uart0_rx_buff, USART_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);  // Disable DMA half transfer interrupt
  
  printf("=============== FatFs Test Start ================\r\n");
    
  //在外部 SD 卡挂载文件系统，文件系统挂载时会对 SD 卡初始化
  printf("Step 3: Mounting File System\r\n");
  f_res = f_mount(&fs, "0:", 1);
    
  /*----------------------- 格式化测试 ---------------------------*/
  if(f_res == FR_NO_FILESYSTEM)
  {
      printf("No file system detected, formatting SD card...\r\n");
      /* 格式化 */
      BYTE sfd = 1;      /* 使用SFD分区方式 */
      UINT au = 0;       /* 自动选择簇大小 */
      
      f_res = f_mkfs("0:", sfd, au);
      if(f_res == FR_OK)
      {
          printf(">> Format successful!\r\n");
          /* 格式化后，先取消挂载 */
          f_res = f_mount(NULL, "0:", 1);
          /* 重新挂载 */
          f_res = f_mount(&fs, "0:", 1);
      }
      else
      {
          printf(">> Format failed! Error code: %d\r\n", f_res);
          printf("Error details: ");
          switch(f_res) {
              case FR_NOT_READY: printf("SD card not ready or not present\r\n"); break;
              case FR_DISK_ERR: printf("Hard error occurred in low level disk I/O\r\n"); break;
              case FR_WRITE_PROTECTED: printf("SD card is write protected\r\n"); break;
              default: printf("Unknown error\r\n");
          }
          Error_Handler();
      }       
  }
  else if(f_res != FR_OK)
  {
      printf(">> Mount failed! Error code: %d\r\n", f_res);
      printf("Error details: ");
      switch(f_res) {
          case FR_NOT_READY: printf("SD card not ready or not present\r\n"); break;
          case FR_DISK_ERR: printf("Hard error occurred in low level disk I/O\r\n"); break;
          case FR_INVALID_DRIVE: printf("Invalid drive number\r\n"); break;
          default: printf("Unknown error\r\n");
      }
      Error_Handler();
  }
  else
  {
      printf(">> Mount successful!\r\n");
      
      // 显示可用空间信息
      DWORD fre_clust, fre_sect, tot_sect;
      FATFS *fs_ptr = &fs;
      f_res = f_getfree("0:", &fre_clust, &fs_ptr);
      if(f_res == FR_OK) {
          tot_sect = (fs_ptr->n_fatent - 2) * fs_ptr->csize;
          fre_sect = fre_clust * fs_ptr->csize;
          printf("SD Card Space Info:\r\n");
          printf("- Total Space: %lu MB\r\n", tot_sect / 2048);
          printf("- Free Space: %lu MB\r\n", fre_sect / 2048);
          printf("- Used Space: %lu MB\r\n", (tot_sect - fre_sect) / 2048);
      }
  }
  printf("\r\n");
    
  /*----------------------- 文件系统测试：写测试 -----------------------------*/
  /* 打开文件，如果文件不存在则创建它 */
  printf("Step 4: File Write Test\r\n");
  printf("Creating file: %s\r\n", FILE_PATH);
  
  f_res = f_open(&file, FILE_PATH, FA_CREATE_ALWAYS | FA_WRITE);
  if(f_res == FR_OK)
  {
      printf(">> File created successfully\r\n");
      /* 将指定存储区内容写入到文件内 */
      f_res = f_write(&file, WriteBuffer, strlen((char *)WriteBuffer), &fnum);
      if(f_res == FR_OK)
      {
          printf(">> Write successful! (%d bytes)\r\n", fnum);
          printf("Written content:\r\n%s\r\n", WriteBuffer);
      }
      else
      {
          printf(">> Write failed! Error code: %d\r\n", f_res);
          printf("Error details: ");
          switch(f_res) {
              case FR_DISK_ERR: printf("Hard error occurred in low level disk I/O\r\n"); break;
              case FR_INT_ERR: printf("Internal error occurred\r\n"); break;
              case FR_DENIED: printf("Access denied\r\n"); break;
              default: printf("Unknown error\r\n");
          }
      }
      /* 确保数据写入到磁盘 */
      f_sync(&file);
      /* 不再读写，关闭文件 */
      f_close(&file);
  }
  else
  {
      printf(">> File creation failed! Error code: %d\r\n", f_res);
      printf("Error details: ");
      switch(f_res) {
          case FR_DISK_ERR: printf("Hard error occurred in low level disk I/O\r\n"); break;
          case FR_INT_ERR: printf("Internal error occurred\r\n"); break;
          case FR_NOT_READY: printf("SD card not ready\r\n"); break;
          case FR_NO_FILE: printf("Could not find the file\r\n"); break;
          case FR_NO_PATH: printf("Could not find the path\r\n"); break;
          case FR_INVALID_NAME: printf("Invalid file name\r\n"); break;
          case FR_DENIED: printf("Access denied\r\n"); break;
          case FR_EXIST: printf("File exists\r\n"); break;
          case FR_WRITE_PROTECTED: printf("Write protected\r\n"); break;
          default: printf("Unknown error\r\n");
      }
  }
  printf("\r\n");
    
  /*------------------- 文件系统测试：读测试 ------------------------------------*/
  printf("Step 5: File Read Test\r\n");
  printf("Opening file: %s\r\n", FILE_PATH);
  
  f_res = f_open(&file, FILE_PATH, FA_OPEN_EXISTING | FA_READ);
  if(f_res == FR_OK)
  {
      printf(">> File opened successfully\r\n");
      f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
      if(f_res == FR_OK)
      {
          ReadBuffer[fnum] = '\0';  // 确保字符串正确终止
          printf(">> Read successful! (%d bytes)\r\n", fnum);
          printf("Read content:\r\n%s\r\n", ReadBuffer);
      }
      else
      {
          printf(">> Read failed! Error code: %d\r\n", f_res);
          printf("Error details: ");
          switch(f_res) {
              case FR_DISK_ERR: printf("Hard error occurred in low level disk I/O\r\n"); break;
              case FR_INT_ERR: printf("Internal error occurred\r\n"); break;
              case FR_DENIED: printf("Access denied\r\n"); break;
              default: printf("Unknown error\r\n");
          }
      }
  }
  else
  {
      printf(">> File open failed! Error code: %d\r\n", f_res);
      printf("Error details: ");
      switch(f_res) {
          case FR_DISK_ERR: printf("Hard error occurred in low level disk I/O\r\n"); break;
          case FR_INT_ERR: printf("Internal error occurred\r\n"); break;
          case FR_NOT_READY: printf("SD card not ready\r\n"); break;
          case FR_NO_FILE: printf("Could not find the file\r\n"); break;
          case FR_NO_PATH: printf("Could not find the path\r\n"); break;
          case FR_INVALID_NAME: printf("Invalid file name\r\n"); break;
          case FR_DENIED: printf("Access denied\r\n"); break;
          default: printf("Unknown error\r\n");
      }
  }
  /* 不再读写，关闭文件 */
  f_close(&file);
  /* 不再使用文件系统，取消挂载文件系统 */
  f_mount(NULL, "0:", 1);
  
  printf("\r\n================ Test Complete ==================\r\n");
  printf("System is ready. Please input any characters...\r\n\r\n");
  
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
