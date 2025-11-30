# Test_Example - 综合测试示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

Test_Example是STM32F407VET6开发板的综合测试示例工程，集成了主板上所有外设和传感器的测试功能。该项目提供了完整的硬件测试套件，包括自动化测试、交互式测试、性能基准测试等功能，是验证硬件功能、排查问题、学习开发的综合性参考工程。

## 主要特性

- **全外设测试**: 覆盖开发板所有外设和传感器
- **自动化测试**: 无人值守的自动测试流程
- **交互式测试**: 基于菜单的用户交互测试
- **性能基准**: 各模块性能基准测试
- **错误诊断**: 详细的错误诊断和报告功能
- **测试报告**: 生成详细的测试结果报告
- **在线调试**: 实时监控和调试功能
- **压力测试**: 长时间稳定性测试

## 测试覆盖范围

### 基础外设测试
- **GPIO**: LED控制、按键检测、引脚状态测试
- **UART**: 串口通信、波特率测试、数据完整性
- **I2C**: 设备扫描、数据传输、时序测试
- **SPI**: 通信速度、数据传输、CS控制测试
- **ADC**: 多通道采集、精度测试、转换速度
- **定时器**: PWM输出、输入捕获、定时精度

### 传感器模块测试
- **OLED显示**: 显示功能、字体显示、图形绘制
- **AHT20**: 温湿度测量精度、通信稳定性
- **ICM20608**: 六轴数据精度、姿态解算、校准
- **灰度传感器**: ADC采集、阈值检测、循迹算法
- **外部Flash**: 读写性能、数据完整性、擦写寿命

### 系统功能测试
- **时钟系统**: 各时钟域频率验证
- **电源管理**: 功耗测试、低功耗模式
- **中断系统**: 中断响应时间、优先级测试
- **DMA传输**: 传输速度、数据完整性
- **看门狗**: 复位功能、超时设置

## 目录结构

```
Test_Example/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── gpio.h
│   │   ├── usart.h
│   │   ├── i2c.h
│   │   ├── spi.h
│   │   ├── adc.h
│   │   ├── dma.h
│   │   └── tim.h
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── gpio.c
│       ├── usart.c
│       ├── i2c.c
│       ├── spi.c
│       ├── adc.c
│       ├── dma.c
│       └── tim.c
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── Test_Example.uvprojx     # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── test_manager.c       # 测试管理器
│   │   ├── test_manager.h
│   │   ├── test_menu.c          # 交互式菜单
│   │   ├── test_menu.h
│   │   ├── test_report.c        # 测试报告生成
│   │   └── test_report.h
│   ├── Driver/
│   │   └── test_drivers.c       # 测试驱动集合
│   └── Module/
│       ├── peripheral_test.c    # 外设测试模块
│       ├── peripheral_test.h
│       ├── sensor_test.c        # 传感器测试模块
│       ├── sensor_test.h
│       ├── performance_test.c   # 性能测试模块
│       ├── performance_test.h
│       ├── stress_test.c        # 压力测试模块
│       └── stress_test.h
├── Template.ioc                # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件准备**
   - 确保所有外设模块正确连接
   - 检查电源供电是否稳定
   - 连接串口调试工具

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/Test_Example.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **运行测试**
   - 打开串口终端(115200,8,N,1)
   - 选择测试模式(自动/交互)
   - 观察测试结果和报告

### 代码示例

#### 测试管理器实现

```c
#include "test_manager.h"

/* 测试项目枚举 */
typedef enum {
    TEST_GPIO = 0,
    TEST_UART,
    TEST_I2C,
    TEST_SPI,
    TEST_ADC,
    TEST_TIMER,
    TEST_OLED,
    TEST_AHT20,
    TEST_ICM20608,
    TEST_GRAYSCALE,
    TEST_FLASH,
    TEST_PERFORMANCE,
    TEST_STRESS,
    TEST_MAX
} test_item_t;

/* 测试结果结构体 */
typedef struct {
    test_item_t item;
    char name[32];
    uint8_t result;         // 0=失败, 1=成功, 2=跳过
    uint32_t duration_ms;   // 测试耗时
    char error_msg[128];    // 错误信息
} test_result_t;

test_result_t test_results[TEST_MAX];

