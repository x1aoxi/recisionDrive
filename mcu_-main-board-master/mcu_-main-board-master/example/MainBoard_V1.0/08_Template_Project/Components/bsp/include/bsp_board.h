#ifndef BSP_BOARD_H
#define BSP_BOARD_H

#include <stdbool.h>

#include "main.h"
#include "mcore_mainboard_v1_0.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Special config for board
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
HAL_StatusTypeDef bsp_board_init(void);

/**
 * @brief Call default button init code
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
HAL_StatusTypeDef bsp_btn_init(void);

/**
 * @brief Register the button event callback function.
 *
 * @param btn: A button handle to register
 * @param event: Button event
 * @param callback: Callback function.
 * @param user_data: user data
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG   Arguments is invalid.
 */
HAL_StatusTypeDef bsp_btn_register_callback(bsp_button_t btn, button_event_t event, button_cb_t callback, void *user_data);

/**
 * @brief Unregister the button event callback function.
 *
 * @param btn: A button handle to unregister
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG   Arguments is invalid.
 */
HAL_StatusTypeDef bsp_btn_rm_all_callback(bsp_button_t btn);

/**
 * @brief Unregister the button event callback function.
 *
 * @param btn: A button handle to unregister
 * @param event: Unregister event
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG   Arguments is invalid.
 */
HAL_StatusTypeDef bsp_btn_rm_event_callback(bsp_button_t btn, size_t event);

/**
 * @brief 初始化RGB LED
 * @return HAL状态
 */
HAL_StatusTypeDef bsp_led_init(void);

/**
 * @brief 设置RGB LED颜色
 * @param r 红色值（0-255）
 * @param g 绿色值（0-255）
 * @param b 蓝色值（0-255）
 * @return HAL状态
 */
HAL_StatusTypeDef bsp_led_set_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief  初始化AHT20温湿度传感器
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 初始化成功
 *         - HAL_ERROR: 初始化失败
 */
HAL_StatusTypeDef bsp_aht20_init(void);

/**
 * @brief  读取AHT20温湿度数据
 * @param  temperature: 温度值指针
 * @param  humidity: 湿度值指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 读取成功
 *         - HAL_ERROR: 读取失败
 */
HAL_StatusTypeDef bsp_aht20_read(float *temperature, uint8_t *humidity);

/**
 * @brief  反初始化AHT20设备
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 反初始化成功
 *         - HAL_ERROR: 反初始化失败
 */
HAL_StatusTypeDef bsp_aht20_deinit(void);

/**
 * @brief  初始化W25QXX Flash
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 初始化成功
 *         - HAL_ERROR: 初始化失败
 */
HAL_StatusTypeDef bsp_w25qxx_init(void);

/**
 * @brief  读取W25QXX Flash数据
 * @param  addr: 读取地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 读取成功
 *         - HAL_ERROR: 读取失败
 */
HAL_StatusTypeDef bsp_w25qxx_read(uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief  写入W25QXX Flash数据
 * @param  addr: 写入地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 写入成功
 *         - HAL_ERROR: 写入失败
 */
HAL_StatusTypeDef bsp_w25qxx_write(uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief  擦除整个W25QXX Flash芯片
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 擦除成功
 *         - HAL_ERROR: 擦除失败
 * @note   此操作将擦除芯片上的所有数据，请谨慎使用
 */
HAL_StatusTypeDef bsp_w25qxx_chip_erase(void);

/**
 * @brief  获取W25QXX Flash芯片信息
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 获取成功
 *         - HAL_ERROR: 获取失败
 */
HAL_StatusTypeDef bsp_w25qxx_get_info(void);

/**
 * @brief  W25QXX进入低功耗模式
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 成功进入低功耗模式
 *         - HAL_ERROR: 进入低功耗模式失败
 */
HAL_StatusTypeDef bsp_w25qxx_enter_power_down(void);

/**
 * @brief  唤醒W25QXX
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 唤醒成功
 *         - HAL_ERROR: 唤醒失败
 */
HAL_StatusTypeDef bsp_w25qxx_wake_up(void);

/**
 * @brief  反初始化W25QXX Flash
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 反初始化成功
 *         - HAL_ERROR: 反初始化失败
 */
HAL_StatusTypeDef bsp_w25qxx_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
