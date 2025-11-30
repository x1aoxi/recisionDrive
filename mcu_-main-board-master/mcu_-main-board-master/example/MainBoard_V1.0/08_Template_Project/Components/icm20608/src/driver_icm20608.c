#include "driver_icm20608.h"

#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "mltypes.h"

#define q30  1073741824.0f
#define q16  65536.0f

#define DEFAULT_ICM_HZ    (100)

static signed char gyro_orientation[9] = {1, 0, 0,
                                        0, 1, 0,
                                        0, 0, 1};

unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";
                                        
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
    
    /* 设置陀螺仪量程为±250dps */
    uint8_t gyro_config = 0x00;  /* 0x00 << 3 = 0x00 */
    res = handle->ps_iic_write(handle->iic_addr, ICM20608_REG_GYRO_CONFIG, &gyro_config, 1);
    if (res)
    {
        handle->ps_debug_print("icm20608: set gyroscope range failed.\n");
        (void)handle->ps_iic_deinit();
        return 1;
    }
    
    /* 设置加速度计量程为±2g */
    uint8_t accel_config = 0x00;  /* 0x00 << 3 = 0x00 */
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

/**
 * @brief      设置传感器使能状态
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[in]  sensors 要使能的传感器位掩码
 * @return     状态码
 *            - 0 成功
 *            - 1 设置失败
 *            - 2 handle为NULL
 */
