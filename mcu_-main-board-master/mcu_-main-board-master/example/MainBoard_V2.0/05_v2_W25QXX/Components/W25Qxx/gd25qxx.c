#include "gd25qxx.h"
#include <string.h>
#include <stdio.h>

#define WRITE 0x02 /* write to memory instruction */
#define WRSR 0x01  /* write status register instruction */
#define WREN 0x06  /* write enable instruction */

#define READ 0x03 /* read from memory instruction */
#define RDSR 0x05 /* read status register instruction  */
#define MAID 0x90 /* read identification */
#define JEID 0x9F /* read identification */
#define SE 0x20   /* sector erase instruction */
#define BE 0xC7   /* bulk erase instruction */

#define WIP_FLAG 0x01 /* write in progress(wip)flag */
#define DUMMY_BYTE 0xa5

/* SPI Flash初始化函数 */
void spi_flash_init(void)
{
    /* CS引脚初始化为高电平 */
    SPI_FLASH_CS_HIGH();
}

void spi_flash_sector_erase(uint32_t sector_addr)
{
    spi_flash_write_enable();

    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(SE);
    spi_flash_send_byte((sector_addr & 0xFF0000) >> 16);
    spi_flash_send_byte((sector_addr & 0xFF00) >> 8);
    spi_flash_send_byte(sector_addr & 0xFF);
    SPI_FLASH_CS_HIGH();

    spi_flash_wait_for_write_end();
}

void spi_flash_bulk_erase(void)
{
    spi_flash_write_enable();

    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(BE);
    SPI_FLASH_CS_HIGH();

    spi_flash_wait_for_write_end();
}

void spi_flash_page_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    spi_flash_write_enable();

    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(WRITE);
    spi_flash_send_byte((write_addr & 0xFF0000) >> 16);
    spi_flash_send_byte((write_addr & 0xFF00) >> 8);
    spi_flash_send_byte(write_addr & 0xFF);

    while (num_byte_to_write--)
    {
        spi_flash_send_byte(*pbuffer);
        pbuffer++;
    }

    SPI_FLASH_CS_HIGH();
    spi_flash_wait_for_write_end();
}

