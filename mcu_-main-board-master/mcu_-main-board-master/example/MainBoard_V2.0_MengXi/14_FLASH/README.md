# 14_FLASH - 外部Flash存储示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

14_FLASH是STM32F407VET6开发板的外部Flash存储器控制示例工程，演示如何使用SPI接口控制W25Q系列Flash存储芯片进行数据读写、扇区擦除、芯片管理等操作。该项目实现了完整的Flash驱动程序、文件系统支持、数据备份、固件升级等功能，适用于数据记录、程序存储、系统升级等应用场景。

## 主要特性

- **W25Q Flash驱动**: 支持W25Q16/32/64/128等系列Flash芯片
- **SPI高速通信**: 支持标准SPI和快速读取模式
- **完整Flash操作**: 页编程、扇区擦除、块擦除、芯片擦除
- **数据保护**: 写保护、状态寄存器管理
- **文件系统**: FatFs文件系统支持(可选)
- **数据管理**: 数据备份、恢复、校验功能
- **固件升级**: 支持程序在线升级(IAP)
- **存储测试**: 全面的存储器测试和基准测试

## 硬件连接

### W25Q Flash连接 (SPI接口)
| 功能 | Flash引脚 | STM32引脚 | 说明 |
|------|-----------|-----------|------|
| 电源 | VCC | 3.3V | 供电电源 |
| 地线 | GND | GND | 接地 |
| 片选 | CS | PA4 (SPI1_NSS) | 片选信号 |
| 时钟 | CLK | PA5 (SPI1_SCK) | SPI时钟线 |
| 数据输入 | DI | PA7 (SPI1_MOSI) | SPI数据输入 |
| 数据输出 | DO | PA6 (SPI1_MISO) | SPI数据输出 |
| 写保护 | WP | 3.3V | 写保护(拉高) |
| 保持 | HOLD | 3.3V | 保持信号(拉高) |

### 其他连接
| 功能 | 引脚 | 说明 |
|------|------|------|
| UART_TX | PA9 | 串口发送 |
| UART_RX | PA10 | 串口接收 |
| LED1 | PC0 | 操作状态LED |
| LED2 | PC1 | 错误指示LED |
| 按键 | PA0 | 功能测试按键 |

## 功能说明

### Flash基本操作
- **芯片识别**: 读取制造商ID和设备ID
- **状态查询**: 读取状态寄存器
- **页编程**: 256字节页编程操作
- **扇区擦除**: 4KB扇区擦除
- **块擦除**: 32KB/64KB块擦除
- **芯片擦除**: 全芯片擦除

### 高级功能
- **快速读取**: 支持Fast Read命令
- **写保护**: 软件和硬件写保护
- **深度断电**: 低功耗模式控制
- **唯一ID**: 读取芯片唯一标识

### 数据管理
- **文件系统**: 基于FatFs的文件操作
- **数据校验**: CRC32数据完整性校验
- **磨损均衡**: 简单的磨损均衡算法
- **坏块管理**: 坏块检测和管理

### 应用功能
- **数据日志**: 系统运行数据记录
- **参数存储**: 系统配置参数保存
- **程序备份**: 应用程序备份和恢复
- **固件升级**: IAP在线升级功能

## 目录结构

```
14_FLASH/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── spi.h                # SPI配置头文件
│   │   └── fatfs.h              # FatFs配置头文件
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── spi.c                # SPI配置源文件
│       └── fatfs.c              # FatFs配置源文件
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── FATFS/                       # FatFs文件系统
│   ├── App/
│   ├── Target/
│   └── ...
├── MDK-ARM/
│   ├── 14_FLASH.uvprojx         # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── flash_app.c          # Flash应用实现
│   │   ├── flash_app.h          # Flash应用头文件
│   │   ├── data_manager.c       # 数据管理应用
│   │   ├── data_manager.h
│   │   ├── iap_app.c            # IAP升级应用
│   │   └── iap_app.h
│   ├── Driver/
│   │   ├── w25qxx_driver.c      # W25Q Flash驱动
│   │   ├── w25qxx_driver.h      # W25Q驱动头文件
│   │   └── spi_flash.c          # SPI Flash接口层
│   └── Module/
│       ├── flash_test.c         # Flash测试模块
│       ├── flash_test.h
│       ├── wear_leveling.c      # 磨损均衡
│       └── wear_leveling.h
├── 14_FLASH.ioc                # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接W25Q Flash芯片
   - 确保SPI接线正确，WP和HOLD引脚拉高
   - 检查电源供电稳定

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/14_FLASH.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **功能测试**
   - 串口输出芯片识别信息
   - 执行Flash读写测试
   - 验证数据完整性

### 代码示例

#### W25Q Flash初始化

```c
#include "w25qxx_driver.h"