uint8_t icm20608_set_sensors(icm20608_handle_t *handle, uint8_t sensors)
{
    uint8_t pwr_mgmt_2 = 0;
    
    if (handle == NULL)
    {
        return 2;
    }
    
    /* 配置电源管理寄存器2 */
    if (sensors & INV_XYZ_GYRO)
    {
        pwr_mgmt_2 &= ~(0x07 << 3);  /* 使能陀螺仪 */
    }
    else
    {
        pwr_mgmt_2 |= (0x07 << 3);   /* 禁用陀螺仪 */
    }
    
    if (sensors & INV_XYZ_ACCEL)
    {
        pwr_mgmt_2 &= ~0x07;         /* 使能加速度计 */
    }
    else
    {
        pwr_mgmt_2 |= 0x07;          /* 禁用加速度计 */
    }
    
    if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_PWR_MGMT_2, &pwr_mgmt_2, 1))
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      配置FIFO
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[in]  sensors 要配置FIFO的传感器位掩码
 * @return     状态码
 *            - 0 成功
 *            - 1 配置失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_configure_fifo(icm20608_handle_t *handle, uint8_t sensors)
{
    uint8_t user_ctrl;
    uint8_t fifo_en = 0;
    uint8_t int_enable = 0;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 检查传感器是否已启用 */
    uint8_t pwr_mgmt_2;
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_PWR_MGMT_2, &pwr_mgmt_2, 1))
    {
        return 1;
    }
    
    /* 检查陀螺仪和加速度计是否已启用 */
    uint8_t enabled_sensors = 0;
    if ((pwr_mgmt_2 & (0x07 << 3)) == 0) /* 陀螺仪已启用 */
    {
        enabled_sensors |= INV_XYZ_GYRO;
    }
    if ((pwr_mgmt_2 & 0x07) == 0) /* 加速度计已启用 */
    {
        enabled_sensors |= INV_XYZ_ACCEL;
    }
    
    /* 只使能已启用的传感器 */
    fifo_en = sensors & enabled_sensors;
    
    /* 如果请求的传感器未全部启用，返回警告 */
    if (fifo_en != sensors)
    {
        handle->ps_debug_print("icm20608: some sensors are not enabled.\n");
    }
    
    /* 配置中断 */
    if (sensors || (pwr_mgmt_2 & 0x07) == 0) /* 如果有传感器使能或处于加速度计低功耗模式 */
    {
        int_enable = 0x01; /* 使能数据就绪中断 */
    }
    
    if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_INT_ENABLE, &int_enable, 1))
    {
        return 1;
    }
    
    if (sensors)
    {
        /* 复位FIFO */
        if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
        {
            return 1;
        }
        
        user_ctrl |= 0x04; /* FIFO_RST */
        if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
        {
            return 1;
        }
        
        /* 使能FIFO */
        user_ctrl |= 0x40; /* FIFO_EN */
        if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
        {
            return 1;
        }
        
        /* 配置FIFO使能寄存器 */
        if (fifo_en & INV_XYZ_GYRO)
        {
            fifo_en = 0x70; /* 使能XYZ轴陀螺仪 */
        }
        if (fifo_en & INV_XYZ_ACCEL)
        {
            fifo_en |= 0x08; /* 使能加速度计 */
        }
        
        if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_FIFO_EN, &fifo_en, 1))
        {
            return 1;
        }
    }
    else
    {
        /* 禁用FIFO */
        if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
        {
            return 1;
        }
        
        user_ctrl &= ~0x40; /* 清除FIFO_EN位 */
        if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
        {
            return 1;
        }
        
        /* 清除FIFO使能寄存器 */
        fifo_en = 0;
        if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_FIFO_EN, &fifo_en, 1))
        {
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief      设置采样率
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[in]  rate 采样率(Hz)
 * @return     状态码
 *            - 0 成功
 *            - 1 设置失败
 *            - 2 handle为NULL
 */
uint8_t icm20608_set_sample_rate(icm20608_handle_t *handle, uint16_t rate)
{
    uint8_t div;
    
    if (handle == NULL)
    {
        return 2;
    }
    
    if (rate < 4)
    {
        rate = 4;
    }
    else if (rate > 1000)
    {
        rate = 1000;
    }
    
    div = (uint8_t)(1000 / rate - 1);
    if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_SMPLRT_DIV, &div, 1))
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      读取FIFO数据
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *gyro 陀螺仪数据
 * @param[out] *accel 加速度计数据
 * @param[out] *sensors 数据类型标志
 * @param[out] *more FIFO中是否还有更多数据
 * @return     状态码
 *            - 0 成功
 *            - 1 读取失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_read_fifo(icm20608_handle_t *handle, int16_t *gyro, int16_t *accel, uint8_t *sensors, uint8_t *more)
{
    uint8_t data[12];  /* 最大数据包大小：陀螺仪(6) + 加速度计(6) */
    uint8_t packet_size = 0;
    uint16_t fifo_count;
    uint8_t index = 0;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    *sensors = 0;
    
    /* 计算数据包大小 */
    packet_size = 12;  /* 我们总是读取所有数据 */
    
    /* 读取FIFO计数 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_FIFO_COUNTH, data, 2))
    {
        handle->ps_debug_print("icm20608: read fifo count failed.\n");
        return 1;
    }
    
    fifo_count = (uint16_t)((data[0] << 8) | data[1]);
    if (fifo_count < packet_size)
    {
        handle->ps_debug_print("icm20608: not enough data in fifo.\n");
        return 1;
    }
    
    /* 检查FIFO溢出 */
    if (fifo_count > 512)
    {
        /* FIFO已满50%，检查溢出位 */
        if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_INT_STATUS, data, 1))
        {
            handle->ps_debug_print("icm20608: read int status failed.\n");
            return 1;
        }
        
        if (data[0] & 0x10)  /* FIFO_OVERFLOW_INT */
        {
            /* 复位FIFO */
            uint8_t user_ctrl;
            if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
            {
                return 1;
            }
            
            user_ctrl |= 0x04;  /* FIFO_RST */
            if (handle->ps_iic_write(handle->iic_addr, ICM20608_REG_USER_CTRL, &user_ctrl, 1))
            {
                return 1;
            }
            
            handle->ps_debug_print("icm20608: fifo overflow, reset fifo.\n");
            return 1;
        }
    }
    
    /* 读取FIFO数据 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_FIFO_R_W, data, packet_size))
    {
        handle->ps_debug_print("icm20608: read fifo data failed.\n");
        return 1;
    }
    
    *more = fifo_count / packet_size - 1;
    
    /* 解析加速度计数据 */
    accel[0] = (int16_t)((data[index+0] << 8) | data[index+1]);
    accel[1] = (int16_t)((data[index+2] << 8) | data[index+3]);
    accel[2] = (int16_t)((data[index+4] << 8) | data[index+5]);
    *sensors |= INV_XYZ_ACCEL;
    index += 6;
    
    /* 解析陀螺仪数据 */
    gyro[0] = (int16_t)((data[index+0] << 8) | data[index+1]);
    gyro[1] = (int16_t)((data[index+2] << 8) | data[index+3]);
    gyro[2] = (int16_t)((data[index+4] << 8) | data[index+5]);
    *sensors |= INV_XYZ_GYRO;
    
    return 0;
}

