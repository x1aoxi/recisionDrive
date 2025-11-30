/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_icm20608.c
 * @brief     driver icm20608 source file
 * @version   1.0.0
 * @author    Ahypnis
 * @date      2024-12-24
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/12/24  <td>1.0      <td>Ahypnis     <td>first upload
 * </table>
 */

#include "driver_icm20608.h"

/* 定义传感器量程的比例因子 */
static const float GYRO_SCALE_FACTOR[] = {
    131.0f,    /* ±250°/s */
    65.5f,     /* ±500°/s */
    32.8f,     /* ±1000°/s */
    16.4f      /* ±2000°/s */
};

static const float ACCEL_SCALE_FACTOR[] = {
    16384.0f,  /* ±2g */
    8192.0f,   /* ±4g */
    4096.0f,   /* ±8g */
    2048.0f    /* ±16g */
};

/**
 * @brief      获取芯片信息
 * @param[out] *info 指向icm20608信息结构体的指针
 * @return     状态码
 *            - 0 成功
 *            - 2 handle为NULL
 */
uint8_t icm20608_info(icm20608_info_t *info)
{
    if (info == NULL)
    {
        return 2;
    }
    
    memset(info, 0, sizeof(icm20608_info_t));
    strncpy(info->chip_name, "ICM20608", 32);
    strncpy(info->manufacturer_name, "TDK-InvenSense", 32);
    strncpy(info->interface, "IIC", 8);
    info->supply_voltage_min_v = 1.71f;
    info->supply_voltage_max_v = 3.45f;
    info->max_current_ma = 3.7f;
    info->temperature_min = -40.0f;
    info->temperature_max = +85.0f;
    info->driver_version = 0x0100;
    
    return 0;
}

/**
 * @brief     初始化ICM20608
 * @param[in] *handle 指向icm20608句柄结构的指针
 * @return    状态码
 *            - 0 成功
 *            - 1 IIC初始化失败
 *            - 2 handle为NULL
 *            - 3 链接函数为NULL
 *            - 4 ID无效
 */
uint8_t icm20608_init(icm20608_handle_t *handle)
{
    uint8_t res, id;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->ps_iic_init == NULL || handle->ps_iic_deinit == NULL || 
        handle->ps_iic_read == NULL || handle->ps_iic_write == NULL || 
        handle->ps_delay_ms == NULL)
    {
        return 3;
    }

    if (handle->ps_iic_init())
    {
        handle->ps_debug_print("icm20608: iic init failed.\n");
        return 1;
    }
    
    /* 读取WHO_AM_I寄存器 */
    res = handle->ps_iic_read(handle->iic_addr, ICM20608_REG_WHO_AM_I, &id, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: read who am i failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 检查设备ID */
    if (id != 0xAF)
    {
        handle->ps_debug_print("icm20608: id is invalid.\n");
        (void)handle->ps_iic_deinit();
        return 4;
    }
    
    /* 复位设备 */
    uint8_t reset_cmd = 0x80;
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_PWR_MGMT_1, &reset_cmd, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: write power management 1 failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    handle->ps_delay_ms(100);  /* 等待复位完成 */
    
    /* 唤醒设备 */
    uint8_t wake_cmd = 0x00;
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_PWR_MGMT_1, &wake_cmd, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: wake up device failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 使能所有轴的加速度计和陀螺仪 */
    uint8_t enable_cmd = 0x00;
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_PWR_MGMT_2, &enable_cmd, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: enable sensors failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 设置陀螺仪量程为±2000dps */
    uint8_t gyro_config = 0x03 << 3;  /* 0x00 << 3 = 0x00 */
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_GYRO_CONFIG, &gyro_config, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: set gyroscope range failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 设置加速度计量程为±8g */
    uint8_t accel_config = 0x03 << 3;  /* 0x00 << 3 = 0x00 */
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_ACCEL_CONFIG, &accel_config, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: set accelerometer range failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 配置DLPF，设置为41Hz */
    uint8_t dlpf_config = 0x03;
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_CONFIG, &dlpf_config, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: configure DLPF failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 设置采样率为1kHz */
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_ACCEL_CONFIG2, &dlpf_config, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: set sample rate failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    handle->inited = 1;
    
    return 0;
}

