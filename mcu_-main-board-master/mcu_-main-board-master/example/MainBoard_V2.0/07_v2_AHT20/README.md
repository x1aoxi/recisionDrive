# STM32F407VET6 AHT20温湿度传感器

## 项目简介
本项目是基于STM32F407VET6微控制器的AHT20温湿度传感器驱动示例，实现了高精度的温度和湿度数据采集。AHT20是一款高精度、完全校准的数字温湿度传感器，具有优异的长期稳定性和快速响应特性。

## 硬件平台
- **主控芯片**: STM32F407VET6
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **传感器**: AHT20温湿度传感器
- **通信接口**: I2C1

## 主要特性
- ✅ I2C接口传感器驱动
- ✅ 温度和湿度数据读取
- ✅ 高精度测量(±0.3°C, ±2%RH)
- ✅ 自动校准功能
- ✅ 低功耗设计
- ✅ 快速响应时间

## 硬件连接
```
STM32F407VET6    AHT20传感器
PB6             ← → SCL (I2C1_SCL)
PB7             ← → SDA (I2C1_SDA)
3.3V            ← → VCC
GND             ← → GND
```

## 传感器规格
- **温度范围**: -40°C ~ +85°C
- **湿度范围**: 0% ~ 100% RH
- **温度精度**: ±0.3°C (典型值)
- **湿度精度**: ±2% RH (典型值)
- **分辨率**: 温度0.01°C, 湿度0.024% RH
- **响应时间**: 温度5s, 湿度8s
- **工作电压**: 2.2V ~ 5.5V
- **工作电流**: 测量时980μA, 休眠时0.25μA

## 快速开始

### 1. 硬件准备
- 连接AHT20传感器到I2C1接口
- 确保电源和地线连接正确
- 检查I2C上拉电阻(通常4.7kΩ)

### 2. 工程编译
```bash
# 使用Keil MDK-ARM打开工程
# 双击 MDK-ARM/AHT20.uvprojx

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
- 观察串口输出温湿度数据
- 按键操作触发单次测量
- 验证测量数据的准确性

## 代码说明

### 传感器初始化
```c
/* 初始化AHT20传感器 */
if (aht20_init(&hi2c1) == AHT20_OK) {
    elog_i("aht20", "AHT20初始化成功");
    
    /* 检查传感器状态 */
    uint8_t status = aht20_get_status();
    if (status & AHT20_STATUS_CALIBRATED) {
        elog_i("aht20", "传感器已校准");
    }
} else {
    elog_e("aht20", "AHT20初始化失败");
}
```

### 触发测量
```c
/* 触发温湿度测量 */
if (aht20_trigger_measurement() == AHT20_OK) {
    elog_i("aht20", "测量命令发送成功");
    
    /* 等待测量完成 */
    HAL_Delay(80);  // 等待80ms测量完成
} else {
    elog_e("aht20", "测量命令发送失败");
}
```

### 读取数据
```c
/* 读取温湿度数据 */
float temperature, humidity;

if (aht20_read_data(&temperature, &humidity) == AHT20_OK) {
    elog_i("aht20", "温度: %.2f°C", temperature);
    elog_i("aht20", "湿度: %.2f%%RH", humidity);
} else {
    elog_e("aht20", "数据读取失败");
}
```

### 一次性测量
```c
/* 一次性测量函数 */
aht20_result_t aht20_measure_once(float *temp, float *humi) {
    aht20_result_t result;
    
    /* 触发测量 */
    result = aht20_trigger_measurement();
    if (result != AHT20_OK) return result;
    
    /* 等待测量完成 */
    HAL_Delay(80);
    
    /* 读取数据 */
    result = aht20_read_data(temp, humi);
    return result;
}
```

## 工程结构
```
07_v2_AHT20/
├── Components/               # 组件库
│   ├── aht20/               # AHT20驱动组件
│   │   ├── aht20.h         # 驱动头文件
│   │   └── aht20.c         # 驱动源文件
│   ├── EasyLogger/          # 日志系统组件
│   ├── ebtn/               # 按键处理组件
│   └── MultiTimer/         # 软件定时器组件
├── Core/
│   ├── Inc/                # 头文件
│   └── Src/                # 源文件
│       ├── main.c         # 主程序
│       └── i2c.c          # I2C配置
├── Drivers/                # STM32 HAL驱动
├── MDK-ARM/                # Keil工程文件
├── AHT20.ioc              # STM32CubeMX配置文件
└── README.md              # 本文档
```

## API参考

### 初始化函数
```c
aht20_result_t aht20_init(I2C_HandleTypeDef *hi2c);        // 初始化传感器
aht20_result_t aht20_deinit(void);                         // 反初始化传感器
aht20_result_t aht20_reset(void);                          // 软件复位
```

### 状态函数
```c
uint8_t aht20_get_status(void);                            // 获取状态寄存器
aht20_result_t aht20_check_calibration(void);              // 检查校准状态
```

### 测量函数
```c
aht20_result_t aht20_trigger_measurement(void);            // 触发测量
aht20_result_t aht20_read_data(float *temp, float *humi);  // 读取数据
aht20_result_t aht20_measure_once(float *temp, float *humi); // 一次性测量
```

### 配置函数
```c
aht20_result_t aht20_set_measurement_mode(uint8_t mode);   // 设置测量模式
aht20_result_t aht20_calibrate(void);                     // 手动校准
```

## 数据处理

### 温度计算
```c
/* 原始数据转换为温度值 */
float calculate_temperature(uint32_t raw_data) {
    return ((float)raw_data / 1048576.0f) * 200.0f - 50.0f;
}
```

### 湿度计算
```c
/* 原始数据转换为湿度值 */
float calculate_humidity(uint32_t raw_data) {
    return ((float)raw_data / 1048576.0f) * 100.0f;
}
```

### 数据滤波
```c
/* 简单移动平均滤波 */
#define FILTER_SIZE 5
float temp_buffer[FILTER_SIZE];
float humi_buffer[FILTER_SIZE];
uint8_t filter_index = 0;