/**
 * @brief      获取采样率
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *rate 采样率(Hz)
 * @return     状态码
 *            - 0 成功
 *            - 1 读取失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_get_sample_rate(icm20608_handle_t *handle, uint16_t *rate)
{
    uint8_t div;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取分频值 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_SMPLRT_DIV, &div, 1))
    {
        handle->ps_debug_print("icm20608: read sample rate div failed.\n");
        return 1;
    }
    
    /* 计算采样率 */
    *rate = 1000 / (1 + div);
    
    return 0;
}

/**
 * @brief      获取陀螺仪量程
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *fsr 量程(dps)
 * @return     状态码
 *            - 0 成功
 *            - 1 读取失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_get_gyro_fsr(icm20608_handle_t *handle, uint16_t *fsr)
{
    uint8_t config;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取配置 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_GYRO_CONFIG, &config, 1))
    {
        handle->ps_debug_print("icm20608: read gyro config failed.\n");
        return 1;
    }
    
    /* 解析量程 */
    switch ((config >> 3) & 0x03)
    {
        case 0:
            *fsr = 250;
            break;
        case 1:
            *fsr = 500;
            break;
        case 2:
            *fsr = 1000;
            break;
        case 3:
            *fsr = 2000;
            break;
        default:
            *fsr = 0;
            break;
    }
    
    return 0;
}

/**
 * @brief      获取加速度计量程
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *fsr 量程(g)
 * @return     状态码
 *            - 0 成功
 *            - 1 读取失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_get_accel_fsr(icm20608_handle_t *handle, uint8_t *fsr)
{
    uint8_t config;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取配置 */
    if (handle->ps_iic_read(handle->iic_addr, ICM20608_REG_ACCEL_CONFIG, &config, 1))
    {
        handle->ps_debug_print("icm20608: read accel config failed.\n");
        return 1;
    }
    
    /* 解析量程 */
    switch ((config >> 3) & 0x03)
    {
        case 0:
            *fsr = 2;
            break;
        case 1:
            *fsr = 4;
            break;
        case 2:
            *fsr = 8;
            break;
        case 3:
            *fsr = 16;
            break;
        default:
            *fsr = 0;
            break;
    }
    
    return 0;
}

/**
 * @brief      初始化MPL库
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @return     状态码
 *            - 0 成功
 *            - 1 初始化失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_mpl_init(icm20608_handle_t *handle)
{
    uint8_t res;
    unsigned char accel_fsr;
    unsigned short gyro_rate, gyro_fsr;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 初始化MPL */
    res = inv_init_mpl();
    if (res)
    {
        handle->ps_debug_print("icm20608: mpl init failed.\n");
        return 1;
    }
    
    /* 配置MPL */
    inv_enable_quaternion();
    inv_enable_9x_sensor_fusion();
    inv_enable_fast_nomot();
    inv_enable_gyro_tc();
    inv_enable_eMPL_outputs();
    
    /* 启动MPL */
    res = inv_start_mpl();
    if (res)
    {
        handle->ps_debug_print("icm20608: mpl start failed.\n");
        return 1;
    }
    
    /* 配置传感器 */
    res = icm20608_set_sensors(handle, INV_XYZ_GYRO | INV_XYZ_ACCEL);
    if (res)
    {
        handle->ps_debug_print("icm20608: set sensors failed.\n");
        return 1;
    }
    
    /* 配置FIFO */
    res = icm20608_configure_fifo(handle, INV_XYZ_GYRO | INV_XYZ_ACCEL);
    if (res)
    {
        handle->ps_debug_print("icm20608: configure fifo failed.\n");
        return 1;
    }
    
    /* 设置采样率 */
    res = icm20608_set_sample_rate(handle, DEFAULT_ICM_HZ);
    if (res)
    {
        handle->ps_debug_print("icm20608: set sample rate failed.\n");
        return 1;
    }
    
    /* 获取当前配置 */
    icm20608_get_sample_rate(handle, &gyro_rate);
    icm20608_get_gyro_fsr(handle, &gyro_fsr);
    icm20608_get_accel_fsr(handle, &accel_fsr);
    
    /* 配置MPL采样率和方向 */
    inv_set_gyro_sample_rate(1000000L / gyro_rate);
    inv_set_accel_sample_rate(1000000L / gyro_rate);
    inv_set_gyro_orientation_and_scale(
        inv_orientation_matrix_to_scalar(gyro_orientation),
        (long)gyro_fsr<<15);
    inv_set_accel_orientation_and_scale(
        inv_orientation_matrix_to_scalar(gyro_orientation),
        (long)accel_fsr<<15);
    
    /* 等待传感器稳定 */
    handle->ps_delay_ms(50);
    
    return 0;
}

