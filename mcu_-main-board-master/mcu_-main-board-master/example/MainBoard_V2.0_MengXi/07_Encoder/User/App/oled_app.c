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
//  /* ≤‚ ‘ª“∂»¥´∏–∆˜ */
//  Oled_Printf(0, 0, "%d-%d-%d-%d-%d-%d-%d-%d",(gray_digtal>>0)&0x01,(gray_digtal>>1)&0x01,(gray_digtal>>2)&0x01,(gray_digtal>>3)&0x01,
//                                               (gray_digtal>>4)&0x01,(gray_digtal>>5)&0x01,(gray_digtal>>6)&0x01,(gray_digtal>>7)&0x01);
  
//  /* ≤‚ ‘µÁª˙ */
//  Oled_Printf(0, 0, " left:%d  ", left_motor.speed);
//  Oled_Printf(0, 2, "right:%d  ", right_motor.speed);
  
  /* ≤‚ ‘±‡¬Î∆˜ */
  Oled_Printf(0, 0, "L_rpm:%.2fcm/s ", left_encoder.rpm);
  Oled_Printf(0, 1, " left:%.2fcm/s ", left_encoder.speed_cm_s);
  Oled_Printf(0, 2, "R_rpm:%.2fcm/s ", right_encoder.rpm);
  Oled_Printf(0, 3, "right:%.2fcm/s ", right_encoder.speed_cm_s);
  
  Uart_Printf(DEBUG_UART, "%.2f left:%.2fcm/s, %.2f  right:%.2fcm/s\r\n", left_encoder.rpm, left_encoder.speed_cm_s, right_encoder.rpm, right_encoder.speed_cm_s);
}
