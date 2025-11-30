# STM32F407VET6 SD卡文件系统

## 项目简介
本项目是基于STM32F407VET6微控制器的SD卡文件系统示例，实现了SDIO接口SD卡驱动和FatFs文件系统支持。提供了完整的文件创建、读取、写入操作，以及SD卡信息读取和显示功能。

## 硬件平台
- **主控芯片**: STM32F407VET6
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **存储接口**: SDIO
- **文件系统**: FatFs

## 主要特性
- ✅ SDIO接口SD卡驱动
- ✅ FatFs文件系统支持
- ✅ 文件创建、读取、写入操作
- ✅ SD卡信息读取和显示
- ✅ 支持FAT32文件系统
- ✅ DMA传输支持
- ✅ 错误处理和状态检查

## 硬件连接
```
STM32F407VET6    SD卡接口
PC8             ← → SDIO_D0
PC9             ← → SDIO_D1
PC10            ← → SDIO_D2
PC11            ← → SDIO_D3
PC12            ← → SDIO_CK
PD2             ← → SDIO_CMD
3.3V            ← → VCC
GND             ← → GND
```

## 快速开始

### 1. 硬件准备
- 插入格式化的SD卡(FAT32格式)
- 确保SD卡容量不超过32GB
- 检查SDIO接口连接

### 2. 工程编译
```bash
# 使用Keil MDK-ARM打开工程
# 双击 MDK-ARM/SDIO.uvprojx

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
- 观察串口输出SD卡信息
- 按键操作触发文件读写测试
- 检查SD卡中生成的test.txt文件

## 代码说明

### SD卡初始化
```c
/* 挂载文件系统 */
f_res = f_mount(&fs, "0:", 1);
if (f_res != FR_OK) {
    elog_e(LOG_TAG, "f_mount error (%d)", f_res);
} else {
    elog_i(LOG_TAG, "f_mount success");
}
```

### 文件写入操作
```c
/* 打开文件，如果不存在则创建 */
f_res = f_open(&file, FILE_PATH, FA_CREATE_ALWAYS | FA_WRITE);
if (f_res == FR_OK) {
    /* 写入数据 */
    f_res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
    if (f_res == FR_OK) {
        elog_i(LOG_TAG, "文件写入成功，写入字节数: %d", fnum);
    }
    f_close(&file);
}
```

### 文件读取操作
```c
/* 打开文件进行读取 */
f_res = f_open(&file, FILE_PATH, FA_OPEN_EXISTING | FA_READ);
if (f_res == FR_OK) {
    /* 读取数据 */
    f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
    if (f_res == FR_OK) {
        elog_i(LOG_TAG, "文件读取成功，读取字节数: %d", fnum);
        elog_i(LOG_TAG, "文件内容: %s", ReadBuffer);
    }
    f_close(&file);
}
```

### SD卡信息读取
```c
/* 获取SD卡信息 */
HAL_SD_GetCardInfo(&hsd, &SDCardInfo);
sprintf(sd_info_buffer, "SD卡容量: %llu MB", 
        (uint64_t)SDCardInfo.LogBlockNbr * SDCardInfo.LogBlockSize / 1024 / 1024);