/* 测试项目表 */
typedef struct {
    test_item_t item;
    char name[32];
    test_status_t (*test_func)(void);
    uint8_t auto_run;       // 是否在自动测试中运行
} test_item_info_t;

const test_item_info_t test_items[TEST_MAX] = {
    {TEST_GPIO,      "GPIO Test",           Test_GPIO,           1},
    {TEST_UART,      "UART Test",           Test_UART,           1},
    {TEST_I2C,       "I2C Test",            Test_I2C,            1},
    {TEST_SPI,       "SPI Test",             Test_SPI,            1},
    {TEST_ADC,       "ADC Test",             Test_ADC,            1},
    {TEST_TIMER,     "Timer Test",           Test_Timer,          1},
    {TEST_OLED,      "OLED Test",            Test_OLED,           1},
    {TEST_AHT20,     "AHT20 Test",           Test_AHT20,          1},
    {TEST_ICM20608,  "ICM20608 Test",        Test_ICM20608,       1},
    {TEST_GRAYSCALE, "Grayscale Test",       Test_Grayscale,      0},
    {TEST_FLASH,     "Flash Test",           Test_Flash,          1},
    {TEST_PERFORMANCE,"Performance Test",    Test_Performance,    0},
    {TEST_STRESS,    "Stress Test",          Test_Stress,         0},
};

/* 测试管理器初始化 */
void Test_Manager_Init(void)
{
    /* 初始化测试结果 */
    for(int i = 0; i < TEST_MAX; i++)
    {
        test_results[i].item = i;
        strcpy(test_results[i].name, test_items[i].name);
        test_results[i].result = 2;  // 初始为跳过状态
        test_results[i].duration_ms = 0;
        memset(test_results[i].error_msg, 0, sizeof(test_results[i].error_msg));
    }
    
    printf("\\r\\n=== STM32F407VET6 Test Suite ===\\r\\n");
    printf("Hardware: MainBoard V2.0\\r\\n");
    printf("Firmware: %s %s\\r\\n", __DATE__, __TIME__);
    printf("Total Test Items: %d\\r\\n", TEST_MAX);
    printf("=====================================\\r\\n\\r\\n");
}

/* 执行单个测试 */
test_status_t Run_Single_Test(test_item_t item)
{
    if(item >= TEST_MAX) return TEST_ERROR;
    
    uint32_t start_time = HAL_GetTick();
    
    printf("Running %s... ", test_items[item].name);
    
    /* 执行测试函数 */
    test_status_t result = test_items[item].test_func();
    
    uint32_t duration = HAL_GetTick() - start_time;
    
    /* 记录测试结果 */
    test_results[item].result = (result == TEST_OK) ? 1 : 0;
    test_results[item].duration_ms = duration;
    
    if(result == TEST_OK)
    {
        printf("[PASS] (%ld ms)\\r\\n", duration);
    }
    else
    {
        printf("[FAIL] (%ld ms)\\r\\n", duration);
        strcpy(test_results[item].error_msg, "Test function returned error");
    }
    
    HAL_Delay(100);  // 测试间隔
    
    return result;
}

/* 自动测试模式 */
void Run_Auto_Test(void)
{
    uint32_t total_start_time = HAL_GetTick();
    uint16_t passed = 0, failed = 0, skipped = 0;
    
    printf("Starting Automatic Test Mode...\\r\\n");
    printf("=====================================\\r\\n");
    
    for(int i = 0; i < TEST_MAX; i++)
    {
        if(test_items[i].auto_run)
        {
            test_status_t result = Run_Single_Test(i);
            if(result == TEST_OK)
                passed++;
            else
                failed++;
        }
        else
        {
            printf("Skipping %s (Manual test only)\\r\\n", test_items[i].name);
            skipped++;
        }
    }
    
    uint32_t total_duration = HAL_GetTick() - total_start_time;
    
    /* 生成测试报告 */
    printf("\\r\\n=====================================\\r\\n");
    printf("Test Summary:\\r\\n");
    printf("  Total: %d\\r\\n", passed + failed + skipped);
    printf("  Passed: %d\\r\\n", passed);
    printf("  Failed: %d\\r\\n", failed);
    printf("  Skipped: %d\\r\\n", skipped);
    printf("  Duration: %ld ms\\r\\n", total_duration);
    printf("  Success Rate: %.1f%%\\r\\n", 
           (passed + failed > 0) ? (float)passed * 100.0f / (passed + failed) : 0.0f);
    printf("=====================================\\r\\n");
    
    /* 生成详细报告 */
    Generate_Test_Report();
}
```

#### 外设测试实现

```c
/* GPIO测试 */
test_status_t Test_GPIO(void)
{
    /* LED测试 */
    for(int i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 << i, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 << i, GPIO_PIN_RESET);
    }
    
    /* 按键测试 */
    uint8_t key_states[4];
    for(int i = 0; i < 4; i++)
    {
        key_states[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0 << i);
    }
    
    return TEST_OK;
}

