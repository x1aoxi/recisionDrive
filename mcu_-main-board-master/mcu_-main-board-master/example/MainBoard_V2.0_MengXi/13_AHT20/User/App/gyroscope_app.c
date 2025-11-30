#include "gyroscope_app.h"

uint8_t first_gyroscope_flag = 0;

float frist_roll = 0;
float frist_pitch = 0;
float frist_yaw = 0;

#if BNO08x_ON == 0

#define ICM20608_TIME 0.0004f

uint32_t icm20608_first_tick;

ICM20608 icm20608;

void Gyroscope_Init(void)
{
  Uart_Printf(DEBUG_UART, "ICM20608 Gyroscope_Init ......\r\n");
  
  ICM206xx_Init();
  
  icm20608_first_tick = HAL_GetTick();
}

void Gyroscope_Task(void)
{
  ICM206xx_Read_Data(&icm20608.gyro,&icm20608.accel,&icm20608.temperature);
  PreprocessForMadgwick(&icm20608.gyro, &icm20608.accel);
  imu_filter(icm20608.accel.x, icm20608.accel.y, icm20608.accel.z, icm20608.gyro.x, icm20608.gyro.y, icm20608.gyro.z);
  eulerAngles(q_est, &icm20608.Roll, &icm20608.Pitch, &icm20608.Yaw);
  
  /* 将当前位置归零 */
  if(first_gyroscope_flag == 0)
  {
    first_gyroscope_flag = 1;
    frist_roll = icm20608.Roll;
    frist_pitch = icm20608.Pitch;
    frist_yaw = icm20608.Yaw;
  }
  
  icm20608.Roll = icm20608.Roll - frist_roll;
  icm20608.Pitch = icm20608.Pitch - frist_pitch;
  icm20608.Yaw = icm20608.Yaw - frist_yaw;
  
  icm20608.Yaw = convert_to_continuous_yaw(icm20608.Yaw); 
  
  if(icm20608.Yaw >= 0)
    icm20608.Yaw -= (HAL_GetTick() - icm20608_first_tick) * ICM20608_TIME;
  else
    icm20608.Yaw += (HAL_GetTick() - icm20608_first_tick) * ICM20608_TIME;
  
}

#else

BNO08x bno08x;

void Gyroscope_Init(void)
{
  Uart_Printf(DEBUG_UART, "BNO08x Gyroscope_Init ......\r\n");
  
  // 1. 硬件初始化
  BNO080_Init(&hi2c1, BNO080_DEFAULT_ADDRESS);
  Uart_Printf(DEBUG_UART, "BNO080 I2C初始化完成\n");

  // 2. 硬件复位（推荐使用，更可靠）
  if (BNO080_HardwareReset() == 0) {
      Uart_Printf(DEBUG_UART, "BNO080硬件复位成功\n");
  } else {
      Uart_Printf(DEBUG_UART, "BNO080硬件复位失败，尝试软件复位\n");
      // 备用方案：软件复位
      softReset();
      HAL_Delay(100);
      Uart_Printf(DEBUG_UART, "BNO080软件复位完成\n");
  }

  // 3. 基础传感器配置（用户可选择性注释/取消注释）
  enableRotationVector(100);        // 启用旋转向量，100ms间隔
  Uart_Printf(DEBUG_UART, "已启用旋转向量报告 (100ms)\n");

  // enableAccelerometer(50);          // 启用加速度计，50ms间隔
  // Uart_Printf(DEBUG_UART, "已启用加速度计报告 (50ms)\n");

  // enableGyro(50);                   // 启用陀螺仪，50ms间隔
  // Uart_Printf(DEBUG_UART, "已启用陀螺仪报告 (50ms)\n");

  // enableMagnetometer(100);          // 启用磁力计，100ms间隔
  // Uart_Printf(DEBUG_UART, "已启用磁力计报告 (100ms)\n");

  // enableLinearAccelerometer(50);    // 启用线性加速度，50ms间隔
  // Uart_Printf(DEBUG_UART, "已启用线性加速度报告 (50ms)\n");

  // enableStepCounter(1000);          // 启用步数计数，1000ms间隔
  // Uart_Printf(DEBUG_UART, "已启用步数计数器 (1000ms)\n");

  // 4. 高级功能配置（可选）
  // enableGameRotationVector(20);     // 游戏模式旋转向量，20ms间隔
  // Uart_Printf(DEBUG_UART, "已启用游戏旋转向量 (20ms)\n");

  // enableStabilityClassifier(500);   // 稳定性分类器，500ms间隔
  // Uart_Printf(DEBUG_UART, "已启用稳定性分类器 (500ms)\n");

  HAL_Delay(200); // 等待传感器配置生效
  Uart_Printf(DEBUG_UART, "BNO080传感器初始化完成！\n");
  
}

void Gyroscope_Task(void)
{
  // 检查是否有新数据
    if (dataAvailable()) 
    {

      // 1. 四元数和姿态数据（基础功能）
      float quat_i = getQuatI();
      float quat_j = getQuatJ();
      float quat_k = getQuatK();
      float quat_real = getQuatReal();
    
      QuaternionToEulerAngles(quat_i, quat_j, quat_k, quat_real, &bno08x.Roll, &bno08x.Pitch, &bno08x.Yaw);
      
      /* 将当前位置归零 */
      if(first_gyroscope_flag == 0)
      {
        first_gyroscope_flag = 1;
        frist_roll = bno08x.Roll;
        frist_pitch = bno08x.Pitch;
        frist_yaw = bno08x.Yaw;
      }
      
      bno08x.Roll = bno08x.Roll - frist_roll;
      bno08x.Pitch = bno08x.Pitch - frist_pitch;
      bno08x.Yaw = bno08x.Yaw - frist_yaw;
    
      bno08x.Yaw = convert_to_continuous_yaw(bno08x.Yaw); 
      
      // 2. 加速度数据（可选）
      // float accel_x = getAccelX();
      // float accel_y = getAccelY();
      // float accel_z = getAccelZ();
      // Uart_Printf(&huart1, "Accel: %.3f, %.3f, %.3f g\n", accel_x, accel_y, accel_z);

      // 3. 陀螺仪数据（可选）
      // float gyro_x = getGyroX();
      // float gyro_y = getGyroY();
      // float gyro_z = getGyroZ();
      // Uart_Printf(&huart1, "Gyro: %.3f, %.3f, %.3f rad/s\n", gyro_x, gyro_y, gyro_z);

      // 4. 磁力计数据（可选）
      // float mag_x = getMagX();
      // float mag_y = getMagY();
      // float mag_z = getMagZ();
      // Uart_Printf(&huart1, "Mag: %.1f, %.1f, %.1f μT\n", mag_x, mag_y, mag_z);

      // 5. 精度监控（调试用）
      // uint8_t quat_accuracy = getQuatAccuracy();
      // uint8_t accel_accuracy = getAccelAccuracy();
      // uint8_t gyro_accuracy = getGyroAccuracy();
      // uint8_t mag_accuracy = getMagAccuracy();
      // Uart_Printf(&huart1, "Accuracy: Q=%d A=%d G=%d M=%d\n",
      //           quat_accuracy, accel_accuracy, gyro_accuracy, mag_accuracy);

      // 6. 高级功能数据（可选）
      // uint16_t steps = getStepCount();
      // uint8_t stability = getStabilityClassifier();
      // Uart_Printf(&huart1, "Steps: %d, Stability: %d\n", steps, stability);
  }
}

#endif

