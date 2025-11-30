#include "menu_navigator.h"
#include <cstdint>
#include <cstring>

extern "C" {
  #include "MyDefine.h"
  
  extern Encoder left_encoder;
  extern Encoder right_encoder;
  
  extern MOTOR left_motor;
  extern MOTOR right_motor;
}

using namespace Menu;

// 生成的变量定义
struct g_menu_variables_t
{
    bool LED1 = false;
    bool LED2 = false;
    bool LED3 = false;
    bool LED4 = false;
    int16_t Left_Speed = basic_speed;
    int16_t Right_Speed = basic_speed;
    float Left_Kp = pid_params_left.kp;
    float Left_Ki = pid_params_left.ki;
    float Left_Kd = pid_params_left.kd;
    float Right_Kp = pid_params_right.kp;
    float Right_Ki = pid_params_right.ki;
    float Right_Kd = pid_params_right.kd;
} g_menu_vars;


// 生成的回调函数和参数指针数组
void X_Left_Turn(void** args) {
    /* 回调函数 */
    Motor_Vel_Synchronous_Control(-100, Get_Y_Step_Motor_Speed());
}

void X_Right_Turn(void** args) {
    /* 回调函数 */
    Motor_Vel_Synchronous_Control(100, Get_Y_Step_Motor_Speed());
}

void Y_Left_Turn(void** args) {
    /* 回调函数 */
    Motor_Vel_Synchronous_Control(Get_X_Step_Motor_Speed(), -100);
}

void Y_Right_Turn(void** args) {
    /* 回调函数 */
    Motor_Vel_Synchronous_Control(Get_X_Step_Motor_Speed(), 100);
}

void Stop_Turn(void** args) {
    /* 回调函数 */
    Motor_Vel_Synchronous_Control(0, 0);
}

void Start_Motor(void** args) {
    /* 回调函数 */
    pid_running = 1;
}

void Stop_Motor(void** args) {
    /* 回调函数 */
    pid_running = 0;
    Motor_Stop(&left_motor);
    Motor_Stop(&right_motor);
}

void Reset_Count(void** args) {
    /* 回调函数 */
    start_count = 0;
  
    spi_flash_sector_erase(0x1000);
    spi_flash_buffer_write(&start_count, 0x1000, 1);
}

// Left_Turn 回调函数参数指针数组
void* Left_TurnArgs[] = {
       nullptr // 请自行添加参数指针
};

// Right_Turn 回调函数参数指针数组
void* Right_TurnArgs[] = {
       nullptr // 请自行添加参数指针
};

// Stop_Turn 回调函数参数指针数组
void* Stop_TurnArgs[] = {
       nullptr // 请自行添加参数指针
};

// Reset_Count 回调函数参数指针数组
void* Reset_CountArgs[] = {
       nullptr // 请自行添加参数指针
};


// 生成的菜单项定义
menuItem* ledChildren[] = {
    menuItem::createToggle("LED1", &g_menu_vars.LED1, [](const bool state) {
        /* 回调函数 */
        led_buf[0] = state;
    }),
    menuItem::createToggle("LED2", &g_menu_vars.LED2, [](const bool state) {
        /* 回调函数 */
        led_buf[1] = state;
    }),
    menuItem::createToggle("LED3", &g_menu_vars.LED3, [](const bool state) {
        /* 回调函数 */
        led_buf[2] = state;
    }),
    menuItem::createToggle("LED4", &g_menu_vars.LED4, [](const bool state) {
        /* 回调函数 */
        led_buf[3] = state;
    }),
};

menuItem* motorChildren[] = {
    menuItem::createChangeableItem<int16_t>("Left", g_menu_vars.Left_Speed, 0, 100, 1, [](const int16_t value) {
        /* 回调函数 */
        pid_set_target(&pid_speed_left, value);
  
    }),
    menuItem::createChangeableItem<int16_t>("Right", g_menu_vars.Right_Speed, 0, 100, 1, [](const int16_t value) {
        /* 回调函数 */
        pid_set_target(&pid_speed_right, value);
    }),
    
    menuItem::createApp("Start", nullptr, Start_Motor),
      
    menuItem::createApp("Stop", nullptr, Stop_Motor),
};

