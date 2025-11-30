#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bsp_board.h"
#include "bsp_err_check.h"
#include "mcore_mainboard_v1_0.h"
#include "elog.h"

static button_handle_t *g_btn_handle = NULL;
static led_handle_t g_rgb_led[BSP_RGB_LED_NUM] = {NULL};
static aht20_handle_t gs_aht20_handle;
static w25qxx_handle_t gs_w25qxx_handle;

static const char *TAG = "bsp_board";

HAL_StatusTypeDef bsp_led_init(void)
{
    int led_cnt = 0;
    return bsp_rgb_led_init(g_rgb_led, &led_cnt, BSP_RGB_LED_NUM);
}

HAL_StatusTypeDef bsp_led_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    return bsp_rgb_led_set_color(g_rgb_led, r, g, b);
}

HAL_StatusTypeDef bsp_btn_init(void)
{
    if (g_btn_handle != NULL) {
        return HAL_ERROR;
    }

    int btn_num = 0;
    g_btn_handle = calloc(sizeof(button_handle_t), BSP_BUTTON_NUM);
    if (g_btn_handle == NULL) {
        elog_e(TAG, "Failed to allocate memory for button");
        return HAL_ERROR;
    }
    return bsp_iot_button_create(g_btn_handle, &btn_num, BSP_BUTTON_NUM);
}

HAL_StatusTypeDef bsp_btn_register_callback(bsp_button_t btn, button_event_t event, button_cb_t callback, void *user_data)
{
    assert((g_btn_handle) && "button not initialized");
    assert((btn < BSP_BUTTON_NUM) && "button id incorrect");

    if (NULL == callback) {
        return button_unregister_cb(g_btn_handle[btn], event);
    }
    return button_register_cb(g_btn_handle[btn], event, callback, user_data);
}

HAL_StatusTypeDef bsp_btn_rm_all_callback(bsp_button_t btn)
{
    assert((g_btn_handle) && "button not initialized");
    assert((btn < BSP_BUTTON_NUM) && "button id incorrect");

    for (size_t event = 0; event < BUTTON_EVENT_MAX; event++) {
        button_unregister_cb(g_btn_handle[btn], event);
    }
    return HAL_OK;
}

HAL_StatusTypeDef bsp_btn_rm_event_callback(bsp_button_t btn, size_t event)
{
    assert((g_btn_handle) && "button not initialized");
    assert((btn < BSP_BUTTON_NUM) && "button id incorrect");

    button_unregister_cb(g_btn_handle[btn], event);
    return HAL_OK;
}

__weak void button_callback(void *handle, void *arg)
{
    button_event_t event = button_get_event((button_handle_t)handle);
    
    switch (event) {
        case BUTTON_PRESS_DOWN:
            elog_d("BSP", "press\r");
            break;
            
        case BUTTON_PRESS_UP:
            elog_d("BSP", "relase\r");
            break;
            
        case BUTTON_SINGLE_CLICK:
            elog_d("BSP", "click\r");
            break;
            
        case BUTTON_DOUBLE_CLICK:
            elog_d("BSP", "double click\r");
            break;
            
        case BUTTON_MULTIPLE_CLICK:
            elog_d("BSP", "mutli click\r");
            break;
            
        case BUTTON_LONG_PRESS_START:
            elog_d("BSP", "long press begin\r");
            break;
            
        case BUTTON_LONG_PRESS_HOLD:
            elog_d("BSP", "long press keep\r");
            break;
            
        case BUTTON_LONG_PRESS_UP:
            elog_d("BSP", "long press relase\r");
            break;
            
        default:
            break;
    }
}

/**
 * @brief  初始化AHT20温湿度传感器
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 初始化成功
 *         - HAL_ERROR: 初始化失败
 */
HAL_StatusTypeDef bsp_aht20_init(void)
{
    HAL_StatusTypeDef ret = HAL_OK;

    /* 链接驱动接口函数 */
    DRIVER_AHT20_LINK_INIT(&gs_aht20_handle, aht20_handle_t);
    DRIVER_AHT20_LINK_IIC_INIT(&gs_aht20_handle, aht20_interface_iic_init);
    DRIVER_AHT20_LINK_IIC_DEINIT(&gs_aht20_handle, aht20_interface_iic_deinit);
    DRIVER_AHT20_LINK_IIC_READ_CMD(&gs_aht20_handle, aht20_interface_iic_read_cmd);
    DRIVER_AHT20_LINK_IIC_WRITE_CMD(&gs_aht20_handle, aht20_interface_iic_write_cmd);
    DRIVER_AHT20_LINK_DELAY_MS(&gs_aht20_handle, aht20_interface_delay_ms);
    DRIVER_AHT20_LINK_DEBUG_PRINT(&gs_aht20_handle, aht20_interface_debug_print);

    /* 初始化AHT20设备 */
    ret = bsp_asair_aht20_init(&gs_aht20_handle);
    if (ret != HAL_OK) {
        elog_e(TAG, "AHT20 initialization failed");
        return HAL_ERROR;
    }

    elog_i(TAG, "AHT20 initialized successfully");
    return ret;
}

