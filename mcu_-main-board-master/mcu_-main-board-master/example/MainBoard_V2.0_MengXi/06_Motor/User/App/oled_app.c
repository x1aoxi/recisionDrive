#include "oled_app.h"

void Oled_Init(void)
{
  OLED_Init();
  OLED_Clear();
}

extern MOTOR left_motor;
extern MOTOR right_motor;

void Oled_Task(void)
{
//  /* 测试灰度传感器 */
//  Oled_Printf(0, 0, "%d-%d-%d-%d-%d-%d-%d-%d",(gray_digtal>>0)&0x01,(gray_digtal>>1)&0x01,(gray_digtal>>2)&0x01,(gray_digtal>>3)&0x01,
//                                               (gray_digtal>>4)&0x01,(gray_digtal>>5)&0x01,(gray_digtal>>6)&0x01,(gray_digtal>>7)&0x01);
  
  /* 测试电机 */
  Oled_Printf(0, 0, " left:%d  ", left_motor.speed);
  Oled_Printf(0, 2, "right:%d  ", right_motor.speed);
}
