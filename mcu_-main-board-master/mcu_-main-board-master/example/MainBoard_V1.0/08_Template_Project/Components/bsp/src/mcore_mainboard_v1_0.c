#include "mcore_mainboard_v1_0.h"

#include "button.h"
#include "button_gpio.h"

#include "led.h"
#include "led_gpio.h"
#include "led_pwm.h"

#include "driver_aht20.h"
#include "driver_aht20_interface.h"

#include "driver_w25qxx.h"
#include "driver_w25qxx_interface.h"

#include "elog.h"

static const char *TAG = "Mcore_MainBoard-v1.0";

/* RGB LED配置 */
static const led_config_t bsp_rgb_led_config[BSP_RGB_LED_NUM] = {
    {   // RGB LED - 使用三个独立的PWM通道
        .type = LED_TYPE_PWM,
        .led_union.pwm_led_config = {
            .htim = &BSP_RGB_TIM,
            .channel = BSP_RGB_R_CHANNEL | BSP_RGB_G_CHANNEL | BSP_RGB_B_CHANNEL,  // 使用三个通道
            .period = BSP_RGB_TIM_PERIOD
        }
    }
};

/* 按键配置 */
static const button_config_t bsp_button_config[BSP_BUTTON_NUM] = {
    {
        .type = BUTTON_TYPE_GPIO,     // 按键类型为GPIO
        .button_union.gpio_button_config = {
            .gpio_port = BSP_BUTTON_USER_PORT,
            .gpio_pin = BSP_BUTTON_USER_IO,
            .active_level = 0
        },
        .long_press_time = 600,      // 长按时间600ms
        .short_press_time = 200       // 短按时间200ms
    }
};

HAL_StatusTypeDef bsp_rgb_led_init(led_handle_t led_array[], int *led_cnt, int array_size)
{
    HAL_StatusTypeDef ret = HAL_OK;

    if ((array_size < BSP_RGB_LED_NUM) || (led_array == NULL)) {
        return HAL_ERROR;
    }

    if (led_cnt) {
        *led_cnt = 0;
    }
    
    /* 初始化RGB LED */
    led_array[BSP_RGB_LED] = led_create(&bsp_rgb_led_config[BSP_RGB_LED]);
    if (led_array[BSP_RGB_LED] == NULL) {
        ret = HAL_ERROR;
    } else if (led_cnt) {
        (*led_cnt)++;
    }
    
    /* 初始化定时器 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // 启动PWM通道1
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);  // 启动PWM通道2
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);  // 启动PWM通道3
    
    /* 如果初始化失败，释放资源 */
    if (ret != HAL_OK) {
        if (led_array[BSP_RGB_LED] != NULL) {
            led_delete(led_array[BSP_RGB_LED]);
            led_array[BSP_RGB_LED] = NULL;
        }
        if (led_cnt) {
            *led_cnt = 0;
        }
    }
    
    return ret;
}

HAL_StatusTypeDef bsp_rgb_led_set_color(led_handle_t led_array[], uint8_t r, uint8_t g, uint8_t b)
{
    HAL_StatusTypeDef ret = HAL_OK;
    led_pwm_t *led;

    if (led_array == NULL || led_array[BSP_RGB_LED] == NULL) {
        return HAL_ERROR;
    }

    led = (led_pwm_t *)led_array[BSP_RGB_LED];

    /* 设置三个通道的PWM值（注意：因为是共阳极，所以用period-value） */
    uint32_t r_value = ((255 - r) * led->config.period) / 255;
    uint32_t g_value = ((255 - g) * led->config.period) / 255;
    uint32_t b_value = ((255 - b) * led->config.period) / 255;

    /* 设置PWM比较值 */
    __HAL_TIM_SET_COMPARE(led->config.htim, BSP_RGB_R_CHANNEL, r_value);
    __HAL_TIM_SET_COMPARE(led->config.htim, BSP_RGB_G_CHANNEL, g_value);
    __HAL_TIM_SET_COMPARE(led->config.htim, BSP_RGB_B_CHANNEL, b_value);

    return ret;
}

