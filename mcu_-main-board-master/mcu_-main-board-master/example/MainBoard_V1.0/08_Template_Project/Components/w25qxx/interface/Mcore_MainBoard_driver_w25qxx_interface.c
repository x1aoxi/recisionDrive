#include "driver_w25qxx_interface.h"
#include "spi.h"
#include "delay.h"
#include <stdarg.h>

/**
 * @brief  interface spi qspi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi qspi init failed
 * @note   none
 */
uint8_t w25qxx_interface_spi_qspi_init(void)
{
    return spi_init(SPI_MODE_3);
}

/**
 * @brief  interface spi qspi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi qspi deinit failed
 * @note   none
 */
uint8_t w25qxx_interface_spi_qspi_deinit(void)
{
    return spi_deinit();
}

/**
 * @brief      interface spi qspi bus write read
 * @param[in]  instruction is the sent instruction
 * @param[in]  instruction_line is the instruction phy lines
 * @param[in]  address is the register address
 * @param[in]  address_line is the address phy lines
 * @param[in]  address_len is the address length
 * @param[in]  alternate is the register address
 * @param[in]  alternate_line is the alternate phy lines
 * @param[in]  alternate_len is the alternate length
 * @param[in]  dummy is the dummy cycle
 * @param[in]  *in_buf points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to a output buffer
 * @param[in]  out_len is the output length
 * @param[in]  data_line is the data phy lines
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t w25qxx_interface_spi_qspi_write_read(uint8_t instruction, uint8_t instruction_line,
                                             uint32_t address, uint8_t address_line, uint8_t address_len,
                                             uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                                             uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                                             uint8_t *out_buf, uint32_t out_len, uint8_t data_line)
{
    if ((instruction_line != 0) || (address_line != 0) || (alternate_line != 0) || (dummy != 0) || (data_line != 1))
    {
        return 1;
    }
    
    return spi_write_read(in_buf, in_len, out_buf, out_len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void w25qxx_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface delay us
 * @param[in] us
 * @note      none
 */
void w25qxx_interface_delay_us(uint32_t us)
{
    delay_us(us);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void w25qxx_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    printf("%.*s", len, (uint8_t *)str);
}
