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
 * @file      driver_icm20608.h
 * @brief     driver icm20608 header file
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

#ifndef DRIVER_ICM20608_H
#define DRIVER_ICM20608_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup icm20608_driver icm20608 driver function
 * @brief    icm20608 driver modules
 * @{
 */

/**
 * @addtogroup icm20608_basic_driver
 * @{
 */

/**
 * @brief icm20608 interface enumeration definition
 */
typedef enum
{
    ICM20608_INTERFACE_IIC = 0x00,        /**< iic interface function */
} icm20608_interface_t;

/**
 * @brief icm20608 address enumeration definition
 */
typedef enum  
{
    ICM20608_ADDRESS_AD0_LOW  = 0x68,        /**< AD0 pin set LOW */
    ICM20608_ADDRESS_AD0_HIGH = 0x69,        /**< AD0 pin set HIGH */
} icm20608_address_t;

/**
 * @brief icm20608 bool enumeration definition
 */
typedef enum  
{
    ICM20608_BOOL_FALSE = 0x00,        /**< disable function */
    ICM20608_BOOL_TRUE  = 0x01,        /**< enable function */
} icm20608_bool_t;

/**
 * @brief icm20608 gyroscope range enumeration definition
 */
typedef enum  
{
    ICM20608_GYROSCOPE_RANGE_250DPS  = 0x00,        /**< ±250 dps */
    ICM20608_GYROSCOPE_RANGE_500DPS  = 0x01,        /**< ±500 dps */
    ICM20608_GYROSCOPE_RANGE_1000DPS = 0x02,        /**< ±1000 dps */
    ICM20608_GYROSCOPE_RANGE_2000DPS = 0x03,        /**< ±2000 dps */
} icm20608_gyroscope_range_t;

/**
 * @brief icm20608 accelerometer range enumeration definition
 */
typedef enum  
{
    ICM20608_ACCELEROMETER_RANGE_2G  = 0x00,        /**< ±2 g */
    ICM20608_ACCELEROMETER_RANGE_4G  = 0x01,        /**< ±4 g */
    ICM20608_ACCELEROMETER_RANGE_8G  = 0x02,        /**< ±8 g */
    ICM20608_ACCELEROMETER_RANGE_16G = 0x03,        /**< ±16 g */
} icm20608_accelerometer_range_t;

/**
 * @brief icm20608 low pass filter enumeration definition
 */
typedef enum
{
    ICM20608_LOW_PASS_FILTER_0 = 0x00,        /**< 250Hz */
    ICM20608_LOW_PASS_FILTER_1 = 0x01,        /**< 184Hz */
    ICM20608_LOW_PASS_FILTER_2 = 0x02,        /**< 92Hz */
    ICM20608_LOW_PASS_FILTER_3 = 0x03,        /**< 41Hz */
    ICM20608_LOW_PASS_FILTER_4 = 0x04,        /**< 20Hz */
    ICM20608_LOW_PASS_FILTER_5 = 0x05,        /**< 10Hz */
    ICM20608_LOW_PASS_FILTER_6 = 0x06,        /**< 5Hz */
} icm20608_low_pass_filter_t;

/**
 * @brief icm20608 fifo enumeration definition
 */
typedef enum  
{
    ICM20608_FIFO_TEMP  = 0x07,        /**< temperature */
    ICM20608_FIFO_XG    = 0x06,        /**< gyroscope x */
    ICM20608_FIFO_YG    = 0x05,        /**< gyroscope y */
    ICM20608_FIFO_ZG    = 0x04,        /**< gyroscope z */
    ICM20608_FIFO_ACCEL = 0x03,        /**< accelerometer */
} icm20608_fifo_t;

/**
 * @brief icm20608 interrupt type enumeration definition
 */
typedef enum
{
    ICM20608_INTERRUPT_MOTION        = 0x00,        /**< motion interrupt */
    ICM20608_INTERRUPT_FIFO_OVERFLOW = 0x01,        /**< fifo overflow interrupt */
    ICM20608_INTERRUPT_FSYNC_INT     = 0x02,        /**< fsync interrupt */
    ICM20608_INTERRUPT_DATA_READY    = 0x03,        /**< data ready interrupt */
} icm20608_interrupt_type_t;

/* ICM20608 register address definition */
#define ICM20608_REG_SELF_TEST_X_GYRO   0x00        /**< gyro self test x */
#define ICM20608_REG_SELF_TEST_Y_GYRO   0x01        /**< gyro self test y */
#define ICM20608_REG_SELF_TEST_Z_GYRO   0x02        /**< gyro self test z */
#define ICM20608_REG_SELF_TEST_X_ACCEL  0x0D        /**< accel self test x */
#define ICM20608_REG_SELF_TEST_Y_ACCEL  0x0E        /**< accel self test y */
#define ICM20608_REG_SELF_TEST_Z_ACCEL  0x0F        /**< accel self test z */
#define ICM20608_REG_XG_OFFS_USRH       0x13        /**< gyro offset x high */
#define ICM20608_REG_XG_OFFS_USRL       0x14        /**< gyro offset x low */
#define ICM20608_REG_YG_OFFS_USRH       0x15        /**< gyro offset y high */
#define ICM20608_REG_YG_OFFS_USRL       0x16        /**< gyro offset y low */
#define ICM20608_REG_ZG_OFFS_USRH       0x17        /**< gyro offset z high */
#define ICM20608_REG_ZG_OFFS_USRL       0x18        /**< gyro offset z low */
#define ICM20608_REG_SMPLRT_DIV         0x19        /**< sample rate divider */
#define ICM20608_REG_CONFIG             0x1A        /**< configuration */
#define ICM20608_REG_GYRO_CONFIG        0x1B        /**< gyroscope configuration */
#define ICM20608_REG_ACCEL_CONFIG       0x1C        /**< accelerometer configuration */
#define ICM20608_REG_ACCEL_CONFIG2      0x1D        /**< accelerometer configuration 2 */
#define ICM20608_REG_LP_MODE_CFG        0x1E        /**< low power mode config */
#define ICM20608_REG_ACCEL_WOM_THR      0x1F        /**< wake on motion threshold */
#define ICM20608_REG_FIFO_EN            0x23        /**< fifo enable */
#define ICM20608_REG_FSYNC_INT          0x36        /**< fsync interrupt status */
#define ICM20608_REG_INT_PIN_CFG        0x37        /**< int pin configuration */
#define ICM20608_REG_INT_ENABLE         0x38        /**< interrupt enable */
#define ICM20608_REG_INT_STATUS         0x3A        /**< interrupt status */
#define ICM20608_REG_ACCEL_XOUT_H       0x3B        /**< accel x high */
#define ICM20608_REG_ACCEL_XOUT_L       0x3C        /**< accel x low */
#define ICM20608_REG_ACCEL_YOUT_H       0x3D        /**< accel y high */
#define ICM20608_REG_ACCEL_YOUT_L       0x3E        /**< accel y low */
#define ICM20608_REG_ACCEL_ZOUT_H       0x3F        /**< accel z high */
#define ICM20608_REG_ACCEL_ZOUT_L       0x40        /**< accel z low */
#define ICM20608_REG_TEMP_OUT_H         0x41        /**< temperature high */
#define ICM20608_REG_TEMP_OUT_L         0x42        /**< temperature low */
#define ICM20608_REG_GYRO_XOUT_H        0x43        /**< gyro x high */
#define ICM20608_REG_GYRO_XOUT_L        0x44        /**< gyro x low */
#define ICM20608_REG_GYRO_YOUT_H        0x45        /**< gyro y high */
#define ICM20608_REG_GYRO_YOUT_L        0x46        /**< gyro y low */
#define ICM20608_REG_GYRO_ZOUT_H        0x47        /**< gyro z high */
#define ICM20608_REG_GYRO_ZOUT_L        0x48        /**< gyro z low */
#define ICM20608_REG_SIGNAL_PATH_RESET  0x68        /**< signal path reset */
#define ICM20608_REG_USER_CTRL          0x6A        /**< user control */
#define ICM20608_REG_PWR_MGMT_1         0x6B        /**< power management 1 */
#define ICM20608_REG_PWR_MGMT_2         0x6C        /**< power management 2 */
#define ICM20608_REG_FIFO_COUNTH        0x72        /**< fifo count high */
#define ICM20608_REG_FIFO_COUNTL        0x73        /**< fifo count low */
#define ICM20608_REG_FIFO_R_W           0x74        /**< fifo read write */
#define ICM20608_REG_WHO_AM_I           0x75        /**< who am i */

/**
 * @brief icm20608 handle structure definition
 */
typedef struct icm20608_handle_s
{
    uint8_t iic_addr;                                                          /**< iic device address */
    uint8_t (*ps_iic_init)(void);                                                /**< point to an iic_init function address */
    uint8_t (*ps_iic_deinit)(void);                                             /**< point to an iic_deinit function address */
    uint8_t (*ps_iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len); /**< point to an iic_read function address */
    uint8_t (*ps_iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);/**< point to an iic_write function address */
    void (*ps_delay_ms)(uint32_t ms);                                           /**< point to a delay_ms function address */
    void (*ps_debug_print)(const char *const fmt, ...);                         /**< point to a debug_print function address */
    uint8_t inited;                                                          /**< inited flag */
    uint8_t interface;                                                       /**< device interface */
} icm20608_handle_t;

/**
 * @brief icm20608 information structure definition
 */
typedef struct icm20608_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                /**< chip interface name */
    float supply_voltage_min_v;       /**< chip min supply voltage */
    float supply_voltage_max_v;       /**< chip max supply voltage */
    float max_current_ma;            /**< chip max current */
    float temperature_min;           /**< chip min operating temperature */
    float temperature_max;           /**< chip max operating temperature */
    uint32_t driver_version;         /**< driver version */
} icm20608_info_t;

