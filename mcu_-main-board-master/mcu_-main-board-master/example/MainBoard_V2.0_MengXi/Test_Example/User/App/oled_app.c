#include "oled_app.h"

void* navigator;

unsigned char oled_clear_flag = 0;
unsigned char statr_flag = 0;

void Oled_Init(void)
{
  Uart_Printf(DEBUG_UART, "Oled_Init ......\r\n");
  OLED_Init();
  
  OLED_ShowPic(48, 0, 80, 4, LOGO);

  navigator = menu_builder(getMainItem());
}

void Oled_Task(void)
{
  if(statr_flag == 0)
  {
    oled_clear_flag = 0;
    statr_flag = 1;
    OLED_Clear();
  }
  
  menu_display(navigator);
}
