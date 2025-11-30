/**
 * @file
 * @brief ESP BSP: 米醋Mcore_MainBoard_V1.0
 */

#ifndef MCORE_MAINBOARD_V1_0
#define MCORE_MAINBOARD_V1_0

#include "main.h"
#include "button.h"
#include "led.h"
#include "tim.h"
#include "driver_aht20.h"
#include "driver_aht20_interface.h"
#include "driver_w25qxx.h"
#include "driver_w25qxx_interface.h"

/**************************************************************************************************
 *  BSP Capabilities
 **************************************************************************************************/

#define BSP_CAPS_DISPLAY        0 // 定义显示功能为不可用
#define BSP_CAPS_TOUCH          0 // 定义触摸功能为不可用
#define BSP_CAPS_BUTTONS        1 // 定义按钮功能为可用
#define BSP_CAPS_LED            1 // 定义LED功能为可用
#define BSP_CAPS_SDCARD         1 // 定义SD卡功能为可用
#define BSP_CAPS_IMU            1 // 定义惯性测量单元(IMU)功能为可用
#define BSP_CAPS_W25QXX					1 // 定义W25QXX FLASH为可用

/**************************************************************************************************
 *  米醋一号Mcore_MainBoard_V1.0 PINOUT
 **************************************************************************************************/

/* LED */
/* RGB LED句柄 */
typedef enum {
    BSP_RGB_LED = 0,     // 只有一个RGB LED
    BSP_RGB_LED_NUM,
} bsp_rgb_led_t;

/* RGB LED通道定义 */
extern TIM_HandleTypeDef htim2;
#define BSP_RGB_TIM              htim2           // 使用定时器2
#define BSP_RGB_TIM_PERIOD       1000            // PWM周期为1000

#define BSP_RGB_R_CHANNEL        TIM_CHANNEL_1   // 红色通道为CH1
#define BSP_RGB_G_CHANNEL        TIM_CHANNEL_2   // 绿色通道为CH2
#define BSP_RGB_B_CHANNEL        TIM_CHANNEL_3   // 蓝色通道为CH3

#define BSP_RGB_R_PORT        (GPIOA)
#define BSP_RGB_R_IO          (GPIO_PIN_0)
#define BSP_RGB_G_PORT        (GPIOA)
#define BSP_RGB_G_IO          (GPIO_PIN_1)
#define BSP_RGB_B_PORT        (GPIOA)
#define BSP_RGB_B_IO          (GPIO_PIN_2)

/* Buttons */
typedef enum {
    BSP_BUTTON_USER = 0,
    BSP_BUTTON_NUM,
} bsp_button_t;

#define BSP_BUTTON_USER_PORT  (GPIOC)
#define BSP_BUTTON_USER_IO    (GPIO_PIN_3)

/* SDCARD */
#define BSP_SDIO_DAT_PORT       (GPIOC)
#define BSP_SDIO_D0_IO          (GPIO_PIN_8)
#define BSP_SDIO_D1_IO          (GPIO_PIN_9)
#define BSP_SDIO_D2_IO          (GPIO_PIN_10)
#define BSP_SDIO_D3_IO          (GPIO_PIN_11)

#define BSP_SDIO_CLK_PORT       (GPIOC)
#define BSP_SDIO_CK_IO          (GPIO_PIN_12)

#define BSP_SDIO_CMD_PORT       (GPIOD)
#define BSP_SDIO_CMD_IO         (GPIO_PIN_2)

/* AHT20 */
#define BSP_ATH20_IIC_PORT      (GPIOB)
#define BSP_ATH20_IIC_SCL_IO    (GPIO_PIN_6)
#define BSP_ATH20_IIC_SDA_IO    (GPIO_PIN_7)

/* W25QXX */
#define BSP_W25QXX_SPI_PORT			(GPIOA)
#define BSP_W25QXX_SPI_CS				(GPIO_PIN_4)
#define BSP_W25QXX_SPI_SCLK			(GPIO_PIN_5)
#define BSP_W25QXX_SPI_MISO			(GPIO_PIN_6)
#define BSP_W25QXX_SPI_MOSI			(GPIO_PIN_7)

