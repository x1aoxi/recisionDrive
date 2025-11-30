#include "oled_app.h"

void Oled_Init(void)
{
  OLED_Init();
  OLED_Clear();
}

void Oled_Task(void)
{
  Oled_Printf(0, 0, "Hello World!");
}