/**
 * @brief  读取AHT20温湿度数据
 * @param  temperature: 温度值指针
 * @param  humidity: 湿度值指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 读取成功
 *         - HAL_ERROR: 读取失败
 */
HAL_StatusTypeDef bsp_aht20_read(float *temperature, uint8_t *humidity)
{
    HAL_StatusTypeDef ret;

    /* 参数检查 */
    if (temperature == NULL || humidity == NULL) {
        elog_e(TAG, "Invalid parameters for AHT20 read");
        return HAL_ERROR;
    }

    /* 检查设备是否已初始化 */
    if (gs_aht20_handle.inited != 1) {
        elog_e(TAG, "AHT20 not initialized");
        return HAL_ERROR;
    }

    /* 读取温湿度数据 */
    ret = bsp_asair_aht20_read(&gs_aht20_handle, temperature, humidity);
    if (ret != HAL_OK) {
        elog_e(TAG, "Failed to read AHT20 data");
        return HAL_ERROR;
    }

    elog_d(TAG, "AHT20 read: temperature=%.1f°C, humidity=%d%%", *temperature, *humidity);
    return HAL_OK;
}

/**
 * @brief  反初始化AHT20设备
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 反初始化成功
 *         - HAL_ERROR: 反初始化失败
 */
HAL_StatusTypeDef bsp_aht20_deinit(void)
{
    HAL_StatusTypeDef ret;

    /* 检查设备是否已初始化 */
    if (gs_aht20_handle.inited != 1) {
        elog_w(TAG, "AHT20 already deinitialized");
        return HAL_OK;
    }

    /* 反初始化设备 */
    ret = bsp_asair_aht20_deinit(&gs_aht20_handle);
    if (ret != HAL_OK) {
        elog_e(TAG, "Failed to deinitialize AHT20");
        return HAL_ERROR;
    }

    elog_i(TAG, "AHT20 deinitialized successfully");
    return HAL_OK;
}

/**
 * @brief  初始化W25QXX Flash
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 初始化成功
 *         - HAL_ERROR: 初始化失败
 */
HAL_StatusTypeDef bsp_w25qxx_init(void)
{
    HAL_StatusTypeDef ret = HAL_OK;

    /* 链接驱动接口函数 */
    DRIVER_W25QXX_LINK_INIT(&gs_w25qxx_handle, w25qxx_handle_t);
    DRIVER_W25QXX_LINK_SPI_QSPI_INIT(&gs_w25qxx_handle, w25qxx_interface_spi_qspi_init);
    DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(&gs_w25qxx_handle, w25qxx_interface_spi_qspi_deinit);
    DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(&gs_w25qxx_handle, w25qxx_interface_spi_qspi_write_read);
    DRIVER_W25QXX_LINK_DELAY_MS(&gs_w25qxx_handle, w25qxx_interface_delay_ms);
    DRIVER_W25QXX_LINK_DELAY_US(&gs_w25qxx_handle, w25qxx_interface_delay_us);
    DRIVER_W25QXX_LINK_DEBUG_PRINT(&gs_w25qxx_handle, w25qxx_interface_debug_print);

    /* 初始化W25QXX设备 */
    if (bsp_zetta_w25qxx_init(&gs_w25qxx_handle) != 0) {
        elog_e(TAG, "W25QXX initialization failed");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX initialized successfully");
    return ret;
}

/**
 * @brief  读取W25QXX Flash数据
 * @param  addr: 读取地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 读取成功
 *         - HAL_ERROR: 读取失败
 */
HAL_StatusTypeDef bsp_w25qxx_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    /* 参数检查 */
    if (data == NULL || len == 0) {
        elog_e(TAG, "Invalid parameters for W25QXX read");
        return HAL_ERROR;      
    }

    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_e(TAG, "W25QXX not initialized");
        return HAL_ERROR;
    }

    /* 读取数据 */
    if (bsp_zetta_w25qxx_read(&gs_w25qxx_handle, addr, data, len) != HAL_OK) {
        elog_e(TAG, "Failed to read W25QXX data at address 0x%08X", addr);
        return HAL_ERROR;
    }

    elog_d(TAG, "W25QXX read %d bytes from address 0x%08X", len, addr);
    return HAL_OK;
}

/**
 * @brief  写入W25QXX Flash数据
 * @param  addr: 写入地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 写入成功
 *         - HAL_ERROR: 写入失败
 */