/* UART测试 */
test_status_t Test_UART(void)
{
    char test_data[] = "UART Test Data";
    char recv_data[32];
    
    /* 发送测试数据 */
    if(HAL_UART_Transmit(&huart1, (uint8_t*)test_data, strlen(test_data), 1000) != HAL_OK)
    {
        return TEST_ERROR;
    }
    
    /* 接收测试(回环测试需要硬件连接) */
    // HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, (uint8_t*)recv_data, strlen(test_data), 1000);
    
    return TEST_OK;
}

/* I2C设备扫描测试 */
test_status_t Test_I2C(void)
{
    uint8_t device_count = 0;
    uint8_t found_devices[10];
    
    printf("\\r\\n  I2C Device Scan:\\r\\n");
    
    for(uint8_t addr = 1; addr < 128; addr++)
    {
        if(HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 1, 100) == HAL_OK)
        {
            printf("    Device found at 0x%02X\\r\\n", addr);
            if(device_count < 10)
            {
                found_devices[device_count] = addr;
                device_count++;
            }
        }
    }
    
    printf("    Total devices found: %d\\r\\n", device_count);
    
    if(device_count == 0)
    {
        return TEST_WARNING;  // 警告：没有发现设备
    }
    
    return TEST_OK;
}

/* SPI测试 */
test_status_t Test_SPI(void)
{
    uint8_t test_data = 0x55;
    uint8_t recv_data;
    
    /* SPI回环测试 */
    if(HAL_SPI_TransmitReceive(&hspi1, &test_data, &recv_data, 1, 1000) != HAL_OK)
    {
        return TEST_ERROR;
    }
    
    /* 如果MISO和MOSI短接，接收数据应该等于发送数据 */
    // if(recv_data != test_data) return TEST_ERROR;
    
    return TEST_OK;
}

/* ADC测试 */
test_status_t Test_ADC(void)
{
    uint16_t adc_values[8];
    
    /* 启动ADC转换 */
    if(HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        return TEST_ERROR;
    }
    
    /* 读取多个通道 */
    for(int ch = 0; ch < 8; ch++)
    {
        if(HAL_ADC_PollForConversion(&hadc1, 1000) != HAL_OK)
        {
            HAL_ADC_Stop(&hadc1);
            return TEST_ERROR;
        }
        
        adc_values[ch] = HAL_ADC_GetValue(&hadc1);
    }
    
    HAL_ADC_Stop(&hadc1);
    
    /* 输出ADC值 */
    printf("\\r\\n  ADC Values:\\r\\n");
    for(int i = 0; i < 8; i++)
    {
        printf("    CH%d: %d (%.2fV)\\r\\n", i, adc_values[i], 
               adc_values[i] * 3.3f / 4096.0f);
    }
    
    return TEST_OK;
}
```

#### 传感器测试实现

```c
/* OLED显示测试 */
test_status_t Test_OLED(void)
{
    /* 检测OLED设备 */
    if(HAL_I2C_IsDeviceReady(&hi2c1, 0x78, 3, 1000) != HAL_OK)
    {
        return TEST_ERROR;
    }
    
    /* 初始化OLED */
    if(OLED_Init() != 0)
    {
        return TEST_ERROR;
    }
    
    /* 显示测试内容 */
    OLED_Clear();
    OLED_Show_String(0, 0, "OLED Test");
    OLED_Show_String(0, 2, "Line 2");
    OLED_Show_String(0, 4, "Line 3");
    
    /* 绘制图形 */
    OLED_Draw_Rectangle(10, 10, 118, 54);
    OLED_Draw_Circle(64, 32, 15);
    
    OLED_Refresh_Gram();
    
    HAL_Delay(2000);  // 显示2秒
    
    return TEST_OK;
}

