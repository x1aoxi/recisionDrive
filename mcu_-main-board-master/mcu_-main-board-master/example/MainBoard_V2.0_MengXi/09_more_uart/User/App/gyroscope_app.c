#include "gyroscope_app.h"
uint8_t first_gyroscope_flag = 0;

float frist_roll = 0;
float frist_pitch = 0;
float frist_yaw = 0;
#if BNO080_ON == 0

ICM20608 icm20608;

void Gyroscope_Init(void)
{
  Uart_Printf(DEBUG_UART, "Gyroscope_Init ......\r\n");
  
  ICM206xx_Init();
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
  
  
//  Uart_Printf(DEBUG_UART, "====== ICM20608 数据 ======\r\n");
//  Uart_Printf(DEBUG_UART, "3轴陀螺仪:%.2f,%.2f,%.2f\n",(icm20608.gyro.x),(icm20608.gyro.y),(icm20608.gyro.z));
//  Uart_Printf(DEBUG_UART, "3轴加速度:%.2f,%.2f,%.2f\n",(icm20608.accel.x),(icm20608.accel.y),(icm20608.accel.z));
//  Uart_Printf(DEBUG_UART, "IMU温度℃:%f\n", icm20608.temperature);
//  Uart_Printf(DEBUG_UART, "3轴欧拉角:%.2f,%.2f,%.2f\n",(icm20608.Roll),(icm20608.Pitch),(icm20608.Yaw));
}

#endif