float filter_temperature(float new_temp) {
    temp_buffer[filter_index] = new_temp;
    filter_index = (filter_index + 1) % FILTER_SIZE;
    
    float sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++) {
        sum += temp_buffer[i];
    }
    return sum / FILTER_SIZE;
}
```

## 自定义配置

### 修改I2C接口
```c
extern I2C_HandleTypeDef hi2c2;  // 使用I2C2接口
#define AHT20_I2C_HANDLE &hi2c2
```

### 修改I2C地址
```c
#define AHT20_I2C_ADDRESS    0x39  // 修改I2C地址(7位地址)
```

### 配置测量间隔
```c
#define MEASUREMENT_INTERVAL 2000  // 2秒测量间隔
```

## 应用场景
- 环境监测系统
- 气象站数据采集
- 智能家居控制
- 温室大棚监控
- 工业过程控制
- 医疗设备环境监测

## 高级功能

### 连续测量模式
```c
void continuous_measurement_task(void) {
    static uint32_t last_measure_time = 0;
    
    if (HAL_GetTick() - last_measure_time >= MEASUREMENT_INTERVAL) {
        float temp, humi;
        if (aht20_measure_once(&temp, &humi) == AHT20_OK) {
            /* 处理测量数据 */
            process_sensor_data(temp, humi);
        }
        last_measure_time = HAL_GetTick();
    }
}
```

### 阈值报警
```c
void check_thresholds(float temp, float humi) {
    if (temp > TEMP_HIGH_THRESHOLD) {
        elog_w("aht20", "温度过高: %.2f°C", temp);
        // 触发高温报警
    }
    
    if (humi > HUMI_HIGH_THRESHOLD) {
        elog_w("aht20", "湿度过高: %.2f%%RH", humi);
        // 触发高湿报警
    }
}
```

### 数据记录
```c
typedef struct {
    uint32_t timestamp;
    float temperature;
    float humidity;
} sensor_record_t;

void save_sensor_record(float temp, float humi) {
    sensor_record_t record;
    record.timestamp = HAL_GetTick();
    record.temperature = temp;
    record.humidity = humi;
    
    /* 保存到Flash或SD卡 */
    save_to_storage(&record, sizeof(record));
}
```

## 注意事项
1. **上拉电阻**: I2C总线需要上拉电阻(4.7kΩ)
2. **测量时间**: 等待足够的测量时间(80ms)
3. **校准状态**: 使用前检查传感器校准状态
4. **电源稳定**: 保持稳定的电源供应
5. **环境影响**: 避免强光直射和气流影响

## 故障排除

### 常见问题
1. **初始化失败**: 检查I2C连接和地址
2. **数据异常**: 检查校准状态和测量时间
3. **通信错误**: 检查I2C时序和上拉电阻

### 调试方法
1. 使用逻辑分析仪检查I2C信号
2. 验证传感器地址和命令
3. 检查电源电压稳定性
4. 逐步测试基本通信

## 性能优化
- 使用DMA传输
- 实现异步测量
- 添加数据缓存
- 优化测量频率

## 扩展功能
- 添加温度补偿
- 实现自动校准
- 集成数据上传
- 支持多传感器

## 版本历史
- v1.0.0 - 初始版本，基本测量功能
- v1.0.1 - 添加数据滤波和校准检查
- v1.0.2 - 优化I2C通信和错误处理

## 许可证
本项目基于MIT许可证开源。
