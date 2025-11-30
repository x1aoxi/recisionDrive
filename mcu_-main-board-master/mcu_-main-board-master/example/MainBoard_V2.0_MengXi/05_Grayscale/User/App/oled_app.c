#include "oled_app.h"

void Oled_Init(void)
{
  OLED_Init();
  OLED_Clear();
}

void Oled_Task(void)
{
  Oled_Printf(0, 0, "%d-%d-%d-%d-%d-%d-%d-%d",(gray_digtal>>0)&0x01,(gray_digtal>>1)&0x01,(gray_digtal>>2)&0x01,(gray_digtal>>3)&0x01,
                                               (gray_digtal>>4)&0x01,(gray_digtal>>5)&0x01,(gray_digtal>>6)&0x01,(gray_digtal>>7)&0x01);
}