HAL_StatusTypeDef bsp_iot_button_create(button_handle_t btn_array[], int *btn_cnt, int btn_array_size)
{
    HAL_StatusTypeDef ret = HAL_OK;
    if ((btn_array_size < BSP_BUTTON_NUM) || (btn_array == NULL)) {
        elog_e(TAG, "Invalid button array parameters");
        return HAL_ERROR;
    }

    if (btn_cnt) {
        *btn_cnt = 0;
    }

    for (int i = 0; i < BSP_BUTTON_NUM; i++) {
        btn_array[i] = button_create(&bsp_button_config[i]);
        if (btn_array[i] == NULL) {
            elog_e(TAG, "Failed to create button %d", i);
            ret = HAL_ERROR;
            // 清理已创建的按键
            for (int j = 0; j < i; j++) {
                if (btn_array[j] != NULL) {
                    button_delete(btn_array[j]);
                    btn_array[j] = NULL;
                }
            }
            break;
        }
        if (btn_cnt) {
            (*btn_cnt)++;
        }
        elog_i(TAG, "Button %d created successfully", i);
    }
    return ret;
}

/**
 * @brief  创建并初始化AHT20温湿度传感器
 * @param  handle: AHT20句柄指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 创建成功
 *         - HAL_ERROR: 创建失败
 */
HAL_StatusTypeDef bsp_asair_aht20_init(aht20_handle_t *handle)
{
    HAL_StatusTypeDef ret = HAL_OK;
    
    if (handle == NULL) {
        elog_e(TAG, "Invalid AHT20 handle");
        return HAL_ERROR;
    }
    
    /* 初始化AHT20 */
    if (aht20_init(handle) != 0) {
        elog_e(TAG, "Failed to initialize AHT20");
        handle = NULL;
        return HAL_ERROR;
    }
    
    elog_i(TAG, "AHT20 initialized successfully");
    return ret;
}

/**
 * @brief  读取AHT20温湿度数据
 * @param  handle: AHT20句柄
 * @param  temperature: 温度值指针
 * @param  humidity: 湿度值指针
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef bsp_asair_aht20_read(aht20_handle_t *handle, float *temperature, uint8_t *humidity)
{
    uint32_t temperature_raw;
    uint32_t humidity_raw;
    
    if (handle == NULL || temperature == NULL || humidity == NULL) {
        return HAL_ERROR;
    }
    
    if (aht20_read_temperature_humidity(handle, &temperature_raw, temperature, 
                                      &humidity_raw, humidity) != 0) {
        return HAL_ERROR;
    }
    
    return HAL_OK;
}

/**
 * @brief  删除AHT20设备
 * @param  handle: AHT20句柄指针
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef bsp_asair_aht20_deinit(aht20_handle_t *handle)
{
    if (handle == NULL) {
        return HAL_ERROR;
    }
    
    if (aht20_deinit(handle) != 0) {
        return HAL_ERROR;
    }
    
    handle = NULL;
    
    return HAL_OK;
}

/**
 * @brief  创建并初始化W25QXX Flash存储器
 * @param  handle: W25QXX句柄指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 初始化成功
 *         - HAL_ERROR: 初始化失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_init(w25qxx_handle_t *handle)
{
    HAL_StatusTypeDef ret = HAL_OK;
    
    /* 参数检查 */
    if (handle == NULL) {
        elog_e(TAG, "Invalid W25QXX handle");
        return HAL_ERROR;
    }
    
    /* 设置芯片类型 */
    if (w25qxx_set_type(handle, W25Q32) != 0) {
        elog_e(TAG, "W25QXX set type failed");
        return HAL_ERROR;
    }

    /* 设置接口类型 */
    if (w25qxx_set_interface(handle, W25QXX_INTERFACE_SPI) != 0) {
        elog_e(TAG, "W25QXX set interface failed");
        return HAL_ERROR;
    }

    /* 设置双/四线SPI模式 */
    if (w25qxx_set_dual_quad_spi(handle, W25QXX_BOOL_FALSE) != 0) {
        elog_e(TAG, "W25QXX set dual quad spi failed");
        return HAL_ERROR;
    }
    
    /* 初始化W25QXX */
    if (w25qxx_init(handle) != 0) {
        elog_e(TAG, "Failed to initialize W25QXX");
        return HAL_ERROR;
    }
    
    /* 验证设备是否正常 */
    uint8_t manufacturer_id;
    uint8_t device_id;
    if (w25qxx_get_manufacturer_device_id(handle, &manufacturer_id, &device_id) != 0) {
        elog_e(TAG, "Failed to get W25QXX device ID");
        w25qxx_deinit(handle);
        return HAL_ERROR;
    }
    
    elog_i(TAG, "W25QXX initialized successfully (MID:0x%02X, DID:0x%02X)", 
           manufacturer_id, device_id);
    return ret;
}

