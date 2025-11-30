# STM32F407VET6 W25QXX SPI Flash存储器

## 项目简介
本项目是基于STM32F407VET6微控制器的W25QXX系列SPI Flash存储器驱动示例，实现了Flash芯片的读取、写入、擦除等基本操作。支持多种W25QXX系列芯片，提供了完整的设备检测、数据验证和错误处理功能。

## 硬件平台
- **主控芯片**: STM32F407VET6
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **存储芯片**: W25QXX系列SPI Flash
- **通信接口**: SPI1

## 主要特性
- ✅ SPI接口Flash驱动
- ✅ 扇区擦除、页编程
- ✅ 数据读取和验证
- ✅ 设备ID检测
- ✅ 支持多种W25QXX芯片
- ✅ 写保护控制
- ✅ 状态寄存器操作

## 硬件连接
```
STM32F407VET6    W25QXX Flash
PA5             ← → SCK (SPI1_SCK)
PA6             ← → DO/MISO (SPI1_MISO)
PA7             ← → DI/MOSI (SPI1_MOSI)
PA4             ← → CS (SPI1_NSS)
3.3V            ← → VCC
GND             ← → GND
```

## 支持的芯片型号
- W25Q80 (1MB)
- W25Q16 (2MB)
- W25Q32 (4MB)
- W25Q64 (8MB)
- W25Q128 (16MB)
- W25Q256 (32MB)

## 快速开始

### 1. 硬件准备
- 连接W25QXX Flash芯片到SPI1接口
- 确保电源和地线连接正确
- 检查SPI信号线连接

### 2. 工程编译
```bash
# 使用Keil MDK-ARM打开工程
# 双击 MDK-ARM/w25qxx.uvprojx

# 编译工程
# 按F7或点击Build按钮
```

### 3. 程序下载
```bash
# 连接ST-Link调试器
# 在Keil中配置调试设置
# 按F8下载程序到Flash
```

### 4. 运行测试
- 复位开发板
- 观察串口输出Flash信息
- 按键操作触发读写测试
- 检查数据读写是否正确

## 代码说明

### Flash初始化
```c
/* 初始化SPI Flash */
if (gd25qxx_init() == 0) {
    elog_i("flash", "W25QXX初始化成功");
    
    /* 读取设备ID */
    uint32_t flash_id = gd25qxx_read_id();
    elog_i("flash", "Flash ID: 0x%06X", flash_id);
} else {
    elog_e("flash", "W25QXX初始化失败");
}
```

### 扇区擦除
```c
/* 擦除扇区 */
uint32_t sector_addr = 0x000000;  // 扇区地址
if (gd25qxx_sector_erase(sector_addr) == 0) {
    elog_i("flash", "扇区擦除成功");
} else {
    elog_e("flash", "扇区擦除失败");
}
```

### 页编程
```c
/* 页编程 */
uint8_t write_data[256] = "Hello W25QXX Flash!";
uint32_t page_addr = 0x000000;  // 页地址

if (gd25qxx_page_program(page_addr, write_data, sizeof(write_data)) == 0) {
    elog_i("flash", "页编程成功");
} else {
    elog_e("flash", "页编程失败");
}
```

### 数据读取
```c
/* 读取数据 */
uint8_t read_data[256];
uint32_t read_addr = 0x000000;  // 读取地址

if (gd25qxx_read_data(read_addr, read_data, sizeof(read_data)) == 0) {
    elog_i("flash", "数据读取成功");
    elog_i("flash", "读取内容: %s", read_data);
} else {
    elog_e("flash", "数据读取失败");
}
```

## 工程结构
```
05_v2_W25QXX/
├── Components/               # 组件库
│   ├── gd25qxx/             # W25QXX驱动组件
│   │   ├── gd25qxx.h       # 驱动头文件
│   │   └── gd25qxx.c       # 驱动源文件
│   ├── EasyLogger/          # 日志系统组件
│   ├── ebtn/               # 按键处理组件
│   └── MultiTimer/         # 软件定时器组件
├── Core/
│   ├── Inc/                # 头文件
│   └── Src/                # 源文件
│       ├── main.c         # 主程序
│       └── spi.c          # SPI配置
├── Drivers/                # STM32 HAL驱动
├── MDK-ARM/                # Keil工程文件
├── w25qxx.ioc             # STM32CubeMX配置文件
└── README.md              # 本文档
```