menuItem* pid_speed_leftChildren[] = {
    menuItem::createChangeableItem<float>("Kp", g_menu_vars.Left_Kp, 0, 100, 0.1, [](const float value) {
        /* 回调函数 */
        pid_speed_left.kp = value;
        pid_set_params(&pid_speed_left, pid_speed_left.kp, pid_speed_left.ki, pid_speed_left.kd);
    }),
    menuItem::createChangeableItem<float>("Ki", g_menu_vars.Left_Ki, 0, 100, 0.1, [](const float value) {
        /* 回调函数 */
        pid_speed_left.ki = value;
        pid_set_params(&pid_speed_left, pid_speed_left.kp, pid_speed_left.ki, pid_speed_left.kd);
    }),
    menuItem::createChangeableItem<float>("Kd", g_menu_vars.Left_Kd, 0, 100, 0.1, [](const float value) {
        /* 回调函数 */
        pid_speed_left.kd = value;
        pid_set_params(&pid_speed_left, pid_speed_left.kp, pid_speed_left.ki, pid_speed_left.kd);
    }),
};

menuItem* pid_speed_rightChildren[] = {
    menuItem::createChangeableItem<float>("Kp", g_menu_vars.Right_Kp, 0, 100, 0.1, [](const float value) {
        /* 回调函数 */
        pid_speed_right.kp = value;
        pid_set_params(&pid_speed_right, pid_speed_right.kp, pid_speed_right.ki, pid_speed_right.kd);
    }),
    menuItem::createChangeableItem<float>("Ki", g_menu_vars.Right_Ki, 0, 100, 0.1, [](const float value) {
        /* 回调函数 */
        pid_speed_right.ki = value;
        pid_set_params(&pid_speed_right, pid_speed_right.kp, pid_speed_right.ki, pid_speed_right.kd);
    }),
    menuItem::createChangeableItem<float>("Kd", g_menu_vars.Right_Kd, 0, 100, 0.1, [](const float value) {
        /* 回调函数 */
        pid_speed_right.kd = value;
        pid_set_params(&pid_speed_right, pid_speed_right.kp, pid_speed_right.ki, pid_speed_right.kd);
    }),
};

menuItem* pidChildren[] = {
    menuItem::createNormalItem("speed_left", pid_speed_leftChildren, 3),
    menuItem::createNormalItem("speed_right", pid_speed_rightChildren, 3),
};

menuItem* step_motorChildren[] = {
    menuItem::createApp("X_Left_Turn", nullptr, X_Left_Turn),
    menuItem::createApp("X_Right_Turn", nullptr, X_Right_Turn),
    menuItem::createApp("Y_Left_Turn", nullptr, Y_Left_Turn),
    menuItem::createApp("Y_Right_Turn", nullptr, Y_Left_Turn),
    menuItem::createApp("Stop_Turn", nullptr, Stop_Turn),
};


// 使用菜单内部接口的新回调函数
void start_count_show_callback_nav(Menu::Navigator* nav)
{
  char buffer[MAX_DISPLAY_CHAR];
  snprintf(buffer, sizeof(buffer), "addr:0x%lx", 0x1000);
  nav->writeDisplayLine(buffer, 1);
  
  snprintf(buffer, sizeof(buffer), "start_count:%d", start_count);
  nav->writeDisplayLine(buffer, 2);
}

menuItem* flashChildren[] = {
    menuItem::createExhibitionItemWithNav("Start_Count", start_count_show_callback_nav),
    menuItem::createApp("Reset_Count", nullptr, Reset_Count),
};


// 使用菜单内部接口的新回调函数
void gray_show_callback_nav(Menu::Navigator* nav)
{
  char buffer[MAX_DISPLAY_CHAR];
  snprintf(buffer, sizeof(buffer), "%d-%d-%d-%d-%d-%d-%d-%d",(gray_digtal>>0)&0x01,(gray_digtal>>1)&0x01,(gray_digtal>>2)&0x01,(gray_digtal>>3)&0x01,
                                                           (gray_digtal>>4)&0x01,(gray_digtal>>5)&0x01,(gray_digtal>>6)&0x01,(gray_digtal>>7)&0x01);
  nav->writeDisplayLine(buffer, 1);
}

void encoder_show_callback_page(Menu::Navigator* nav, uint8_t current_page, uint8_t total_pages)
{
  char buffer[MAX_DISPLAY_CHAR];
  
  switch(current_page) 
  {
    case 0: // Page 1: System Status
      snprintf(buffer, sizeof(buffer), " left.rpm:%.2f", left_encoder.rpm);
      nav->writeDisplayLine(buffer, 1);
      
      snprintf(buffer, sizeof(buffer), " left:%.2fcm/s", left_encoder.speed_cm_s);
      nav->writeDisplayLine(buffer, 2);
      break;
      
    case 1: // Page 2: Network Info
      snprintf(buffer, sizeof(buffer), "right.rpm:%.2f", right_encoder.rpm);
      nav->writeDisplayLine(buffer, 1);
      
      snprintf(buffer, sizeof(buffer), "right:%.2fcm/s", right_encoder.speed_cm_s);
      nav->writeDisplayLine(buffer, 2);
      break;
    default:
      snprintf(buffer, sizeof(buffer), "Page %d/%d", current_page + 1, total_pages);
      nav->writeDisplayLine(buffer, 1);
      break;
  }
}

