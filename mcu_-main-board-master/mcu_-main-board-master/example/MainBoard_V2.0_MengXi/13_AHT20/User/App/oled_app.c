#include "oled_app.h"

void Oled_Init(void)
{
  Uart_Printf(DEBUG_UART, "Oled_Init ......\r\n");
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
  
//  /* 测试 ICM20608 陀螺仪 */
//  Oled_Printf(0, 0, " Roll:%.2f    ", icm20608.Roll);
//  Oled_Printf(0, 1, "Pitch:%.2f    ", icm20608.Pitch);
//  Oled_Printf(0, 2, "  Yaw:%.2f    ", icm20608.Yaw);
//  Uart_Printf(DEBUG_UART, "Euler: %.2f, %.2f, %.2f\n", icm20608.Roll, icm20608.Pitch, icm20608.Yaw);
  
//  /* 测试速度环 */
//  Uart_Printf(DEBUG_UART, "%.2f,%.2f,%.2f,%.2f\r\n", pid_speed_left.target, left_encoder.speed_cm_s, pid_speed_right.target, right_encoder.speed_cm_s);

//    /* 测试张大头步进电机 */
//    Oled_Printf(0, 0, "== ZDT Motor ==");

//    /* 测试 BNO08x */
//    Oled_Printf(0, 0, " Roll:%.2f    ", bno08x.Roll);
//    Oled_Printf(0, 1, "Pitch:%.2f    ", bno08x.Pitch);
//    Oled_Printf(0, 2, "  Yaw:%.2f    ", bno08x.Yaw);
//    Uart_Printf(DEBUG_UART, "Euler: %.2f, %.2f, %.2f\n", bno08x.Roll, bno08x.Pitch, bno08x.Yaw);

  /* 测试 AHT20 温湿度传感器 */
  Oled_Printf(0, 0, "tempature:%.2f    ", Humiture.Temp);
  Oled_Printf(0, 1, " humidity:%.2f    ", Humiture.RH);
  Uart_Printf(DEBUG_UART, "\r\n温度：%.2f \r\n 湿度：%.2f\r\n",Humiture.Temp, Humiture.RH);
}