HAL_StatusTypeDef bsp_w25qxx_write(uint32_t addr, uint8_t *data, uint32_t len)
{
    /* 参数检查 */
    if (data == NULL || len == 0) {
        elog_e(TAG, "Invalid parameters for W25QXX write");
        return HAL_ERROR;
    }

    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_e(TAG, "W25QXX not initialized");
        return HAL_ERROR;
    }

    /* 写入数据 */
    if (bsp_zetta_w25qxx_write(&gs_w25qxx_handle, addr, data, len) != HAL_OK) {
        elog_e(TAG, "Failed to write W25QXX data at address 0x%08X", addr);
        return HAL_ERROR;
    }

    elog_d(TAG, "W25QXX wrote %d bytes to address 0x%08X", len, addr);
    return HAL_OK;
}

/**
 * @brief  擦除整个W25QXX Flash芯片
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 擦除成功
 *         - HAL_ERROR: 擦除失败
 * @note   此操作将擦除芯片上的所有数据，请谨慎使用
 */
HAL_StatusTypeDef bsp_w25qxx_chip_erase(void)
{
    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_e(TAG, "W25QXX not initialized");
        return HAL_ERROR;
    }

    elog_w(TAG, "Starting W25QXX chip erase - ALL DATA WILL BE LOST");

    /* 执行芯片擦除 */
    if (bsp_zetta_w25qxx_chip_erase(&gs_w25qxx_handle) != HAL_OK) {
        elog_e(TAG, "W25QXX chip erase failed");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX chip erase completed successfully");
    return HAL_OK;
}

/**
 * @brief  获取W25QXX Flash芯片信息
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 获取成功
 *         - HAL_ERROR: 获取失败
 */
HAL_StatusTypeDef bsp_w25qxx_get_info(void)
{
    uint8_t manufacturer_id;
    uint8_t device_id;

    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_e(TAG, "W25QXX not initialized");
        return HAL_ERROR;
    }

    /* 读取芯片ID */
    if (bsp_zetta_w25qxx_get_id(&gs_w25qxx_handle, &manufacturer_id, &device_id) != HAL_OK) {
        elog_e(TAG, "Failed to get W25QXX chip ID");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX Info - Manufacturer ID: 0x%02X, Device ID: 0x%02X", 
           manufacturer_id, device_id);
    return HAL_OK;
}

/**
 * @brief  W25QXX进入低功耗模式
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 成功进入低功耗模式
 *         - HAL_ERROR: 进入低功耗模式失败
 */
HAL_StatusTypeDef bsp_w25qxx_enter_power_down(void)
{
    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_e(TAG, "W25QXX not initialized");
        return HAL_ERROR;
    }

    if (bsp_zetta_w25qxx_power_down(&gs_w25qxx_handle) != HAL_OK) {
        elog_e(TAG, "Failed to enter power down mode");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX entered power down mode");
    return HAL_OK;
}

/**
 * @brief  唤醒W25QXX
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 唤醒成功
 *         - HAL_ERROR: 唤醒失败
 */
HAL_StatusTypeDef bsp_w25qxx_wake_up(void)
{
    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_e(TAG, "W25QXX not initialized");
        return HAL_ERROR;
    }

    if (bsp_zetta_w25qxx_wake_up(&gs_w25qxx_handle) != HAL_OK) {
        elog_e(TAG, "Failed to wake up W25QXX");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX woke up successfully");
    return HAL_OK;
}

/**
 * @brief  反初始化W25QXX Flash
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 反初始化成功
 *         - HAL_ERROR: 反初始化失败
 */
HAL_StatusTypeDef bsp_w25qxx_deinit(void)
{
    /* 检查设备是否已初始化 */
    if (gs_w25qxx_handle.inited != 1) {
        elog_w(TAG, "W25QXX already deinitialized");
        return HAL_OK;
    }

    if (bsp_zetta_w25qxx_deinit(&gs_w25qxx_handle) != HAL_OK) {
        elog_e(TAG, "Failed to deinitialize W25QXX");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX deinitialized successfully");
    return HAL_OK;
}

HAL_StatusTypeDef bsp_board_init(void)
{
    HAL_StatusTypeDef ret = HAL_OK;

    elog_i(TAG, "Board init");
    
    /* 初始化W25QXX */
    BSP_ERROR_CHECK_RETURN_ERR(bsp_w25qxx_init());
    
    /* 初始化AHT20 */
    BSP_ERROR_CHECK_RETURN_ERR(bsp_aht20_init());
    
    /* 初始化LED */
    BSP_ERROR_CHECK_RETURN_ERR(bsp_led_init());
    
    /* 初始化按键 */
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_init());
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_PRESS_DOWN, button_callback, NULL));       // 按下
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_PRESS_UP, button_callback, NULL));         // 释放
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_SINGLE_CLICK, button_callback, NULL));     // 单击
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_MULTIPLE_CLICK, button_callback, NULL));   // 连击
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_LONG_PRESS_START, button_callback, NULL)); // 长按开始
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_LONG_PRESS_HOLD, button_callback, NULL));  // 长按保持
    BSP_ERROR_CHECK_RETURN_ERR(bsp_btn_register_callback(BSP_BUTTON_USER, BUTTON_LONG_PRESS_UP, button_callback, NULL));    // 长按释放
    
    return ret;    
}