void spi_flash_buffer_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    uint8_t num_of_page = 0, num_of_single = 0, addr = 0, count = 0, temp = 0;

    addr = write_addr % SPI_FLASH_PAGE_SIZE;
    count = SPI_FLASH_PAGE_SIZE - addr;
    num_of_page = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
    num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

    if (0 == addr)
    {
        if (0 == num_of_page)
        {
            spi_flash_page_write(pbuffer, write_addr, num_byte_to_write);
        }
        else
        {
            while (num_of_page--)
            {
                spi_flash_page_write(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }
            spi_flash_page_write(pbuffer, write_addr, num_of_single);
        }
    }
    else
    {
        if (0 == num_of_page)
        {
            if (num_of_single > count)
            {
                temp = num_of_single - count;
                spi_flash_page_write(pbuffer, write_addr, count);
                write_addr += count;
                pbuffer += count;
                spi_flash_page_write(pbuffer, write_addr, temp);
            }
            else
            {
                spi_flash_page_write(pbuffer, write_addr, num_byte_to_write);
            }
        }
        else
        {
            num_byte_to_write -= count;
            num_of_page = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
            num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

            spi_flash_page_write(pbuffer, write_addr, count);
            write_addr += count;
            pbuffer += count;

            while (num_of_page--)
            {
                spi_flash_page_write(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }

            if (0 != num_of_single)
            {
                spi_flash_page_write(pbuffer, write_addr, num_of_single);
            }
        }
    }
}

void spi_flash_buffer_read(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read)
{
    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(READ);
    spi_flash_send_byte((read_addr & 0xFF0000) >> 16);
    spi_flash_send_byte((read_addr & 0xFF00) >> 8);
    spi_flash_send_byte(read_addr & 0xFF);

    while (num_byte_to_read--)
    {
        *pbuffer = spi_flash_send_byte(DUMMY_BYTE);
        pbuffer++;
    }

    SPI_FLASH_CS_HIGH();
}

uint32_t spi_flash_read_ma_id(void)
{
    uint32_t temp = 0, temp0 = 0, temp1 = 0;

    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(MAID);
    spi_flash_send_byte(DUMMY_BYTE);
    spi_flash_send_byte(DUMMY_BYTE);
    spi_flash_send_byte(DUMMY_BYTE);
    temp0 = spi_flash_send_byte(0xff);
    temp1 = spi_flash_send_byte(0xff);
    SPI_FLASH_CS_HIGH();

    temp = (temp1 << 8) | temp0;
    return temp;
}

uint32_t spi_flash_read_jed_id(void)
{
    uint32_t temp = 0, temp0 = 0, temp1 = 0, temp2 = 0;

    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(JEID);
    temp0 = spi_flash_send_byte(DUMMY_BYTE);
    temp1 = spi_flash_send_byte(DUMMY_BYTE);
    temp2 = spi_flash_send_byte(DUMMY_BYTE);
    SPI_FLASH_CS_HIGH();

    temp = (temp0 << 16) | (temp1 << 8) | temp2;
    return temp;
}

void spi_flash_start_read_sequence(uint32_t read_addr)
{
    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(READ);
    spi_flash_send_byte((read_addr & 0xFF0000) >> 16);
    spi_flash_send_byte((read_addr & 0xFF00) >> 8);
    spi_flash_send_byte(read_addr & 0xFF);
}

void spi_flash_write_enable(void)
{
    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(WREN);
    SPI_FLASH_CS_HIGH();
}

void spi_flash_wait_for_write_end(void)
{
    uint8_t flash_status = 0;

    SPI_FLASH_CS_LOW();
    spi_flash_send_byte(RDSR);

    do
    {
        flash_status = spi_flash_send_byte(DUMMY_BYTE);
    } while ((flash_status & WIP_FLAG) == 0x01);

    SPI_FLASH_CS_HIGH();
}

/**
 * @brief 发送并接收一个字节
 * @param byte 要发送的字节
 * @return 从 SPI 总线接收到的字节
 */
uint8_t spi_flash_send_byte(uint8_t byte)
{
    uint8_t rx_data = 0;

    /* 使用HAL库进行SPI传输 */
    if (HAL_SPI_TransmitReceive(SPI_FLASH_HANDLE, &byte, &rx_data, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        /* 传输错误处理 */
        return 0xFF;
    }

    return rx_data;
}

/**
 * @brief 发送并接收一个半字（16位数据）
 * @param half_word 要发送的半字
 * @return 从 SPI 总线接收到的半字
 */
uint16_t spi_flash_send_halfword(uint16_t half_word)
{
    uint8_t tx_data[2], rx_data[2];
    uint16_t result;

    /* 分解半字为两个字节 */
    tx_data[0] = (uint8_t)(half_word >> 8);
    tx_data[1] = (uint8_t)half_word;

    /* 使用HAL库进行SPI传输 */
    if (HAL_SPI_TransmitReceive(SPI_FLASH_HANDLE, tx_data, rx_data, 2, HAL_MAX_DELAY) != HAL_OK)
    {
        /* 传输错误处理 */
        return 0xFFFF;
    }

    /* 组合接收到的数据 */
    result = (uint16_t)(rx_data[0] << 8) | rx_data[1];

    return result;
}

/**
 * @brief 发送和接收多个字节
 * @param tx_buffer 发送缓冲区
 * @param rx_buffer 接收缓冲区
 * @param size 传输大小
 */
void spi_flash_transmit_receive(uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t size)
{
    /* 使用HAL库进行SPI传输 */
    if (HAL_SPI_TransmitReceive(SPI_FLASH_HANDLE, tx_buffer, rx_buffer, size, HAL_MAX_DELAY) != HAL_OK)
    {
        /* 传输错误处理 */
        memset(rx_buffer, 0xFF, size);
    }
}


uint32_t flash_id;

void test_spi_flash(void)
{
    uint8_t write_buffer[SPI_FLASH_PAGE_SIZE];
    uint8_t read_buffer[SPI_FLASH_PAGE_SIZE];

    uint32_t test_addr = 0x000000; // 测试地址，选择扇区起始地址

    // 1. 初始化SPI Flash驱动（主要是CS引脚状态）
    spi_flash_init();

    // 2. 读取Flash ID
    flash_id = spi_flash_read_ma_id();
    printf("ma-id: 0x%x\r\n", flash_id);
    
    flash_id = spi_flash_read_jed_id();
    printf("jed-id: 0x%x\r\n", flash_id);
    
    // 3. 擦除一个扇区（通常是4KB）
    // 注意：擦除操作需要时间
    spi_flash_sector_erase(test_addr);

    // （可选）验证擦除：读取一页并检查所有字节是否为0xFF
    spi_flash_buffer_read(read_buffer, test_addr, SPI_FLASH_PAGE_SIZE);
    int erased_check_ok = 1;
    for (int i = 0; i < SPI_FLASH_PAGE_SIZE; i++)
    {
        if (read_buffer[i] != 0xFF)
        {
            erased_check_ok = 0;
            break;
        }
    }

    // 4. 准备要写入的数据（一页）
    const char *message = "Hello from STM32 to SPI FLASH! HAL Library Test - 12345.";
    uint16_t data_len = strlen(message);
    if (data_len >= SPI_FLASH_PAGE_SIZE)
    {
        data_len = SPI_FLASH_PAGE_SIZE - 1; // 确保不超过页大小
    }
    memset(write_buffer, 0, SPI_FLASH_PAGE_SIZE);
    memcpy(write_buffer, message, data_len);
    write_buffer[data_len] = '\0'; // 确保字符串终止

    // 使用spi_flash_buffer_write（可以处理跨页，但这里我们在一页内写入）
    // 或者如果确定在一页内，直接使用spi_flash_page_write
     spi_flash_buffer_write(write_buffer, test_addr, SPI_FLASH_PAGE_SIZE); // 写入整页并填充

    // 5. 读回写入的数据
    memset(read_buffer, 0x00, SPI_FLASH_PAGE_SIZE);
    spi_flash_buffer_read(read_buffer, test_addr, SPI_FLASH_PAGE_SIZE);
    printf("Data read: \"%.*s\"\r\n", SPI_FLASH_PAGE_SIZE, read_buffer);
    // 6. 验证数据
    if (memcmp(write_buffer, read_buffer, SPI_FLASH_PAGE_SIZE) == 0)
    {
        // 数据验证成功！写入和读取成功
        printf("success\r\n");
    }
    else
    {
        // 数据验证失败！
        printf("success\r\n");
    }
}