/**
 * @defgroup icm20608_link_driver icm20608 link driver function
 * @brief    icm20608 link driver modules
 * @ingroup  icm20608_driver
 * @{
 */

/**
 * @brief     initialize icm20608_handle_t structure
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] STRUCTURE is icm20608_handle_t
 * @note      none
 */
#define DRIVER_ICM20608_LINK_INIT(HANDLE, STRUCTURE)   memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_ICM20608_LINK_IIC_INIT(HANDLE, FUC)    (HANDLE)->ps_iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_ICM20608_LINK_IIC_DEINIT(HANDLE, FUC)  (HANDLE)->ps_iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_ICM20608_LINK_IIC_READ(HANDLE, FUC)    (HANDLE)->ps_iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_ICM20608_LINK_IIC_WRITE(HANDLE, FUC)   (HANDLE)->ps_iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_ICM20608_LINK_DELAY_MS(HANDLE, FUC)    (HANDLE)->ps_delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an icm20608 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_ICM20608_LINK_DEBUG_PRINT(HANDLE, FUC)  (HANDLE)->ps_debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup icm20608_basic_driver icm20608 basic driver function
 * @brief    icm20608 basic driver modules
 * @ingroup  icm20608_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an icm20608 info structure
 * @return     status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note       none
 */
