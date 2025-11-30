/**
 * @file     w25qxx_basic.h
 * @brief    W25QXX基本操作头文件
 * @author   MCU Electronics Technology
 * @version  V1.0
 * @date     2024-01-01
 */

#ifndef W25QXX_BASIC_H
#define W25QXX_BASIC_H

#include "w25qxx_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief     basic example init
 * @param[in] type is the chip type
 * @param[in] interface is the chip interface
 * @param[in] dual_quad_spi_enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t w25qxx_basic_init(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t w25qxx_basic_deinit(void);

/**
 * @brief  basic example power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t w25qxx_basic_power_down(void);

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t w25qxx_basic_wake_up(void);

/**
 * @brief  basic example chip erase
 * @return status code
 *         - 0 success
 *         - 1 chip erase failed
 * @note   none
 */
uint8_t w25qxx_basic_chip_erase(void);

/**
 * @brief      basic example get the manufacturer && device id information
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id failed
 * @note       none
 */
uint8_t w25qxx_basic_get_id(uint8_t *manufacturer, uint8_t *device_id);

/**
 * @brief     basic example write
 * @param[in] addr is the write address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t w25qxx_basic_write(uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      basic example read
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_basic_read(uint32_t addr, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* W25QXX_BASIC_H */ 