/**
 * @brief  读取W25QXX数据
 * @param  addr: 读取地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 读取成功
 *         - HAL_ERROR: 读取失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    /* 参数检查 */
    if (data == NULL) {
        elog_e(TAG, "Invalid parameters for W25QXX read");
        return HAL_ERROR;
    }

    /* 读取数据 */
    if (w25qxx_read(handle, addr, data, len) != 0) {
        elog_e(TAG, "Failed to read W25QXX data");
        return HAL_ERROR;
    }

    elog_d(TAG, "W25QXX read %d bytes from address 0x%08X", len, addr);
    return HAL_OK;
}

/**
 * @brief  写入W25QXX数据
 * @param  addr: 写入地址
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 写入成功
 *         - HAL_ERROR: 写入失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_write(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    /* 参数检查 */
    if (data == NULL) {
        elog_e(TAG, "Invalid parameters for W25QXX write");
        return HAL_ERROR;
    }

    /* 写入数据 */
    if (w25qxx_write(handle, addr, data, len) != 0) {
        elog_e(TAG, "Failed to write W25QXX data");
        return HAL_ERROR;
    }

    elog_d(TAG, "W25QXX wrote %d bytes to address 0x%08X", len, addr);
    return HAL_OK;
}

/**
 * @brief  反初始化W25QXX设备
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 反初始化成功
 *         - HAL_ERROR: 反初始化失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_deinit(w25qxx_handle_t *handle)
{
    if (w25qxx_deinit(handle) != 0) {
        elog_e(TAG, "Failed to deinitialize W25QXX");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX deinitialized successfully");
    return HAL_OK;
}

/**
 * @brief  W25QXX进入掉电模式
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 进入掉电模式成功
 *         - HAL_ERROR: 进入掉电模式失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_power_down(w25qxx_handle_t *handle)
{
    if (w25qxx_power_down(handle) != 0) {
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
HAL_StatusTypeDef bsp_zetta_w25qxx_wake_up(w25qxx_handle_t *handle)
{
    if (w25qxx_release_power_down(handle) != 0) {
        elog_e(TAG, "Failed to wake up from power down mode");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX woke up from power down mode");
    return HAL_OK;
}

/**
 * @brief  擦除整个W25QXX芯片
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 擦除成功
 *         - HAL_ERROR: 擦除失败
 * @note   此操作将擦除芯片上的所有数据，请谨慎使用
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_chip_erase(w25qxx_handle_t *handle)
{
    if (w25qxx_chip_erase(handle) != 0) {
        elog_e(TAG, "Failed to erase chip");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX chip erase completed");
    return HAL_OK;
}

/**
 * @brief  获取W25QXX制造商和设备ID
 * @param  manufacturer: 制造商ID存储指针
 * @param  device_id: 设备ID存储指针
 * @return HAL_StatusTypeDef: 
 *         - HAL_OK: 获取成功
 *         - HAL_ERROR: 获取失败
 */
HAL_StatusTypeDef bsp_zetta_w25qxx_get_id(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id)
{
    /* 参数检查 */
    if (manufacturer == NULL || device_id == NULL) {
        elog_e(TAG, "Invalid parameters for getting device ID");
        return HAL_ERROR;
    }

    if (w25qxx_get_manufacturer_device_id(handle, manufacturer, device_id) != 0) {
        elog_e(TAG, "Failed to get manufacturer and device ID");
        return HAL_ERROR;
    }

    elog_i(TAG, "W25QXX manufacturer ID: 0x%02X, device ID: 0x%02X", *manufacturer, *device_id);
    return HAL_OK;
}
