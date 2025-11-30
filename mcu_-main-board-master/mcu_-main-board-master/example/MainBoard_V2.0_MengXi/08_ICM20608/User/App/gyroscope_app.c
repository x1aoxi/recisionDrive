#include "gyroscope_app.h"

uint8_t first_gyroscope_flag = 0;

float frist_roll = 0;
float frist_pitch = 0;
float frist_yaw = 0;

#if BNO08x_ON == 0

ICM20608 icm20608;
static uint32_t last_gyro_time = 0;

void Gyroscope_Init(void)
{
  Uart_Printf(DEBUG_UART, "ICM20608 Gyroscope_Init ......\r\n");
  
  // 初始化ICM20608???
  ICM206xx_Init();
  
  // 初始化驱动程序
  Gyroscope_Driver_Init();
  
  // 开始校准
  Gyroscope_Calibrate_Start();
}

void Gyroscope_Task(void)
{
  uint32_t current_time = HAL_GetTick();
  float dt = (current_time - last_gyro_time) / 1000.0f;  // 转换为秒
  
  // 初始化时间
  if (last_gyro_time == 0) {
    last_gyro_time = current_time;
    return;
  }
  
  // 限制时间间隔范围
  if (dt > 0.1f) dt = 0.01f;  // 最大100ms，防止异常
  if (dt < 0.001f) return;    // 最小1ms，避免过于频繁
  
  // 读取原始数据
  ICM206xx_Read_Data(&icm20608.gyro, &icm20608.accel, &icm20608.temperature);
  
  // 静态变量记录上次校准状态
  static uint8_t was_calibrating = 0;
  
  // 检查校准状态变化
  if (was_calibrating && !gyro_calibration.is_calibrating && euler_angles.calibrated) {
    // 校准刚完成，初始化姿态
    Gyroscope_Initialize_Attitude(&icm20608.accel);
  }
  was_calibrating = gyro_calibration.is_calibrating;
  
  // 更新欧拉角解算
  Gyroscope_Update_Euler(&icm20608.gyro, &icm20608.accel, dt);
  
  // 获取解算后的欧拉角
  Gyroscope_Get_Euler_Angles(&icm20608.Pitch, &icm20608.Roll, &icm20608.Yaw);
  
  // 抵消初始的 -179
  if(icm20608.Pitch < 0)
    icm20608.Pitch = 180 + icm20608.Pitch;
  else
    icm20608.Pitch = 180 - icm20608.Pitch;
  
    
  // 记录第一个有效欧拉角
  if (!first_gyroscope_flag && euler_angles.calibrated) {
    frist_roll = icm20608.Roll;
    frist_pitch = icm20608.Pitch;
    frist_yaw = icm20608.Yaw;
    first_gyroscope_flag = 1;
  }
  
  last_gyro_time = current_time;
}

#endif