/* Flash设备信息结构体 */
typedef struct {
    uint16_t manufacturer_id;   // 制造商ID
    uint16_t device_id;        // 设备ID
    uint32_t capacity;         // 容量(字节)
    uint16_t page_size;        // 页大小
    uint16_t sector_size;      // 扇区大小
    char name[32];             // 芯片名称
} flash_info_t;

flash_info_t flash_info;

/* W25Q Flash初始化 */
w25qxx_status_t W25QXX_Init(void)
{
    /* 唤醒Flash芯片 */
    if(W25QXX_Wake_Up() != W25QXX_OK)
    {
        printf("W25QXX Wake Up Failed!\\r\\n");
        return W25QXX_ERROR;
    }
    
    /* 读取芯片ID */
    uint16_t manufacturer_id, device_id;
    if(W25QXX_Read_ID(&manufacturer_id, &device_id) != W25QXX_OK)
    {
        printf("W25QXX Read ID Failed!\\r\\n");
        return W25QXX_ERROR;
    }
    
    /* 解析芯片信息 */
    flash_info.manufacturer_id = manufacturer_id;
    flash_info.device_id = device_id;
    flash_info.page_size = 256;
    flash_info.sector_size = 4096;
    
    /* 根据设备ID确定容量和名称 */
    switch(device_id)
    {
        case 0x4015:  // W25Q16
            flash_info.capacity = 2 * 1024 * 1024;
            strcpy(flash_info.name, "W25Q16");
            break;
        case 0x4016:  // W25Q32
            flash_info.capacity = 4 * 1024 * 1024;
            strcpy(flash_info.name, "W25Q32");
            break;
        case 0x4017:  // W25Q64
            flash_info.capacity = 8 * 1024 * 1024;
            strcpy(flash_info.name, "W25Q64");
            break;
        case 0x4018:  // W25Q128
            flash_info.capacity = 16 * 1024 * 1024;
            strcpy(flash_info.name, "W25Q128");
            break;
        default:
            flash_info.capacity = 0;
            strcpy(flash_info.name, "Unknown");
            break;
    }
    
    printf("W25Q Flash Initialized Successfully!\\r\\n");
    printf("Manufacturer ID: 0x%04X\\r\\n", manufacturer_id);
    printf("Device ID: 0x%04X\\r\\n", device_id);
    printf("Chip Name: %s\\r\\n", flash_info.name);
    printf("Capacity: %d KB\\r\\n", flash_info.capacity / 1024);
    
    return W25QXX_OK;
}
```

#### SPI通信底层函数

```c
/* SPI Flash片选控制 */
void W25QXX_CS_Low(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void W25QXX_CS_High(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

/* SPI数据传输 */
uint8_t W25QXX_SPI_Transmit_Receive(uint8_t data)
{
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi1, &data, &rx_data, 1, HAL_MAX_DELAY);
    return rx_data;
}

/* 读取芯片ID */
w25qxx_status_t W25QXX_Read_ID(uint16_t *manufacturer_id, uint16_t *device_id)
{
    uint8_t cmd[4] = {0x90, 0x00, 0x00, 0x00};  // Read ID命令
    uint8_t id_data[2];
    
    W25QXX_CS_Low();
    
    /* 发送命令和地址 */
    for(int i = 0; i < 4; i++)
    {
        W25QXX_SPI_Transmit_Receive(cmd[i]);
    }
    
    /* 读取ID数据 */
    id_data[0] = W25QXX_SPI_Transmit_Receive(0xFF);  // Manufacturer ID
    id_data[1] = W25QXX_SPI_Transmit_Receive(0xFF);  // Device ID
    
    W25QXX_CS_High();
    
    *manufacturer_id = id_data[0];
    *device_id = id_data[1];
    
    return W25QXX_OK;
}

/* 读取状态寄存器 */
uint8_t W25QXX_Read_Status_Register(void)
{
    uint8_t status;
    
    W25QXX_CS_Low();
    W25QXX_SPI_Transmit_Receive(0x05);  // Read Status Register命令
    status = W25QXX_SPI_Transmit_Receive(0xFF);
    W25QXX_CS_High();
    
    return status;
}

/* 等待写操作完成 */
w25qxx_status_t W25QXX_Wait_For_Write_End(void)
{
    uint32_t timeout = 1000;
    
    while(timeout--)
    {
        if((W25QXX_Read_Status_Register() & 0x01) == 0)
        {
            return W25QXX_OK;  // 写操作完成
        }
        HAL_Delay(1);
    }
    
    return W25QXX_TIMEOUT;
}
```

#### Flash读写操作

```c
/* 页编程(256字节) */
w25qxx_status_t W25QXX_Page_Program(uint32_t address, uint8_t *data, uint16_t size)
{
    if(size > 256) return W25QXX_ERROR;
    
    /* 写使能 */
    W25QXX_Write_Enable();
    
    W25QXX_CS_Low();
    
    /* 发送页编程命令 */
    W25QXX_SPI_Transmit_Receive(0x02);  // Page Program命令
    W25QXX_SPI_Transmit_Receive((address >> 16) & 0xFF);
    W25QXX_SPI_Transmit_Receive((address >> 8) & 0xFF);
    W25QXX_SPI_Transmit_Receive(address & 0xFF);
    
    /* 发送数据 */
    for(uint16_t i = 0; i < size; i++)
    {
        W25QXX_SPI_Transmit_Receive(data[i]);
    }
    
    W25QXX_CS_High();
    
    /* 等待写操作完成 */
    return W25QXX_Wait_For_Write_End();
}

/* 数据读取 */
w25qxx_status_t W25QXX_Read_Data(uint32_t address, uint8_t *data, uint32_t size)
{
    W25QXX_CS_Low();
    
    /* 发送读命令 */
    W25QXX_SPI_Transmit_Receive(0x03);  // Read Data命令
    W25QXX_SPI_Transmit_Receive((address >> 16) & 0xFF);
    W25QXX_SPI_Transmit_Receive((address >> 8) & 0xFF);
    W25QXX_SPI_Transmit_Receive(address & 0xFF);
    
    /* 读取数据 */
    for(uint32_t i = 0; i < size; i++)
    {
        data[i] = W25QXX_SPI_Transmit_Receive(0xFF);
    }
    
    W25QXX_CS_High();
    
    return W25QXX_OK;
}

/* 快速读取 */
w25qxx_status_t W25QXX_Fast_Read(uint32_t address, uint8_t *data, uint32_t size)
{
    W25QXX_CS_Low();
    
    /* 发送快速读命令 */
    W25QXX_SPI_Transmit_Receive(0x0B);  // Fast Read命令
    W25QXX_SPI_Transmit_Receive((address >> 16) & 0xFF);
    W25QXX_SPI_Transmit_Receive((address >> 8) & 0xFF);
    W25QXX_SPI_Transmit_Receive(address & 0xFF);
    W25QXX_SPI_Transmit_Receive(0xFF);  // Dummy字节
    
    /* 读取数据 */
    for(uint32_t i = 0; i < size; i++)
    {
        data[i] = W25QXX_SPI_Transmit_Receive(0xFF);
    }
    
    W25QXX_CS_High();
    
    return W25QXX_OK;
}

/* 扇区擦除(4KB) */
w25qxx_status_t W25QXX_Sector_Erase(uint32_t address)
{
    /* 写使能 */
    W25QXX_Write_Enable();
    
    W25QXX_CS_Low();
    
    /* 发送扇区擦除命令 */
    W25QXX_SPI_Transmit_Receive(0x20);  // Sector Erase命令
    W25QXX_SPI_Transmit_Receive((address >> 16) & 0xFF);
    W25QXX_SPI_Transmit_Receive((address >> 8) & 0xFF);
    W25QXX_SPI_Transmit_Receive(address & 0xFF);
    
    W25QXX_CS_High();
    
    /* 等待擦除完成 */
    return W25QXX_Wait_For_Write_End();
}

/* 块擦除(64KB) */
w25qxx_status_t W25QXX_Block_Erase_64K(uint32_t address)
{
    W25QXX_Write_Enable();
    
    W25QXX_CS_Low();
    W25QXX_SPI_Transmit_Receive(0xD8);  // Block Erase 64K命令
    W25QXX_SPI_Transmit_Receive((address >> 16) & 0xFF);
    W25QXX_SPI_Transmit_Receive((address >> 8) & 0xFF);
    W25QXX_SPI_Transmit_Receive(address & 0xFF);
    W25QXX_CS_High();
    
    return W25QXX_Wait_For_Write_End();
}

/* 芯片擦除 */
w25qxx_status_t W25QXX_Chip_Erase(void)
{
    W25QXX_Write_Enable();
    
    W25QXX_CS_Low();
    W25QXX_SPI_Transmit_Receive(0xC7);  // Chip Erase命令
    W25QXX_CS_High();
    
    printf("Chip Erase Started, Please Wait...\\r\\n");
    
    return W25QXX_Wait_For_Write_End();
}
```

#### 高级功能实现

```c
/* 写保护控制 */
w25qxx_status_t W25QXX_Set_Write_Protection(uint8_t protection)
{
    W25QXX_Write_Enable();
    
    W25QXX_CS_Low();
    W25QXX_SPI_Transmit_Receive(0x01);  // Write Status Register命令
    W25QXX_SPI_Transmit_Receive(protection);
    W25QXX_CS_High();
    
    return W25QXX_Wait_For_Write_End();
}

/* 深度断电模式 */
w25qxx_status_t W25QXX_Deep_Power_Down(void)
{
    W25QXX_CS_Low();
    W25QXX_SPI_Transmit_Receive(0xB9);  // Deep Power Down命令
    W25QXX_CS_High();
    
    return W25QXX_OK;
}

/* 从深度断电模式唤醒 */
w25qxx_status_t W25QXX_Wake_Up(void)
{
    W25QXX_CS_Low();
    W25QXX_SPI_Transmit_Receive(0xAB);  // Release Power Down命令
    W25QXX_CS_High();
    
    HAL_Delay(1);  // 等待唤醒
    
    return W25QXX_OK;
}

/* 读取唯一ID */
w25qxx_status_t W25QXX_Read_Unique_ID(uint8_t *unique_id)
{
    W25QXX_CS_Low();
    
    /* 发送读唯一ID命令 */
    W25QXX_SPI_Transmit_Receive(0x4B);  // Read Unique ID命令
    for(int i = 0; i < 4; i++)
    {
        W25QXX_SPI_Transmit_Receive(0xFF);  // Dummy字节
    }
    
    /* 读取8字节唯一ID */
    for(int i = 0; i < 8; i++)
    {
        unique_id[i] = W25QXX_SPI_Transmit_Receive(0xFF);
    }
    
    W25QXX_CS_High();
    
    return W25QXX_OK;
}
```

#### Flash测试功能

```c
/* Flash读写测试 */
w25qxx_status_t Flash_Read_Write_Test(void)
{
    uint8_t write_data[256];
    uint8_t read_data[256];
    uint32_t test_address = 0x1000;  // 测试地址
    
    /* 生成测试数据 */
    for(int i = 0; i < 256; i++)
    {
        write_data[i] = i;
    }
    
    printf("Flash Read/Write Test Started...\\r\\n");
    
    /* 擦除扇区 */
    if(W25QXX_Sector_Erase(test_address) != W25QXX_OK)
    {
        printf("Sector Erase Failed!\\r\\n");
        return W25QXX_ERROR;
    }
    
    /* 写入数据 */
    if(W25QXX_Page_Program(test_address, write_data, 256) != W25QXX_OK)
    {
        printf("Page Program Failed!\\r\\n");
        return W25QXX_ERROR;
    }
    
    /* 读取数据 */
    if(W25QXX_Read_Data(test_address, read_data, 256) != W25QXX_OK)
    {
        printf("Read Data Failed!\\r\\n");
        return W25QXX_ERROR;
    }
    
    /* 数据校验 */
    for(int i = 0; i < 256; i++)
    {
        if(write_data[i] != read_data[i])
        {
            printf("Data Verify Failed at offset %d: Write=0x%02X, Read=0x%02X\\r\\n",
                   i, write_data[i], read_data[i]);
            return W25QXX_ERROR;
        }
    }
    
    printf("Flash Read/Write Test Passed!\\r\\n");
    return W25QXX_OK;
}

/* Flash速度测试 */
void Flash_Speed_Test(void)
{
    uint8_t test_data[4096];
    uint32_t start_time, end_time;
    uint32_t test_address = 0x10000;
    
    /* 生成测试数据 */
    for(int i = 0; i < 4096; i++)
    {
        test_data[i] = i & 0xFF;
    }
    
    printf("Flash Speed Test Started...\\r\\n");
    
    /* 擦除测试 */
    start_time = HAL_GetTick();
    W25QXX_Sector_Erase(test_address);
    end_time = HAL_GetTick();
    printf("Sector Erase Time: %ld ms\\r\\n", end_time - start_time);
    
    /* 写入测试 */
    start_time = HAL_GetTick();
    for(int i = 0; i < 16; i++)  // 16页 = 4KB
    {
        W25QXX_Page_Program(test_address + i * 256, test_data + i * 256, 256);
    }
    end_time = HAL_GetTick();
    printf("4KB Write Time: %ld ms, Speed: %.2f KB/s\\r\\n", 
           end_time - start_time, 4000.0f / (end_time - start_time));
    
    /* 读取测试 */
    start_time = HAL_GetTick();
    W25QXX_Read_Data(test_address, test_data, 4096);
    end_time = HAL_GetTick();
    printf("4KB Read Time: %ld ms, Speed: %.2f KB/s\\r\\n", 
           end_time - start_time, 4000.0f / (end_time - start_time));
    
    /* 快速读取测试 */
    start_time = HAL_GetTick();
    W25QXX_Fast_Read(test_address, test_data, 4096);
    end_time = HAL_GetTick();
    printf("4KB Fast Read Time: %ld ms, Speed: %.2f KB/s\\r\\n", 
           end_time - start_time, 4000.0f / (end_time - start_time));
}
```

#### 数据管理功能

```c
/* CRC32校验 */
uint32_t Calculate_CRC32(uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t i, j;
    
    for(i = 0; i < length; i++)
    {
        crc ^= data[i];
        for(j = 0; j < 8; j++)
        {
            if(crc & 1)
            {
                crc = (crc >> 1) ^ 0xEDB88320;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    
    return ~crc;
}

/* 数据备份和恢复 */
typedef struct {
    uint32_t data_size;
    uint32_t crc32;
    uint8_t version;
    uint8_t reserved[3];
} backup_header_t;

/* 备份数据到Flash */
w25qxx_status_t Backup_Data_To_Flash(uint32_t address, uint8_t *data, uint32_t size)
{
    backup_header_t header;
    uint32_t total_size = sizeof(header) + size;
    
    /* 检查空间 */
    if(total_size > 4096)  // 超过一个扇区
    {
        return W25QXX_ERROR;
    }
    
    /* 准备头部信息 */
    header.data_size = size;
    header.crc32 = Calculate_CRC32(data, size);
    header.version = 1;
    memset(header.reserved, 0, sizeof(header.reserved));
    
    /* 擦除扇区 */
    if(W25QXX_Sector_Erase(address) != W25QXX_OK)
    {
        return W25QXX_ERROR;
    }
    
    /* 写入头部 */
    if(W25QXX_Page_Program(address, (uint8_t*)&header, sizeof(header)) != W25QXX_OK)
    {
        return W25QXX_ERROR;
    }
    
    /* 写入数据 */
    uint32_t data_address = address + sizeof(header);
    uint32_t remaining = size;
    uint32_t offset = 0;
    
    while(remaining > 0)
    {
        uint32_t write_size = (remaining > 256) ? 256 : remaining;
        
        if(W25QXX_Page_Program(data_address + offset, data + offset, write_size) != W25QXX_OK)
        {
            return W25QXX_ERROR;
        }
        
        offset += write_size;
        remaining -= write_size;
    }
    
    printf("Data Backup Completed: %ld bytes, CRC32: 0x%08lX\\r\\n", size, header.crc32);
    return W25QXX_OK;
}

/* 从Flash恢复数据 */
w25qxx_status_t Restore_Data_From_Flash(uint32_t address, uint8_t *data, uint32_t *size)
{
    backup_header_t header;
    
    /* 读取头部信息 */
    if(W25QXX_Read_Data(address, (uint8_t*)&header, sizeof(header)) != W25QXX_OK)
    {
        return W25QXX_ERROR;
    }
    
    /* 检查数据有效性 */
    if(header.data_size == 0xFFFFFFFF || header.data_size > 4000)
    {
        printf("Invalid backup data!\\r\\n");
        return W25QXX_ERROR;
    }
    
    /* 读取数据 */
    uint32_t data_address = address + sizeof(header);
    if(W25QXX_Read_Data(data_address, data, header.data_size) != W25QXX_OK)
    {
        return W25QXX_ERROR;
    }
    
    /* CRC校验 */
    uint32_t calculated_crc = Calculate_CRC32(data, header.data_size);
    if(calculated_crc != header.crc32)
    {
        printf("CRC Check Failed! Calculated: 0x%08lX, Expected: 0x%08lX\\r\\n",
               calculated_crc, header.crc32);
        return W25QXX_ERROR;
    }
    
    *size = header.data_size;
    printf("Data Restore Completed: %ld bytes, CRC32: OK\\r\\n", header.data_size);
    
    return W25QXX_OK;
}
```

## API参考

### 基础函数

```c
/* 初始化函数 */
w25qxx_status_t W25QXX_Init(void);                      // 初始化Flash
w25qxx_status_t W25QXX_Read_ID(uint16_t *mfg_id, uint16_t *dev_id); // 读取ID
uint8_t W25QXX_Read_Status_Register(void);             // 读取状态寄存器

/* 电源控制 */
w25qxx_status_t W25QXX_Deep_Power_Down(void);          // 深度断电
w25qxx_status_t W25QXX_Wake_Up(void);                  // 唤醒
```

### 读写操作函数

```c
/* 数据读写 */
w25qxx_status_t W25QXX_Read_Data(uint32_t address, uint8_t *data, uint32_t size);     // 读取数据
w25qxx_status_t W25QXX_Fast_Read(uint32_t address, uint8_t *data, uint32_t size);     // 快速读取
w25qxx_status_t W25QXX_Page_Program(uint32_t address, uint8_t *data, uint16_t size);  // 页编程

/* 擦除操作 */
w25qxx_status_t W25QXX_Sector_Erase(uint32_t address);      // 扇区擦除(4KB)
w25qxx_status_t W25QXX_Block_Erase_32K(uint32_t address);   // 块擦除(32KB)
w25qxx_status_t W25QXX_Block_Erase_64K(uint32_t address);   // 块擦除(64KB)
w25qxx_status_t W25QXX_Chip_Erase(void);                   // 芯片擦除
```

### 高级功能函数

```c
/* 保护和安全 */
w25qxx_status_t W25QXX_Set_Write_Protection(uint8_t protection);     // 设置写保护
w25qxx_status_t W25QXX_Read_Unique_ID(uint8_t *unique_id);          // 读取唯一ID

/* 测试和管理 */
w25qxx_status_t Flash_Read_Write_Test(void);                        // 读写测试
void Flash_Speed_Test(void);                                        // 速度测试
w25qxx_status_t Backup_Data_To_Flash(uint32_t addr, uint8_t *data, uint32_t size);   // 数据备份
w25qxx_status_t Restore_Data_From_Flash(uint32_t addr, uint8_t *data, uint32_t *size); // 数据恢复
```

## 配置选项

### SPI配置

```c
/* 在w25qxx_driver.h中配置 */
#define W25QXX_SPI_HANDLE       &hspi1          // SPI句柄
#define W25QXX_CS_PORT          GPIOA           // CS引脚端口
#define W25QXX_CS_PIN           GPIO_PIN_4      // CS引脚号
#define W25QXX_SPI_TIMEOUT      1000            // SPI超时时间(ms)
```

### Flash参数

```c
/* Flash参数 */
#define W25QXX_PAGE_SIZE        256             // 页大小
#define W25QXX_SECTOR_SIZE      4096            // 扇区大小
#define W25QXX_BLOCK_SIZE_32K   32768           // 32K块大小
#define W25QXX_BLOCK_SIZE_64K   65536           // 64K块大小
```

## 扩展功能

### 磨损均衡算法

```c
/* 简单磨损均衡 */
typedef struct {
    uint32_t erase_count;
    uint8_t bad_block;
} block_info_t;

block_info_t block_table[256];  // 假设256个块

/* 获取最少擦除次数的块 */
uint16_t Get_Min_Erase_Block(void)
{
    uint16_t min_block = 0;
    uint32_t min_count = 0xFFFFFFFF;
    
    for(int i = 0; i < 256; i++)
    {
        if(!block_table[i].bad_block && block_table[i].erase_count < min_count)
        {
            min_count = block_table[i].erase_count;
            min_block = i;
        }
    }
    
    return min_block;
}

/* 分配新块 */
uint32_t Allocate_New_Block(void)
{
    uint16_t block_num = Get_Min_Erase_Block();
    block_table[block_num].erase_count++;
    
    return block_num * W25QXX_BLOCK_SIZE_64K;
}
```

### IAP固件升级

```c
#define APP_START_ADDRESS   0x08010000  // 应用程序起始地址
#define FLASH_APP_ADDRESS   0x100000    // Flash中应用程序地址

/* 从Flash加载固件到内部Flash */
w25qxx_status_t IAP_Load_Firmware(void)
{
    uint8_t buffer[256];
    uint32_t app_size = 64 * 1024;  // 假设应用程序64KB
    
    printf("Starting IAP Firmware Update...\\r\\n");
    
    /* 解锁内部Flash */
    HAL_FLASH_Unlock();
    
    /* 擦除应用程序区域 */
    FLASH_EraseInitTypeDef erase_init;
    erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    erase_init.Sector = FLASH_SECTOR_4;  // 从扇区4开始
    erase_init.NbSectors = 4;            // 擦除4个扇区
    
    uint32_t sector_error;
    if(HAL_FLASHEx_Erase(&erase_init, &sector_error) != HAL_OK)
    {
        printf("Internal Flash Erase Failed!\\r\\n");
        HAL_FLASH_Lock();
        return W25QXX_ERROR;
    }
    
    /* 从外部Flash读取固件并写入内部Flash */
    for(uint32_t i = 0; i < app_size; i += 256)
    {
        /* 从外部Flash读取 */
        if(W25QXX_Read_Data(FLASH_APP_ADDRESS + i, buffer, 256) != W25QXX_OK)
        {
            printf("External Flash Read Failed!\\r\\n");
            HAL_FLASH_Lock();
            return W25QXX_ERROR;
        }
        
        /* 写入内部Flash */
        for(int j = 0; j < 256; j += 4)
        {
            uint32_t data = *(uint32_t*)(buffer + j);
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 
                               APP_START_ADDRESS + i + j, data) != HAL_OK)
            {
                printf("Internal Flash Program Failed!\\r\\n");
                HAL_FLASH_Lock();
                return W25QXX_ERROR;
            }
        }
        
        /* 显示进度 */
        if(i % 4096 == 0)
        {
            printf("Progress: %ld%%\\r\\n", (i * 100) / app_size);
        }
    }
    
    HAL_FLASH_Lock();
    printf("IAP Firmware Update Completed!\\r\\n");
    
    return W25QXX_OK;
}
```

## 故障排除

### 常见问题

1. **无法识别芯片**
   - 检查SPI接线是否正确
   - 确认CS、SCK、MISO、MOSI连接
   - 检查电源供电是否稳定

2. **读写错误**
   - 确认WP和HOLD引脚拉高
   - 检查SPI时钟频率设置
   - 确认写保护是否已解除

3. **数据丢失**
   - 检查电源稳定性
   - 确认擦写次数是否超限
   - 添加数据校验和备份

### 调试方法

1. 使用逻辑分析仪检查SPI时序
2. 通过串口输出详细错误信息
3. 添加LED指示操作状态

## 应用场景

1. **数据记录**: 传感器数据长期存储
2. **参数保存**: 系统配置参数备份
3. **程序更新**: 固件在线升级功能
4. **文件系统**: 构建简单的文件系统
5. **数据缓存**: 大容量数据缓存

## 学习要点

1. **SPI通信**: 掌握SPI协议和时序要求
2. **Flash原理**: 理解Flash存储器工作原理
3. **数据管理**: 学习数据组织和管理方法
4. **错误处理**: 掌握错误检测和恢复机制
5. **系统升级**: 理解IAP升级原理和实现

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
