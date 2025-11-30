#ifndef GD25QXX_H
#define GD25QXX_H

#include "main.h"
#include "spi.h"
#include "gpio.h"

#define SPI_FLASH_PAGE_SIZE 0x100

/* 使用 GPIOA PIN4 作为 SPI Flash 的 CS 引脚 */
#define SPI_FLASH_CS_LOW()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define SPI_FLASH_CS_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

/* SPI 外设定义，使用 SPI1 */
extern SPI_HandleTypeDef hspi1;
#define SPI_FLASH_HANDLE    &hspi1

#define ARRAYSIZE             (256)

void test_spi_flash(void);

/* initialize SPI Flash CS GPIO */
void spi_flash_init(void);
/* erase the specified flash sector */
void spi_flash_sector_erase(uint32_t sector_addr);
/* erase the entire flash */
void spi_flash_bulk_erase(void);
/* write more than one byte to the flash */
void spi_flash_page_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
/* write block of data to the flash */
void spi_flash_buffer_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
/* read a block of data from the flash */
void spi_flash_buffer_read(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read);
/* read flash identification */
uint32_t spi_flash_read_id(void);
/* initiate a read data byte (read) sequence from the flash */
void spi_flash_start_read_sequence(uint32_t read_addr);
/* enable the write access to the flash */
void spi_flash_write_enable(void);
/* poll the status of the write in progress (wip) flag in the flash's status register */
void spi_flash_wait_for_write_end(void);

/* HAL库SPI传输函数 */
/* 发送并接收一个字节 */
uint8_t spi_flash_send_byte(uint8_t byte);
/* 发送并接收一个半字 */
uint16_t spi_flash_send_halfword(uint16_t half_word);
/* 发送和接收多个字节 */
void spi_flash_transmit_receive(uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t size);

#endif /* GD25QXX_H */
