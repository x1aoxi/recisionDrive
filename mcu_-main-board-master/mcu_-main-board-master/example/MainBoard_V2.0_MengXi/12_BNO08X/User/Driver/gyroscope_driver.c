#include "gyroscope_driver.h"

// 使用静态变量来保存上一次的状态
float g_last_yaw = 0.0f;
int g_revolution_count = 0;
bool g_is_yaw_initialized = false;

/**
 * @brief 将一个在[-180, 180]范围内的yaw角度转换为连续的角度值。
 * 
 * @param current_yaw 从传感器读取的当前yaw值 (-180 to 180)。
 * @return float 连续的yaw角度值 (例如 370, -450 等)。
 */
float convert_to_continuous_yaw(float current_yaw) 
{
    // 定义一个阈值来检测“跳变”。这个值应该大于180，通常取270或300比较安全。
    const float WRAP_AROUND_THRESHOLD = 300.0f;

    // 首次调用时进行初始化
    if (!g_is_yaw_initialized) {
        g_last_yaw = current_yaw;
        g_is_yaw_initialized = true;
        g_revolution_count = 0;
    }

    // 计算与上次读数的差异
    float diff = current_yaw - g_last_yaw;

    // 检测是否发生了“跳变”
    if (diff > WRAP_AROUND_THRESHOLD) {
        // 从正角度跳到负角度 (例如, 从 170° 到 -175°), 实际是向右转, 圈数应该增加
        // 此时 diff 接近 -360 (例如 -175 - 170 = -345)
        // 这段逻辑处理的是从-180跳变到+180的情况，说明是向左转过界
        g_revolution_count--;
    } else if (diff < -WRAP_AROUND_THRESHOLD) {
        // 从负角度跳到正角度 (例如, 从 -170° 到 175°), 实际是向左转, 圈数应该减小
        // 此时 diff 接近 360 (例如 175 - (-170) = 345)
        // 这段逻辑处理的是从+180跳变到-180的情况，说明是向右转过界
        g_revolution_count++;
    }

    // 更新上次的yaw值以备下次调用
    g_last_yaw = current_yaw;

    // 计算连续的yaw值
    float continuous_yaw = current_yaw + (float)g_revolution_count * 360.0f;

    return continuous_yaw;
}

#define PI 3.14159265358979f

// 预处理函数
void PreprocessForMadgwick(Vector3f *gyro, Vector3f *accel) 
{
    // 1. 陀螺仪单位转换（度/秒 → 弧度/秒）
    gyro->x *= PI / 180.0f;
    gyro->y *= PI / 180.0f;
    gyro->z *= PI / 180.0f;
    
    // 2. 坐标系调整（根据你的 ICM20608 安装方向）
    // 以下是针对 ICM20608G 的配置
    float temp;
    switch(IMU_ID) {
        case WHO_AM_I_ICM20608D:
        case WHO_AM_I_ICM20608G:
        case WHO_AM_I_ICM20602:
            // 交换 X 和 Y 轴，反转 Z 轴
            temp = accel->x;
            accel->x = accel->y;
            accel->y = temp;
            accel->z = -accel->z;
            
            temp = gyro->x;
            gyro->x = gyro->y;
            gyro->y = temp;
            gyro->z = -gyro->z;
            break;
            
        default:
            // 其他传感器保持默认
            break;
    }
}