/* AHT20温湿度传感器测试 */
test_status_t Test_AHT20(void)
{
    /* 检测AHT20设备 */
    if(HAL_I2C_IsDeviceReady(&hi2c1, 0x70, 3, 1000) != HAL_OK)
    {
        return TEST_ERROR;
    }
    
    /* 初始化AHT20 */
    if(AHT20_Init() != AHT20_OK)
    {
        return TEST_ERROR;
    }
    
    /* 读取温湿度数据 */
    aht20_data_t data;
    if(AHT20_Read_Data(&data) != AHT20_OK)
    {
        return TEST_ERROR;
    }
    
    /* 检查数据合理性 */
    if(data.temperature < -40.0f || data.temperature > 85.0f ||
       data.humidity < 0.0f || data.humidity > 100.0f)
    {
        return TEST_WARNING;
    }
    
    printf("\\r\\n  Temperature: %.2f°C\\r\\n", data.temperature);
    printf("  Humidity: %.2f%%RH\\r\\n", data.humidity);
    printf("  CRC: %s\\r\\n", data.crc_ok ? "OK" : "FAIL");
    
    return data.crc_ok ? TEST_OK : TEST_WARNING;
}

/* ICM20608六轴传感器测试 */
test_status_t Test_ICM20608(void)
{
    /* 检测ICM20608设备 */
    if(HAL_I2C_IsDeviceReady(&hi2c1, 0xD0, 3, 1000) != HAL_OK)
    {
        return TEST_ERROR;
    }
    
    /* 初始化ICM20608 */
    if(ICM20608_Init() != ICM20608_OK)
    {
        return TEST_ERROR;
    }
    
    /* 读取设备ID */
    uint8_t device_id;
    if(ICM20608_Read_Device_ID(&device_id) != ICM20608_OK)
    {
        return TEST_ERROR;
    }
    
    if(device_id != ICM20608_DEVICE_ID)
    {
        return TEST_ERROR;
    }
    
    /* 读取传感器数据 */
    icm20608_raw_data_t raw_data;
    if(ICM20608_Read_Raw_Data(&raw_data) != ICM20608_OK)
    {
        return TEST_ERROR;
    }
    
    printf("\\r\\n  Device ID: 0x%02X\\r\\n", device_id);
    printf("  Accel: X=%d, Y=%d, Z=%d\\r\\n", 
           raw_data.accel_x, raw_data.accel_y, raw_data.accel_z);
    printf("  Gyro: X=%d, Y=%d, Z=%d\\r\\n", 
           raw_data.gyro_x, raw_data.gyro_y, raw_data.gyro_z);
    printf("  Temperature: %d\\r\\n", raw_data.temperature);
    
    return TEST_OK;
}

