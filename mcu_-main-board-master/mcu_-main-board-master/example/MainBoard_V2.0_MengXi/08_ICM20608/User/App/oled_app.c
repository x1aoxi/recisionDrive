#include "oled_app.h"

void Oled_Init(void)
{
  OLED_Init();
  OLED_Clear();
}

extern MOTOR left_motor;
extern MOTOR right_motor;

extern Encoder left_encoder;
extern Encoder right_encoder;

void Oled_Task(void)
{
//  /* 测试灰度传感器 */
//  Oled_Printf(0, 0, "%d-%d-%d-%d-%d-%d-%d-%d",(gray_digtal>>0)&0x01,(gray_digtal>>1)&0x01,(gray_digtal>>2)&0x01,(gray_digtal>>3)&0x01,
//                                               (gray_digtal>>4)&0x01,(gray_digtal>>5)&0x01,(gray_digtal>>6)&0x01,(gray_digtal>>7)&0x01);
  
//  /* 测试电机 */
//  Oled_Printf(0, 0, " left:%d  ", left_motor.speed);
//  Oled_Printf(0, 2, "right:%d  ", right_motor.speed);
  
//  /* 测试编码器 */
//  Oled_Printf(0, 0, " left:%.2fcm/s   ", left_encoder.speed_cm_s);
//  Oled_Printf(0, 2, "right:%.2fcm/s   ", right_encoder.speed_cm_s);
//  Uart_Printf(DEBUG_UART, "left:%.2fcm/s,right:%.2fcm/s\r\n", left_encoder.speed_cm_s, right_encoder.speed_cm_s);
  
  /* 测试 ICM20608 陀螺仪 */
  Oled_Printf(0, 0, " Roll:%.2f    ", icm20608.Roll);
  Oled_Printf(0, 1, "Pitch:%.2f    ", icm20608.Pitch);
  Oled_Printf(0, 2, "  Yaw:%.2f    ", icm20608.Yaw);
  Uart_Printf(DEBUG_UART, "====== ICM20608 数据 ======\r\n");
  Uart_Printf(DEBUG_UART, "3轴陀螺仪:%.2f,%.2f,%.2f\n",(icm20608.gyro.x),(icm20608.gyro.y),(icm20608.gyro.z));
  Uart_Printf(DEBUG_UART, "3轴加速度:%.2f,%.2f,%.2f\n",(icm20608.accel.x),(icm20608.accel.y),(icm20608.accel.z));
  Uart_Printf(DEBUG_UART, "IMU温度℃:%f\n", icm20608.temperature);
  Uart_Printf(DEBUG_UART, "3轴欧拉角:%.2f,%.2f,%.2f\n",(icm20608.Roll),(icm20608.Pitch),(icm20608.Yaw));
}