/**
 * @brief     关闭ICM20608
 * @param[in] *handle 指向icm20608句柄结构的指针
 * @return    状态码
 *            - 0 成功
 *            - 1 IIC反初始化失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_deinit(icm20608_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 进入睡眠模式 */
    uint8_t sleep_cmd = 0x40;
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_PWR_MGMT_1, &sleep_cmd, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: write power management 1 failed.\n");
        return 1;
    }
    
    /* 反初始化IIC */
    if (handle->ps_iic_deinit())
    {
        handle->ps_debug_print("icm20608: iic deinit failed.\n");
        return 1;
    }
    
    handle->inited = 0;
    
    return 0;
}

/**
 * @brief      读取温度数据
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *raw 指向原始温度数据缓存的指针
 * @param[out] *degrees 指向转换后温度数据缓存的指针
 * @return     状态码
 *            - 0 成功
 *            - 1 读取温度失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_read_temperature(icm20608_handle_t *handle, int16_t *raw, float *degrees)
{
    uint8_t buf[2];
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取温度寄存器 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_TEMP_OUT_H, buf, 2))
    {
        handle->ps_debug_print("icm20608: read temperature failed.\n");
        return 1;
    }
    
    *raw = (int16_t)((buf[0] << 8) | buf[1]);
    *degrees = (float)(*raw) / 326.8f + 25.0f;
    
    return 0;
}

/**
 * @brief      读取陀螺仪数据
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *raw 指向原始陀螺仪数据缓存的指针
 * @param[out] *dps 指向转换后陀螺仪数据缓存的指针
 * @return     状态码
 *            - 0 成功
 *            - 1 读取陀螺仪失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
#include "elog.h"
uint8_t icm20608_read_gyroscope(icm20608_handle_t *handle, int16_t raw[3], float dps[3])
{
    uint8_t buf[6];
    uint8_t range;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取陀螺仪数据 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_GYRO_XOUT_H, buf, 6))
    {
        handle->ps_debug_print("icm20608: read gyroscope failed.\n");
        return 1;
    }
    
    /* 获取当前量程设置 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_GYRO_CONFIG, &range, 1))
    {
        handle->ps_debug_print("icm20608: read gyroscope config failed.\n");
        return 1;
    }
    range = (range >> 3) & 0x03;
    
    elog_i("debug_G", "%d", range);
    
    /* 转换原始数据 */
    raw[0] = (int16_t)((buf[0] << 8) | buf[1]);
    raw[1] = (int16_t)((buf[2] << 8) | buf[3]);
    raw[2] = (int16_t)((buf[4] << 8) | buf[5]);
    
    /* 转换为角速度 */
    dps[0] = (float)raw[0] / GYRO_SCALE_FACTOR[range];
    dps[1] = (float)raw[1] / GYRO_SCALE_FACTOR[range];
    dps[2] = (float)raw[2] / GYRO_SCALE_FACTOR[range];
    
    return 0;
}

/**
 * @brief      读取加速度计数据
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *raw 指向原始加速度计数据缓存的指针
 * @param[out] *g 指向转换后加速度计数据缓存的指针
 * @return     状态码
 *            - 0 成功
 *            - 1 读取加速度计失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */

uint8_t icm20608_read_accelerometer(icm20608_handle_t *handle, int16_t raw[3], float g[3])
{
    uint8_t buf[6];
    uint8_t range;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取加速度计数据 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_ACCEL_XOUT_H, buf, 6))
    {
        handle->ps_debug_print("icm20608: read accelerometer failed.\n");
        return 1;
    }
    
    /* 获取当前量程设置 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_ACCEL_CONFIG, &range, 1))
    {
        handle->ps_debug_print("icm20608: read accelerometer config failed.\n");
        return 1;
    }
    range = (range >> 3) & 0x03;
    elog_i("debug_A", "%d", range);
    
    /* 转换原始数据 */
    raw[0] = (int16_t)((buf[0] << 8) | buf[1]);
    raw[1] = (int16_t)((buf[2] << 8) | buf[3]);
    raw[2] = (int16_t)((buf[4] << 8) | buf[5]);
    
    /* 转换为重力加速度 */
    g[0] = (float)raw[0] / ACCEL_SCALE_FACTOR[range];
    g[1] = (float)raw[1] / ACCEL_SCALE_FACTOR[range];
    g[2] = (float)raw[2] / ACCEL_SCALE_FACTOR[range];
    
    return 0;
}


