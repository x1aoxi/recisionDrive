#include "driver_aht20_interface.h"
#include "i2c.h"
#include "elog.h"
#include <stdarg.h>
#include "perf_counter.h"

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t aht20_interface_iic_init(void)
{
    return iic_init();
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t aht20_interface_iic_deinit(void)
{
    return iic_deinit();
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t aht20_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    return iic_read_cmd(addr, buf, len);
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t aht20_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    return iic_write_cmd(addr, buf, len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void aht20_interface_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void aht20_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    elog_d("ASAIR_AHT20", "%.*s", len, (uint8_t *)str);
}