uint8_t icm20608_info(icm20608_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an icm20608 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 * @note      none
 */
uint8_t icm20608_init(icm20608_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an icm20608 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t icm20608_deinit(icm20608_handle_t *handle);

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an icm20608 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *degrees points to a converted temperature buffer
 * @return     status code
 *            - 0 success
 *            - 1 read temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note       none
 */
uint8_t icm20608_read_temperature(icm20608_handle_t *handle, int16_t *raw, float *degrees);

/**
 * @brief      read the gyroscope
 * @param[in]  *handle points to an icm20608 handle structure
 * @param[out] *raw points to a raw gyroscope buffer
 * @param[out] *dps points to a converted gyroscope buffer
 * @return     status code
 *            - 0 success
 *            - 1 read gyroscope failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note       none
 */
uint8_t icm20608_read_gyroscope(icm20608_handle_t *handle, int16_t raw[3], float dps[3]);

/**
 * @brief      read the accelerometer
 * @param[in]  *handle points to an icm20608 handle structure
 * @param[out] *raw points to a raw accelerometer buffer
 * @param[out] *g points to a converted accelerometer buffer
 * @return     status code
 *            - 0 success
 *            - 1 read accelerometer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note       none
 */
uint8_t icm20608_read_accelerometer(icm20608_handle_t *handle, int16_t raw[3], float g[3]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif 