elog_i(LOG_TAG, "%s", sd_info_buffer);
```

## 工程结构
```
04_v2_SDIO/
├── Components/               # 组件库
│   ├── EasyLogger/          # 日志系统组件
│   ├── ebtn/               # 按键处理组件
│   └── MultiTimer/         # 软件定时器组件
├── Core/
│   ├── Inc/                # 头文件
│   └── Src/                # 源文件
│       ├── main.c         # 主程序
│       └── sdio.c         # SDIO配置
├── FATFS/                  # FatFs文件系统
│   ├── App/               # 应用层接口
│   └── Target/            # 底层驱动接口
├── Middlewares/            # 中间件
│   └── Third_Party/FatFs/ # FatFs源码
├── Drivers/                # STM32 HAL驱动
├── MDK-ARM/                # Keil工程文件
├── SDIO.ioc               # STM32CubeMX配置文件
└── README.md              # 本文档
```

## FatFs文件系统

### 支持的文件系统
- FAT12
- FAT16
- FAT32
- exFAT (可选)

### 文件操作函数
```c
f_mount()    // 挂载/卸载文件系统
f_open()     // 打开/创建文件
f_close()    // 关闭文件
f_read()     // 读取文件
f_write()    // 写入文件
f_lseek()    // 移动文件指针
f_truncate() // 截断文件
f_sync()     // 同步文件
f_unlink()   // 删除文件
f_mkdir()    // 创建目录
f_opendir()  // 打开目录
f_readdir()  // 读取目录
```

### 文件打开模式
```c
FA_READ          // 只读模式
FA_WRITE         // 写入模式
FA_OPEN_EXISTING // 打开已存在文件
FA_CREATE_NEW    // 创建新文件
FA_CREATE_ALWAYS // 总是创建文件
FA_OPEN_ALWAYS   // 打开或创建文件
FA_OPEN_APPEND   // 追加模式
```

## 自定义配置

### 修改文件路径
```c
const TCHAR *FILE_PATH = "0:/data/sensor.txt";  // 修改文件路径
```

### 修改写入内容
```c
BYTE WriteBuffer[] = "Custom data content\r\n";  // 自定义写入内容
```

### 配置FatFs参数
在ffconf.h中修改配置：
```c
#define FF_USE_LFN      1    // 启用长文件名支持
#define FF_MAX_LFN      255  // 最大长文件名长度
#define FF_FS_RPATH     1    // 启用相对路径
```

## 应用场景
- 数据记录和存储
- 日志文件管理
- 配置文件读写
- 固件升级存储
- 多媒体文件处理

## 高级功能

### 目录操作
```c
/* 创建目录 */
f_res = f_mkdir("0:/logs");

/* 遍历目录 */
DIR dir;
FILINFO fno;
f_res = f_opendir(&dir, "0:/");
while (f_readdir(&dir, &fno) == FR_OK && fno.fname[0]) {
    elog_i(LOG_TAG, "文件: %s", fno.fname);
}
f_closedir(&dir);
```

### 文件信息获取
```c
FILINFO fno;
f_res = f_stat(FILE_PATH, &fno);
if (f_res == FR_OK) {
    elog_i(LOG_TAG, "文件大小: %lu bytes", fno.fsize);
    elog_i(LOG_TAG, "修改时间: %04d/%02d/%02d %02d:%02d:%02d",
           (fno.fdate >> 9) + 1980, (fno.fdate >> 5) & 15, fno.fdate & 31,
           fno.ftime >> 11, (fno.ftime >> 5) & 63, (fno.ftime & 31) * 2);
}
```

### 磁盘空间查询
```c
FATFS *fs;
DWORD fre_clust, fre_sect, tot_sect;

f_res = f_getfree("0:", &fre_clust, &fs);
if (f_res == FR_OK) {
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
    elog_i(LOG_TAG, "总容量: %lu KB", tot_sect / 2);
    elog_i(LOG_TAG, "可用容量: %lu KB", fre_sect / 2);
}
```

## 注意事项
1. **SD卡格式**: 确保SD卡格式为FAT32
2. **电源稳定**: SD卡操作需要稳定的电源供应
3. **时钟频率**: SDIO时钟频率不要超过SD卡规格
4. **文件关闭**: 操作完成后及时关闭文件
5. **错误处理**: 每次操作后检查返回值

## 故障排除

### 常见问题
1. **挂载失败**: 检查SD卡格式和硬件连接
2. **读写错误**: 检查文件路径和权限
3. **速度慢**: 考虑使用DMA传输

### 调试方法
1. 检查SDIO信号完整性
2. 使用不同品牌SD卡测试
3. 降低SDIO时钟频率
4. 启用详细错误日志

## 性能优化
- 使用DMA传输
- 批量读写操作
- 合理的缓冲区大小
- 避免频繁开关文件

## 扩展功能
- 添加文件加密
- 实现文件压缩
- 支持多分区
- 添加文件系统检查

## API参考

### SDIO函数
- `HAL_SD_Init()` - SD卡初始化
- `HAL_SD_GetCardInfo()` - 获取SD卡信息
- `HAL_SD_ReadBlocks()` - 读取数据块
- `HAL_SD_WriteBlocks()` - 写入数据块

### FatFs函数
- `f_mount()` - 挂载文件系统
- `f_open()` - 打开文件
- `f_read()` - 读取文件
- `f_write()` - 写入文件

## 版本历史
- v1.0.0 - 初始版本，基本文件操作
- v1.0.1 - 添加目录操作和文件信息
- v1.0.2 - 优化错误处理和性能

## 许可证
本项目基于MIT许可证开源。
