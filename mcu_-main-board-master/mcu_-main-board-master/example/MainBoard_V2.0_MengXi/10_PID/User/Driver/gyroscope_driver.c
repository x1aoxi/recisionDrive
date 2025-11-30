#include "gyroscope_driver.h"

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