/* ICM20608 */
#define BSP_ICM20608_IIC_PORT      (GPIOB)
#define BSP_ICM20608_IIC_SCL_IO    (GPIO_PIN_6)
#define BSP_ICM20608_IIC_SDA_IO    (GPIO_PIN_7)

/* EXPAND */
typedef enum {
    TOP_ID_GAME,   /*!< top is Mcore_Game */
    TOP_ID_UNKNOW,   /*!< top isn't Mcore_Game */
    TOP_ID_LOST,     /*!< top Mcore_Game is connect when poweron, and lost */
} top_id_t;

typedef struct {
    uint16_t column1[20]; /*!< first column */
    uint16_t column2[20]; /*!< second column */
    uint16_t column3[20]; /*!< second column */
    uint16_t column4[20]; /*!< second column */
} pmod_pins_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  创建并初始化按键设备
 * @param  btn_array: 按键句柄数组，用于存储创建的按键对象
 * @param  btn_cnt: 输出参数，返回实际创建的按键数量
 * @param  btn_array_size: 输入参数，btn_array数组的最大容量
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 按键创建成功
 *         - HAL_ERROR: 创建失败（可能是堆栈内存不足）
 */
HAL_StatusTypeDef bsp_iot_button_create(button_handle_t btn_array[], int *btn_cnt, int btn_array_size);

/**
 * @brief 初始化RGB LED
 * @param led_array LED句柄数组
 * @param led_cnt LED数量
 * @param array_size 数组大小
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 按键创建成功
 *         - HAL_ERROR: 创建失败（可能是堆栈内存不足）
 */
HAL_StatusTypeDef bsp_rgb_led_init(led_handle_t led_array[], int *led_cnt, int array_size);

/**
 * @brief 设置RGB LED颜色
 * @param led_array LED句柄数组
 * @param r 红色值（0-255）
 * @param g 绿色值（0-255）
 * @param b 蓝色值（0-255）
 * @return HAL状态
 */
HAL_StatusTypeDef bsp_rgb_led_set_color(led_handle_t led_array[], uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief  创建并初始化AHT20温湿度传感器
 * @param  handle: AHT20句柄指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 创建成功
 *         - HAL_ERROR: 创建失败
 */
HAL_StatusTypeDef bsp_asair_aht20_init(aht20_handle_t *handle);

/**
 * @brief  读取AHT20温湿度数据
 * @param  handle: AHT20句柄
 * @param  temperature: 温度值指针
 * @param  humidity: 湿度值指针
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef bsp_asair_aht20_read(aht20_handle_t *handle, float *temperature, uint8_t *humidity);

/**
 * @brief  删除AHT20设备
 * @param  handle: AHT20句柄指针
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef bsp_asair_aht20_deinit(aht20_handle_t *handle);

/**
 * @brief  创建并初始化W25QXX Flash存储器
 * @param  handle: W25QXX句柄指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 初始化成功
 *         - HAL_ERROR: 初始化失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_init(w25qxx_handle_t *handle);

/**
 * @brief  读取W25QXX数据
 * @param  addr: 读取地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 读取成功
 *         - HAL_ERROR: 读取失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief  写入W25QXX数据
 * @param  addr: 写入地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 写入成功
 *         - HAL_ERROR: 写入失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_write(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief  反初始化W25QXX设备
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 反初始化成功
 *         - HAL_ERROR: 反初始化失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_deinit(w25qxx_handle_t *handle);

/**
 * @brief  W25QXX进入掉电模式
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 进入掉电模式成功
 *         - HAL_ERROR: 进入掉电模式失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_power_down(w25qxx_handle_t *handle);

/**
 * @brief  唤醒W25QXX
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 唤醒成功
 *         - HAL_ERROR: 唤醒失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_wake_up(w25qxx_handle_t *handle);

/**
 * @brief  擦除整个W25QXX芯片
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 擦除成功
 *         - HAL_ERROR: 擦除失败
 * @note   此操作将擦除芯片上的所有数据，请谨慎使用
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_chip_erase(w25qxx_handle_t *handle);

/**
 * @brief  获取W25QXX制造商和设备ID
 * @param  manufacturer: 制造商ID存储指针
 * @param  device_id: 设备ID存储指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 获取成功
 *         - HAL_ERROR: 获取失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_get_id(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id);

#ifdef __cplusplus
}
#endif

#endif