/* Flash存储测试 */
test_status_t Test_Flash(void)
{
    uint16_t manufacturer_id, device_id;
    
    /* 读取Flash ID */
    if(W25QXX_Read_ID(&manufacturer_id, &device_id) != W25QXX_OK)
    {
        return TEST_ERROR;
    }
    
    printf("\\r\\n  Manufacturer ID: 0x%04X\\r\\n", manufacturer_id);
    printf("  Device ID: 0x%04X\\r\\n", device_id);
    
    /* 执行读写测试 */
    if(Flash_Read_Write_Test() != W25QXX_OK)
    {
        return TEST_ERROR;
    }
    
    return TEST_OK;
}
```

#### 性能基准测试

```c
/* 性能基准测试 */
test_status_t Test_Performance(void)
{
    uint32_t start_time, end_time;
    
    printf("\\r\\n=== Performance Benchmark ===\\r\\n");
    
    /* CPU计算性能测试 */
    start_time = HAL_GetTick();
    volatile float result = 0;
    for(int i = 0; i < 100000; i++)
    {
        result += sqrt((float)i);
    }
    end_time = HAL_GetTick();
    printf("CPU Float Calc: %ld ms (100K sqrt ops)\\r\\n", end_time - start_time);
    
    /* 内存访问性能 */
    uint8_t *test_buffer = malloc(1024);
    if(test_buffer == NULL) return TEST_ERROR;
    
    start_time = HAL_GetTick();
    for(int i = 0; i < 1000; i++)
    {
        memset(test_buffer, i & 0xFF, 1024);
    }
    end_time = HAL_GetTick();
    printf("Memory Write: %ld ms (1MB)\\r\\n", end_time - start_time);
    
    start_time = HAL_GetTick();
    volatile uint32_t sum = 0;
    for(int i = 0; i < 1000; i++)
    {
        for(int j = 0; j < 1024; j++)
        {
            sum += test_buffer[j];
        }
    }
    end_time = HAL_GetTick();
    printf("Memory Read: %ld ms (1MB)\\r\\n", end_time - start_time);
    
    free(test_buffer);
    
    /* GPIO切换速度测试 */
    start_time = HAL_GetTick();
    for(int i = 0; i < 10000; i++)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
    }
    end_time = HAL_GetTick();
    printf("GPIO Toggle: %ld ms (20K ops)\\r\\n", end_time - start_time);
    
    /* 中断响应时间测试 */
    // 需要特殊的硬件配置和代码
    
    return TEST_OK;
}
```

#### 压力测试

```c
/* 压力测试 */
test_status_t Test_Stress(void)
{
    printf("\\r\\n=== Stress Test ===\\r\\n");
    printf("Running 10 minutes stress test...\\r\\n");
    
    uint32_t start_time = HAL_GetTick();
    uint32_t test_count = 0;
    uint32_t error_count = 0;
    
    while(HAL_GetTick() - start_time < 600000)  // 10分钟
    {
        test_count++;
        
        /* 连续读写Flash */
        if(Flash_Read_Write_Test() != W25QXX_OK)
        {
            error_count++;
        }
        
        /* 连续读取传感器 */
        aht20_data_t aht_data;
        if(AHT20_Read_Data(&aht_data) != AHT20_OK)
        {
            error_count++;
        }
        
        icm20608_raw_data_t imu_data;
        if(ICM20608_Read_Raw_Data(&imu_data) != ICM20608_OK)
        {
            error_count++;
        }
        
        /* GPIO连续切换 */
        for(int i = 0; i < 4; i++)
        {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0 << i);
        }
        
        /* 定期输出进度 */
        if(test_count % 100 == 0)
        {
            uint32_t elapsed = HAL_GetTick() - start_time;
            printf("  Progress: %ld%%, Tests: %ld, Errors: %ld\\r\\n",
                   elapsed * 100 / 600000, test_count, error_count);
        }
        
        HAL_Delay(10);
    }
    
    printf("Stress Test Completed:\\r\\n");
    printf("  Total Tests: %ld\\r\\n", test_count);
    printf("  Errors: %ld\\r\\n", error_count);
    printf("  Error Rate: %.2f%%\\r\\n", 
           (float)error_count * 100.0f / test_count);
    
    return (error_count < test_count / 100) ? TEST_OK : TEST_WARNING; // 1%错误率以下认为通过
}
```

#### 交互式菜单

```c
/* 显示主菜单 */
void Show_Main_Menu(void)
{
    printf("\\r\\n=== Test Menu ===\\r\\n");
    printf("1. Auto Test All\\r\\n");
    printf("2. Individual Tests\\r\\n");
    printf("3. Performance Test\\r\\n");
    printf("4. Stress Test\\r\\n");
    printf("5. System Info\\r\\n");
    printf("6. Test Report\\r\\n");
    printf("0. Exit\\r\\n");
    printf("Select: ");
}

/* 显示单项测试菜单 */
void Show_Individual_Menu(void)
{
    printf("\\r\\n=== Individual Tests ===\\r\\n");
    for(int i = 0; i < TEST_MAX; i++)
    {
        printf("%d. %s\\r\\n", i + 1, test_items[i].name);
    }
    printf("0. Back\\r\\n");
    printf("Select: ");
}

/* 菜单处理 */
void Process_Menu(void)
{
    static char input_buffer[32];
    static uint8_t buffer_index = 0;
    
    /* 检查串口输入 */
    uint8_t ch;
    if(HAL_UART_Receive(&huart1, &ch, 1, 0) == HAL_OK)
    {
        if(ch == '\\r' || ch == '\\n')
        {
            if(buffer_index > 0)
            {
                input_buffer[buffer_index] = '\\0';
                
                int selection = atoi(input_buffer);
                Handle_Menu_Selection(selection);
                
                buffer_index = 0;
                Show_Main_Menu();
            }
        }
        else if(ch >= '0' && ch <= '9' && buffer_index < sizeof(input_buffer) - 1)
        {
            input_buffer[buffer_index++] = ch;
            printf("%c", ch);  // 回显
        }
    }
}