## Flash存储特性

### 存储结构
- **页大小**: 256字节
- **扇区大小**: 4KB (16页)
- **块大小**: 64KB (16扇区)
- **芯片容量**: 根据型号不同

### 操作特点
- **擦除单位**: 扇区(4KB)、块(64KB)、整片
- **编程单位**: 页(256字节)
- **读取单位**: 字节级别
- **擦除状态**: 0xFF

### 时序要求
- **页编程时间**: 典型值0.7ms
- **扇区擦除时间**: 典型值30ms
- **块擦除时间**: 典型值120ms
- **整片擦除时间**: 典型值40s

## API参考

### 初始化函数
```c
int gd25qxx_init(void);                    // 初始化Flash
int gd25qxx_deinit(void);                  // 反初始化Flash
```

### 信息读取函数
```c
uint32_t gd25qxx_read_id(void);            // 读取设备ID
uint8_t gd25qxx_read_status_reg(void);     // 读取状态寄存器
```

### 擦除函数
```c
int gd25qxx_sector_erase(uint32_t addr);   // 扇区擦除
int gd25qxx_block_erase(uint32_t addr);    // 块擦除
int gd25qxx_chip_erase(void);              // 整片擦除
```

### 读写函数
```c
int gd25qxx_read_data(uint32_t addr, uint8_t *data, uint32_t len);    // 读取数据
int gd25qxx_page_program(uint32_t addr, uint8_t *data, uint32_t len); // 页编程
```

### 保护函数
```c
int gd25qxx_write_enable(void);            // 写使能
int gd25qxx_write_disable(void);           // 写禁止
int gd25qxx_wait_busy(void);               // 等待操作完成
```

## 自定义配置

### 修改SPI接口
在gd25qxx.c中修改SPI句柄：
```c
extern SPI_HandleTypeDef hspi2;  // 使用SPI2接口
#define W25QXX_SPI_HANDLE &hspi2
```

### 修改CS引脚
```c
#define W25QXX_CS_PORT    GPIOB
#define W25QXX_CS_PIN     GPIO_PIN_12
```

### 配置SPI参数
在spi.c中修改配置：
```c
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;  // 修改时钟分频
hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;              // 修改时钟极性
```

## 应用场景
- 程序代码存储
- 数据备份和归档
- 配置参数保存
- 日志文件存储
- 固件升级缓存

## 高级功能

### 快速读取
```c
/* 快速读取模式 */
int gd25qxx_fast_read(uint32_t addr, uint8_t *data, uint32_t len) {
    // 发送快速读取命令(0x0B)
    // 添加虚拟字节
    // 读取数据
}
```

### 双线/四线模式
```c
/* 双线读取模式 */
int gd25qxx_dual_read(uint32_t addr, uint8_t *data, uint32_t len);

/* 四线读取模式 */
int gd25qxx_quad_read(uint32_t addr, uint8_t *data, uint32_t len);
```

### 写保护设置
```c
/* 设置写保护 */
int gd25qxx_set_write_protect(uint8_t protect_level) {
    uint8_t status = gd25qxx_read_status_reg();
    status |= (protect_level << 2);  // 设置保护位
    return gd25qxx_write_status_reg(status);
}
```

## 注意事项
1. **擦除顺序**: 写入前必须先擦除
2. **页边界**: 页编程不能跨页边界
3. **写使能**: 每次写操作前需要写使能
4. **忙状态**: 操作期间需要检查忙状态
5. **电源稳定**: 操作期间保持电源稳定

## 故障排除

### 常见问题
1. **设备ID读取失败**: 检查SPI连接和时序
2. **写入失败**: 确认写使能和擦除操作
3. **数据错误**: 检查地址对齐和长度

### 调试方法
1. 使用逻辑分析仪检查SPI信号
2. 验证CS信号时序
3. 检查电源电压稳定性
4. 逐步测试基本操作

## 性能优化
- 使用DMA传输
- 批量操作减少开销
- 合理规划存储布局
- 避免频繁擦除

## 扩展功能
- 添加文件系统支持
- 实现磨损均衡
- 添加数据校验
- 支持OTA升级

## 版本历史
- v1.0.0 - 初始版本，基本读写功能
- v1.0.1 - 添加设备检测和错误处理
- v1.0.2 - 优化SPI时序和性能

## 许可证
本项目基于MIT许可证开源。