/**
 * @brief      获取欧拉角
 * @param[in]  *handle 指向icm20608句柄结构的指针
 * @param[out] *pitch 俯仰角(°) 范围:-90.0° <---> +90.0°
 * @param[out] *roll 横滚角(°) 范围:-180.0°<---> +180.0°
 * @param[out] *yaw 航向角(°) 范围:-180.0°<---> +180.0°
 * @return     状态码
 *            - 0 成功
 *            - 1 读取失败
 *            - 2 handle为NULL
 *            - 3 handle未初始化
 */
uint8_t icm20608_mpl_get_euler(icm20608_handle_t *handle, float *pitch, float *roll, float *yaw)
{
    static inv_time_t timestamp = 0;
    short gyro[3], accel_short[3];
    unsigned char more, sensors;
    long accel[3], temperature;
    long data[9];
    int8_t accuracy;
    
    if (handle == NULL)
    {
        return 2;
    }
    if (handle->inited != 1)
    {
        return 3;
    }
    
    /* 读取FIFO数据 */
    if (icm20608_read_fifo(handle, gyro, accel_short, &sensors, &more))
    {
        handle->ps_debug_print("icm20608: read fifo failed.\n");
        return 1;
    }
    
    /* 处理陀螺仪数据 */
    if (sensors & INV_XYZ_GYRO)
    {
        /* 转换为实际物理单位并打印 */
        uint16_t gyro_fsr;
        icm20608_get_gyro_fsr(handle, &gyro_fsr);
        float scale = GYRO_SCALE_FACTOR[0];  /* 默认使用±250dps的比例 */
        switch(gyro_fsr) {
            case 500: scale = GYRO_SCALE_FACTOR[1]; break;
            case 1000: scale = GYRO_SCALE_FACTOR[2]; break;
            case 2000: scale = GYRO_SCALE_FACTOR[3]; break;
            default: break;
        }
        
        inv_build_gyro(gyro, timestamp);
        
        /* 读取温度数据 */
        int16_t raw_temp;
        float temp;
        if (icm20608_read_temperature(handle, &raw_temp, &temp) == 0)
        {
            temperature = (long)(temp * 65536.0f);
            inv_build_temp(temperature, timestamp);
        }
    }
    else
    {
        handle->ps_debug_print("icm20608: no gyro data.\n");
        return 1;
    }
    
    /* 处理加速度计数据 */
    if (sensors & INV_XYZ_ACCEL)
    {
        /* 转换为实际物理单位并打印 */
        uint8_t accel_fsr;
        icm20608_get_accel_fsr(handle, &accel_fsr);
        float scale = ACCEL_SCALE_FACTOR[0];  /* 默认使用±2g的比例 */
        switch(accel_fsr) {
            case 4: scale = ACCEL_SCALE_FACTOR[1]; break;
            case 8: scale = ACCEL_SCALE_FACTOR[2]; break;
            case 16: scale = ACCEL_SCALE_FACTOR[3]; break;
            default: break;
        }
        
        accel[0] = (long)accel_short[0];
        accel[1] = (long)accel_short[1];
        accel[2] = (long)accel_short[2];
        inv_build_accel(accel, 0, timestamp);
    }
    else
    {
        handle->ps_debug_print("icm20608: no accel data.\n");
        return 1;
    }
    
    /* 执行MPL算法 */
    inv_execute_on_data();
    
    /* 获取欧拉角数据 */
    if (inv_get_sensor_type_euler(data, &accuracy, &timestamp))
    {
        /* 转换数据 */
        *roll = data[0] / (float)q16;
        *pitch = -data[1] / (float)q16;
        *yaw = -data[2] / (float)q16;
    }
    else
    {
        handle->ps_debug_print("icm20608: get euler data failed.\n");
        return 1;
    }
    
    return 0;
}