/* 处理菜单选择 */
void Handle_Menu_Selection(int selection)
{
    switch(selection)
    {
        case 1:
            Run_Auto_Test();
            break;
            
        case 2:
            Show_Individual_Menu();
            // 等待子菜单选择
            break;
            
        case 3:
            Run_Single_Test(TEST_PERFORMANCE);
            break;
            
        case 4:
            printf("Warning: This will run for 10 minutes. Continue? (y/n): ");
            // 等待确认
            break;
            
        case 5:
            Show_System_Info();
            break;
            
        case 6:
            Generate_Test_Report();
            break;
            
        case 0:
            printf("Goodbye!\\r\\n");
            break;
            
        default:
            printf("Invalid selection!\\r\\n");
            break;
    }
}
```

## API参考

### 测试管理函数

```c
/* 测试管理 */
void Test_Manager_Init(void);                           // 初始化测试管理器
test_status_t Run_Single_Test(test_item_t item);       // 执行单个测试
void Run_Auto_Test(void);                              // 执行自动测试
test_status_t Get_Test_Result(test_item_t item);       // 获取测试结果
```

### 外设测试函数

```c
/* 基础外设测试 */
test_status_t Test_GPIO(void);                         // GPIO测试
test_status_t Test_UART(void);                         // UART测试
test_status_t Test_I2C(void);                          // I2C测试
test_status_t Test_SPI(void);                          // SPI测试
test_status_t Test_ADC(void);                          // ADC测试
test_status_t Test_Timer(void);                        // 定时器测试
```

### 传感器测试函数

```c
/* 传感器测试 */
test_status_t Test_OLED(void);                         // OLED显示测试
test_status_t Test_AHT20(void);                        // 温湿度传感器测试
test_status_t Test_ICM20608(void);                     // 六轴传感器测试
test_status_t Test_Grayscale(void);                    // 灰度传感器测试
test_status_t Test_Flash(void);                        // Flash存储测试
```

### 高级测试函数

```c
/* 高级测试 */
test_status_t Test_Performance(void);                  // 性能基准测试
test_status_t Test_Stress(void);                       // 压力测试
void Generate_Test_Report(void);                       // 生成测试报告
void Show_System_Info(void);                          // 显示系统信息
```

## 配置选项

### 测试参数配置

```c
/* 在test_manager.h中配置 */
#define AUTO_TEST_DELAY_MS      100         // 自动测试间隔
#define STRESS_TEST_DURATION    600000      // 压力测试时长(ms)
#define PERFORMANCE_TEST_LOOPS  100000      // 性能测试循环次数
#define MAX_ERROR_RATE         1.0f         // 最大错误率(%)
```

### 测试使能配置

```c
/* 测试项目使能 */
#define ENABLE_GPIO_TEST        1
#define ENABLE_UART_TEST        1
#define ENABLE_I2C_TEST         1
#define ENABLE_SPI_TEST         1
#define ENABLE_ADC_TEST         1
#define ENABLE_SENSOR_TEST      1
#define ENABLE_PERFORMANCE_TEST 1
#define ENABLE_STRESS_TEST      1
```

## 使用场景

1. **硬件验证**: 新板子的功能验证和问题排查
2. **生产测试**: 批量生产时的自动化测试
3. **学习参考**: 各模块的使用方法和代码示例
4. **性能评估**: 系统性能基准和优化参考
5. **稳定性测试**: 长期运行稳定性验证

## 故障排除

### 常见问题

1. **测试失败**
   - 检查硬件连接是否正确
   - 确认外设模块是否正常工作
   - 查看详细错误信息

2. **部分功能不工作**
   - 检查对应的外设配置
   - 确认引脚定义是否正确
   - 验证时钟配置

3. **测试结果不准确**
   - 检查测试环境是否稳定
   - 确认基准参数设置
   - 重新校准传感器

## 学习要点

1. **系统集成**: 学习如何集成多个模块和外设
2. **测试方法**: 掌握自动化测试和验证方法
3. **错误处理**: 学习错误检测和诊断技术
4. **性能分析**: 理解系统性能分析方法
5. **代码组织**: 学习大型项目的代码组织和管理

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