// 使用菜单内部接口的新回调函数
void gyroscope_show_callback_nav(Menu::Navigator* nav)
{
  char buffer[MAX_DISPLAY_CHAR];
#if BNO08x_ON == 0
  snprintf(buffer, sizeof(buffer), " Roll:%.2f", icm20608.Roll);
  nav->writeDisplayLine(buffer, 1);
  snprintf(buffer, sizeof(buffer), "Pitch:%.2f", icm20608.Pitch);
  nav->writeDisplayLine(buffer, 2);
  snprintf(buffer, sizeof(buffer), "  Yaw:%.2f", icm20608.Yaw);
  nav->writeDisplayLine(buffer, 3);
#else
  snprintf(buffer, sizeof(buffer), " Roll:%.2f", bno08x.Roll);
  nav->writeDisplayLine(buffer, 1);
  snprintf(buffer, sizeof(buffer), "Pitch:%.2f", bno08x.Pitch);
  nav->writeDisplayLine(buffer, 2);
  snprintf(buffer, sizeof(buffer), "  Yaw:%.2f", bno08x.Yaw);
  nav->writeDisplayLine(buffer, 3);
#endif
}

void AHT20_show_callback(void)
{
  /* 回调函数 */
  Oled_Printf(0, 1, "tempature:%.2f    ", Humiture.Temp);
  Oled_Printf(0, 2, " humidity:%.2f    ", Humiture.RH);
  Oled_Printf(0, 3, "                ");
}

// 使用菜单内部接口的新回调函数
void AHT20_show_callback_nav(Menu::Navigator* nav)
{
  char buffer[MAX_DISPLAY_CHAR];
  snprintf(buffer, sizeof(buffer), "tempature:%.2f", Humiture.Temp);
  nav->writeDisplayLine(buffer, 1);
  
  snprintf(buffer, sizeof(buffer), " humidity:%.2f", Humiture.RH);
  nav->writeDisplayLine(buffer, 2);
  
  nav->writeDisplayLine("", 3);  // Clear line 3
}


void RTC_show_callback_nav(Menu::Navigator* nav)
{
    
  char buffer[MAX_DISPLAY_CHAR];
  snprintf(buffer, sizeof(buffer), "Date:20%02d-%02d-%02d", RTC_Day.Year, RTC_Day.Month, RTC_Day.Date);
  nav->writeDisplayLine(buffer, 1);
  
  snprintf(buffer, sizeof(buffer), "Time:%02d:%02d:%02d", RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);
  nav->writeDisplayLine(buffer, 2);
}

menuItem* main_menuChildren[] = {
    menuItem::createExhibitionItemWithNav("RTC", RTC_show_callback_nav),
    menuItem::createNormalItem("LED", ledChildren, 4),
    menuItem::createExhibitionItemWithNav("Gray", gray_show_callback_nav),
    menuItem::createNormalItem("Motor", motorChildren, 4),
    menuItem::createExhibitionItemWithPage("Encoder", 2, encoder_show_callback_page),
    menuItem::createExhibitionItemWithNav("Gyroscope", gyroscope_show_callback_nav),
    menuItem::createNormalItem("PID", pidChildren, 2),
    menuItem::createNormalItem("Step_Motor", step_motorChildren, 5),
    menuItem::createExhibitionItemWithNav("AHT20", AHT20_show_callback_nav),
    menuItem::createNormalItem("Flash", flashChildren, 2),
};


menuItem* mainMenu = menuItem::createNormalItem("Main Menu", main_menuChildren, 10);

#ifdef __cplusplus
extern "C" {
#endif

void* getMainItem()
{
return (void*)mainMenu;
}

#ifdef __cplusplus
}
#endif


// Example function demonstrating how to get current page name
void debug_info_show_callback(Menu::Navigator* nav)
{
  char buffer[MAX_DISPLAY_CHAR];
  
  // Display current page name (includes item name + page info if in exhibition mode)
  const char* page_name = nav->getCurrentPageName();
  snprintf(buffer, sizeof(buffer), "Page: %s", page_name);
  nav->writeDisplayLine(buffer, 1);
  
  // Display current selected item name
  const char* item_name = nav->getCurrentSelectedItemName();
  snprintf(buffer, sizeof(buffer), "Item: %s", item_name);
  nav->writeDisplayLine(buffer, 2);
  
  // Display exhibition mode status
  bool in_exhibition = nav->isInExhibitionMode();
  snprintf(buffer, sizeof(buffer), "Mode: %s", in_exhibition ? "Exhibition" : "Menu");
  nav->writeDisplayLine(buffer, 3);
}
