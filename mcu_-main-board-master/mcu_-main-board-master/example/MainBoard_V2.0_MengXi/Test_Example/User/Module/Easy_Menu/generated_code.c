#include "menu_navigator.h"
#include "menu_wrapper.h"
#include <stdint.h>
#include <stdio.h>

#include "MyDefine.h"

/* ============================ 生成的变量定义 ============================  */
typedef struct {
    bool led1;
    bool led2;
    bool led3;
    bool led4;
    int32_t l_pwm;
    int32_t r_pwm;
    float left_kp;
    float left_ki;
    float left_kd;
    float right_kp;
    float right_ki;
    float right_kd;
    int32_t x_speed;
    int32_t y_speed;
} menu_variables_t;

static menu_variables_t g_menu_vars = {
    .led1 = false,
    .led2 = false,
    .led3 = false,
    .led4 = false,
    .l_pwm = 0,
    .r_pwm = 0,
    .left_kp = 0.0f,
    .left_ki = 0.0f,
    .left_kd = 0.0f,
    .right_kp = 0.0f,
    .right_ki = 0.0f,
    .right_kd = 0.0f,
    .x_speed = 0,
    .y_speed = 0,
};

/* ============================ 回调函数 ============================  */
void Aht20_Exhibition_Callback(navigator_t* nav, uint8_t current_page, uint8_t total_pages) {
    char buffer[MAX_DISPLAY_CHAR];
    /* TODO: 实现无分页展示回调函数 */
		snprintf(buffer, sizeof(buffer), "    Temp:%.2f", Humiture.Temp);
    navigator_write_display_line(nav, buffer, 1);
    
		snprintf(buffer, sizeof(buffer), "Humidity:%.2f", Humiture.RH);
    navigator_write_display_line(nav, buffer, 2);
}

void Encoder_Exhibition_Callback(navigator_t* nav, uint8_t current_page, uint8_t total_pages) {
    char buffer[MAX_DISPLAY_CHAR];
    /* TODO: 实现分页展示回调函数 */
    switch(current_page)
    {
    case 0:
        /* TODO: 实现第0页的显示内容 */
        snprintf(buffer, sizeof(buffer), "     <Left>");
        navigator_write_display_line(nav, buffer, 1);

				snprintf(buffer, sizeof(buffer), "RPM:%.2f", left_encoder.rpm);
        navigator_write_display_line(nav, buffer, 2);

				snprintf(buffer, sizeof(buffer), "Speed:%.2f", left_encoder.speed_cm_s);
        navigator_write_display_line(nav, buffer, 3);

        break;
    case 1:
        /* TODO: 实现第1页的显示内容 */
        snprintf(buffer, sizeof(buffer), "     <Right>");
        navigator_write_display_line(nav, buffer, 1);

				snprintf(buffer, sizeof(buffer), "RPM:%.2f", right_encoder.rpm);
        navigator_write_display_line(nav, buffer, 2);

				snprintf(buffer, sizeof(buffer), "Speed:%.2f", right_encoder.speed_cm_s);
        navigator_write_display_line(nav, buffer, 3);

        break;
    }
}

void Gray_Exhibition_Callback(navigator_t* nav, uint8_t current_page, uint8_t total_pages) {
    char buffer[MAX_DISPLAY_CHAR];
    /* TODO: 实现无分页展示回调函数 */
    
    snprintf(buffer, sizeof(buffer), "%d-%d-%d-%d-%d-%d-%d-%d\r\n",(gray_digtal>>0)&0x01,(gray_digtal>>1)&0x01,(gray_digtal>>2)&0x01,(gray_digtal>>3)&0x01,
                                                                   (gray_digtal>>4)&0x01,(gray_digtal>>5)&0x01,(gray_digtal>>6)&0x01,(gray_digtal>>7)&0x01);
    navigator_write_display_line(nav, buffer, 2);
    
}

void Gyroscope_Exhibition_Callback(navigator_t* nav, uint8_t current_page, uint8_t total_pages) {
    char buffer[MAX_DISPLAY_CHAR];
    /* TODO: 实现无分页展示回调函数 */
		#if BNO08x_ON == 0
			snprintf(buffer, sizeof(buffer), "Pitch:%.2f", icm20608.Pitch);
			navigator_write_display_line(nav, buffer, 1);
			
			snprintf(buffer, sizeof(buffer), " Roll:%.2f", icm20608.Roll);
			navigator_write_display_line(nav, buffer, 2);
			
			snprintf(buffer, sizeof(buffer), " Yaw:%.2f", icm20608.Yaw);
			navigator_write_display_line(nav, buffer, 3);
		#else
			snprintf(buffer, sizeof(buffer), "Pitch:%.2f", bno08x.Pitch);
			navigator_write_display_line(nav, buffer, 1);
			
			snprintf(buffer, sizeof(buffer), " Roll:%.2f", bno08x.Roll);
			navigator_write_display_line(nav, buffer, 2);
			
			snprintf(buffer, sizeof(buffer), " Yaw:%.2f", bno08x.Yaw);
			navigator_write_display_line(nav, buffer, 3);
		#endif
}

void Led1_Toggle_Callback(bool state) {
    /* TODO: 实现切换回调函数 */
		led_buf[0] = (uint8_t)state;
}

void Led2_Toggle_Callback(bool state) {
    /* TODO: 实现切换回调函数 */
		led_buf[1] = (uint8_t)state;
}

void Led3_Toggle_Callback(bool state) {
    /* TODO: 实现切换回调函数 */
		led_buf[2] = (uint8_t)state;
}

void Led4_Toggle_Callback(bool state) {
    /* TODO: 实现切换回调函数 */
		led_buf[3] = (uint8_t)state;
}

void Left_Kd_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		pid_speed_left.kp = *(float*)value;
}

void Left_Ki_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		pid_speed_left.ki = *(float*)value;
}

void Left_Kp_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		pid_speed_left.kd = *(float*)value;
}

void Motor_Stop_App_Callback(void** args) {
    /* TODO: 实现应用回调函数 */
		Motor_Set_Speed(&left_motor, 0);
		Motor_Set_Speed(&right_motor, 0);
		g_menu_vars.l_pwm = left_motor.speed;
		g_menu_vars.r_pwm = right_motor.speed;
}

void L_Pwm_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		Motor_Set_Speed(&left_motor, *(int32_t*)value);
		g_menu_vars.l_pwm = left_motor.speed;
}

void Reset_App_Callback(void** args) {
    /* TODO: 实现应用回调函数 */
		start_count = 0;
  
		spi_flash_sector_erase(0x1000);
		spi_flash_buffer_write(&start_count, 0x1000, 1);
}

void Right_Kd_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		pid_speed_right.kp = *(float*)value;
}

void Right_Ki_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		pid_speed_right.ki = *(float*)value;
}

void Right_Kp_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		pid_speed_right.kd = *(float*)value;
}

void R_Pwm_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		Motor_Set_Speed(&right_motor, *(int32_t*)value);
		g_menu_vars.r_pwm = right_motor.speed;
}

void Rtc_Exhibition_Callback(navigator_t* nav, uint8_t current_page, uint8_t total_pages) {
    char buffer[MAX_DISPLAY_CHAR];
    /* TODO: 实现无分页展示回调函数 */
		snprintf(buffer, sizeof(buffer), "Date:20%02d-%02d-%02d", RTC_Day.Year, RTC_Day.Month, RTC_Day.Date);
    navigator_write_display_line(nav, buffer, 1);
    
		snprintf(buffer, sizeof(buffer), "Time:%02d:%02d:%02d", RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);
    navigator_write_display_line(nav, buffer, 2);
}

void Start_Count_Exhibition_Callback(navigator_t* nav, uint8_t current_page, uint8_t total_pages) {
    char buffer[MAX_DISPLAY_CHAR];
    /* TODO: 实现无分页展示回调函数 */
		snprintf(buffer, sizeof(buffer), "Address:0x1000");
    navigator_write_display_line(nav, buffer, 1);
    
		snprintf(buffer, sizeof(buffer), "Count:%d", start_count);
    navigator_write_display_line(nav, buffer, 2);
}

void Step_Stop_App_Callback(void** args) {
    /* TODO: 实现应用回调函数 */
		Motor_Vel_Synchronous_Control(0, 0);
		g_menu_vars.x_speed = Get_X_Step_Motor_Speed();
		g_menu_vars.y_speed = Get_Y_Step_Motor_Speed();
}

void X_Speed_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		Motor_Vel_Synchronous_Control(*(int32_t*)value, Get_Y_Step_Motor_Speed());
		g_menu_vars.x_speed = Get_X_Step_Motor_Speed();
}

void Y_Speed_Change_Callback(void* value) {
    /* TODO: 实现实时变化回调函数 */
		Motor_Vel_Synchronous_Control(Get_X_Step_Motor_Speed(), *(int32_t*)value);
		g_menu_vars.y_speed = Get_Y_Step_Motor_Speed();
}


/* ============================ 静态菜单项定义 ============================  */

static menu_item_t menu_main;
static menu_item_t menu_rtc;
static menu_item_t menu_led;
static menu_item_t menu_led1;
static menu_item_t menu_led2;
static menu_item_t menu_led3;
static menu_item_t menu_led4;
static menu_item_t menu_gray;
static menu_item_t menu_gyroscope;
static menu_item_t menu_motor;
static menu_item_t menu_l_pwm;
static menu_item_t menu_r_pwm;
static menu_item_t menu_motor_stop;
static menu_item_t menu_encoder;
static menu_item_t menu_pid;
static menu_item_t menu_left_speed;
static menu_item_t menu_left_kp;
static menu_item_t menu_left_ki;
static menu_item_t menu_left_kd;
static menu_item_t menu_right_speed;
static menu_item_t menu_right_kp;
static menu_item_t menu_right_ki;
static menu_item_t menu_right_kd;
static menu_item_t menu_step_motor;
static menu_item_t menu_x_speed;
static menu_item_t menu_y_speed;
static menu_item_t menu_step_stop;
static menu_item_t menu_aht20;
static menu_item_t menu_flash;
static menu_item_t menu_start_count;
static menu_item_t menu_reset;

static menu_item_t* main_children[] = {&menu_rtc, &menu_led, &menu_gray, &menu_gyroscope, &menu_motor, &menu_encoder, &menu_pid, &menu_step_motor, &menu_aht20, &menu_flash};
static menu_item_t* led_children[] = {&menu_led1, &menu_led2, &menu_led3, &menu_led4};
static menu_item_t* motor_children[] = {&menu_l_pwm, &menu_r_pwm, &menu_motor_stop};
static menu_item_t* pid_children[] = {&menu_left_speed, &menu_right_speed};
static menu_item_t* left_speed_children[] = {&menu_left_kp, &menu_left_ki, &menu_left_kd};
static menu_item_t* right_speed_children[] = {&menu_right_kp, &menu_right_ki, &menu_right_kd};
static menu_item_t* step_motor_children[] = {&menu_x_speed, &menu_y_speed, &menu_step_stop};
static menu_item_t* flash_children[] = {&menu_start_count, &menu_reset};

static int32_t l_pwm_min_val = -999;
static int32_t l_pwm_max_val = 999;
static int32_t l_pwm_step_val = 10;
static int32_t r_pwm_min_val = -999;
static int32_t r_pwm_max_val = 999;
static int32_t r_pwm_step_val = 10;
static float left_kp_min_val = 0.0f;
static float left_kp_max_val = 999.0f;
static float left_kp_step_val = 1.0f;
static float left_ki_min_val = 0.0f;
static float left_ki_max_val = 999.0f;
static float left_ki_step_val = 1.0f;
static float left_kd_min_val = 0.0f;
static float left_kd_max_val = 999.0f;
static float left_kd_step_val = 1.0f;
static float right_kp_min_val = 0.0f;
static float right_kp_max_val = 999.0f;
static float right_kp_step_val = 1.0f;
static float right_ki_min_val = 0.0f;
static float right_ki_max_val = 999.0f;
static float right_ki_step_val = 1.0f;
static float right_kd_min_val = 0.0f;
static float right_kd_max_val = 999.0f;
static float right_kd_step_val = 1.0f;
static int32_t x_speed_min_val = -5000;
static int32_t x_speed_max_val = 5000;
static int32_t x_speed_step_val = 10;
static int32_t y_speed_min_val = -5000;
static int32_t y_speed_max_val = 5000;
static int32_t y_speed_step_val = 10;

static menu_item_t menu_main = {
    .is_locked = true,
    .item_name = "Main",
    .parent_item = &menu_main,
    .children_items = main_children,
    .children_count = 10,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_rtc = {
    .is_locked = true,
    .item_name = "RTC",
    .parent_item = &menu_main,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_EXHIBITION,
    .periodic_callback_with_page = Rtc_Exhibition_Callback,
    .data.exhibition = {
        .current_page = 0,
        .total_pages = 1,
        .lines_per_page = MAX_DISPLAY_ITEM - 1
    }
};

static menu_item_t menu_led = {
    .is_locked = true,
    .item_name = "LED",
    .parent_item = &menu_main,
    .children_items = led_children,
    .children_count = 4,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_led1 = {
    .is_locked = true,
    .item_name = "LED1",
    .parent_item = &menu_led,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_TOGGLE,
    .data.toggle = {
        .state = false,
        .ref = &g_menu_vars.led1,
        .on_toggle = Led1_Toggle_Callback
    }
};

static menu_item_t menu_led2 = {
    .is_locked = true,
    .item_name = "LED2",
    .parent_item = &menu_led,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_TOGGLE,
    .data.toggle = {
        .state = false,
        .ref = &g_menu_vars.led2,
        .on_toggle = Led2_Toggle_Callback
    }
};

static menu_item_t menu_led3 = {
    .is_locked = true,
    .item_name = "LED3",
    .parent_item = &menu_led,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_TOGGLE,
    .data.toggle = {
        .state = false,
        .ref = &g_menu_vars.led3,
        .on_toggle = Led3_Toggle_Callback
    }
};

static menu_item_t menu_led4 = {
    .is_locked = true,
    .item_name = "LED4",
    .parent_item = &menu_led,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_TOGGLE,
    .data.toggle = {
        .state = false,
        .ref = &g_menu_vars.led4,
        .on_toggle = Led4_Toggle_Callback
    }
};

static menu_item_t menu_gray = {
    .is_locked = true,
    .item_name = "Gray",
    .parent_item = &menu_main,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_EXHIBITION,
    .periodic_callback_with_page = Gray_Exhibition_Callback,
    .data.exhibition = {
        .current_page = 0,
        .total_pages = 1,
        .lines_per_page = MAX_DISPLAY_ITEM - 1
    }
};

static menu_item_t menu_gyroscope = {
    .is_locked = true,
    .item_name = "Gyroscope",
    .parent_item = &menu_main,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_EXHIBITION,
    .periodic_callback_with_page = Gyroscope_Exhibition_Callback,
    .data.exhibition = {
        .current_page = 0,
        .total_pages = 1,
        .lines_per_page = MAX_DISPLAY_ITEM - 1
    }
};

static menu_item_t menu_motor = {
    .is_locked = true,
    .item_name = "Motor",
    .parent_item = &menu_main,
    .children_items = motor_children,
    .children_count = 3,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_l_pwm = {
    .is_locked = true,
    .item_name = "L_PWM",
    .parent_item = &menu_motor,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.l_pwm,
        .min_val = &l_pwm_min_val,
        .max_val = &l_pwm_max_val,
        .step_val = &l_pwm_step_val,
        .data_type = DATA_TYPE_INT32,
        .on_change = L_Pwm_Change_Callback
    }
};

static menu_item_t menu_r_pwm = {
    .is_locked = true,
    .item_name = "R_PWM",
    .parent_item = &menu_motor,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.r_pwm,
        .min_val = &r_pwm_min_val,
        .max_val = &r_pwm_max_val,
        .step_val = &r_pwm_step_val,
        .data_type = DATA_TYPE_INT32,
        .on_change = R_Pwm_Change_Callback
    }
};

static menu_item_t menu_motor_stop = {
    .is_locked = true,
    .item_name = "Motor_Stop",
    .parent_item = &menu_motor,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_NORMAL,
    .app_func = Motor_Stop_App_Callback,
    .app_args = NULL
};

static menu_item_t menu_encoder = {
    .is_locked = true,
    .item_name = "Encoder",
    .parent_item = &menu_main,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_EXHIBITION,
    .periodic_callback_with_page = Encoder_Exhibition_Callback,
    .data.exhibition = {
        .current_page = 0,
        .total_pages = 2,
        .lines_per_page = MAX_DISPLAY_ITEM - 1
    }
};

static menu_item_t menu_pid = {
    .is_locked = true,
    .item_name = "PID",
    .parent_item = &menu_main,
    .children_items = pid_children,
    .children_count = 2,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_left_speed = {
    .is_locked = true,
    .item_name = "Left_Speed",
    .parent_item = &menu_pid,
    .children_items = left_speed_children,
    .children_count = 3,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_left_kp = {
    .is_locked = true,
    .item_name = "Kp",
    .parent_item = &menu_left_speed,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.left_kp,
        .min_val = &left_kp_min_val,
        .max_val = &left_kp_max_val,
        .step_val = &left_kp_step_val,
        .data_type = DATA_TYPE_FLOAT,
        .on_change = Left_Kp_Change_Callback
    }
};

static menu_item_t menu_left_ki = {
    .is_locked = true,
    .item_name = "Ki",
    .parent_item = &menu_left_speed,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.left_ki,
        .min_val = &left_ki_min_val,
        .max_val = &left_ki_max_val,
        .step_val = &left_ki_step_val,
        .data_type = DATA_TYPE_FLOAT,
        .on_change = Left_Ki_Change_Callback
    }
};

static menu_item_t menu_left_kd = {
    .is_locked = true,
    .item_name = "Kd",
    .parent_item = &menu_left_speed,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.left_kd,
        .min_val = &left_kd_min_val,
        .max_val = &left_kd_max_val,
        .step_val = &left_kd_step_val,
        .data_type = DATA_TYPE_FLOAT,
        .on_change = Left_Kd_Change_Callback
    }
};

static menu_item_t menu_right_speed = {
    .is_locked = true,
    .item_name = "Right_Speed",
    .parent_item = &menu_pid,
    .children_items = right_speed_children,
    .children_count = 3,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_right_kp = {
    .is_locked = true,
    .item_name = "Kp",
    .parent_item = &menu_right_speed,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.right_kp,
        .min_val = &right_kp_min_val,
        .max_val = &right_kp_max_val,
        .step_val = &right_kp_step_val,
        .data_type = DATA_TYPE_FLOAT,
        .on_change = Right_Kp_Change_Callback
    }
};

static menu_item_t menu_right_ki = {
    .is_locked = true,
    .item_name = "Ki",
    .parent_item = &menu_right_speed,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.right_ki,
        .min_val = &right_ki_min_val,
        .max_val = &right_ki_max_val,
        .step_val = &right_ki_step_val,
        .data_type = DATA_TYPE_FLOAT,
        .on_change = Right_Ki_Change_Callback
    }
};

static menu_item_t menu_right_kd = {
    .is_locked = true,
    .item_name = "Kd",
    .parent_item = &menu_right_speed,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.right_kd,
        .min_val = &right_kd_min_val,
        .max_val = &right_kd_max_val,
        .step_val = &right_kd_step_val,
        .data_type = DATA_TYPE_FLOAT,
        .on_change = Right_Kd_Change_Callback
    }
};

static menu_item_t menu_step_motor = {
    .is_locked = true,
    .item_name = "Step_Motor",
    .parent_item = &menu_main,
    .children_items = step_motor_children,
    .children_count = 3,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_x_speed = {
    .is_locked = true,
    .item_name = "X_Speed",
    .parent_item = &menu_step_motor,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.x_speed,
        .min_val = &x_speed_min_val,
        .max_val = &x_speed_max_val,
        .step_val = &x_speed_step_val,
        .data_type = DATA_TYPE_INT32,
        .on_change = X_Speed_Change_Callback
    }
};

static menu_item_t menu_y_speed = {
    .is_locked = true,
    .item_name = "Y_Speed",
    .parent_item = &menu_step_motor,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_CHANGEABLE,
    .data.changeable = {
        .ref = &g_menu_vars.y_speed,
        .min_val = &y_speed_min_val,
        .max_val = &y_speed_max_val,
        .step_val = &y_speed_step_val,
        .data_type = DATA_TYPE_INT32,
        .on_change = Y_Speed_Change_Callback
    }
};

static menu_item_t menu_step_stop = {
    .is_locked = true,
    .item_name = "Step_Stop",
    .parent_item = &menu_step_motor,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_NORMAL,
    .app_func = Step_Stop_App_Callback,
    .app_args = NULL
};

static menu_item_t menu_aht20 = {
    .is_locked = true,
    .item_name = "AHT20",
    .parent_item = &menu_main,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_EXHIBITION,
    .periodic_callback_with_page = Aht20_Exhibition_Callback,
    .data.exhibition = {
        .current_page = 0,
        .total_pages = 1,
        .lines_per_page = MAX_DISPLAY_ITEM - 1
    }
};

static menu_item_t menu_flash = {
    .is_locked = true,
    .item_name = "Flash",
    .parent_item = &menu_main,
    .children_items = flash_children,
    .children_count = 2,
    .type = MENU_TYPE_NORMAL
};

static menu_item_t menu_start_count = {
    .is_locked = true,
    .item_name = "Start_Count",
    .parent_item = &menu_flash,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_EXHIBITION,
    .periodic_callback_with_page = Start_Count_Exhibition_Callback,
    .data.exhibition = {
        .current_page = 0,
        .total_pages = 1,
        .lines_per_page = MAX_DISPLAY_ITEM - 1
    }
};

static menu_item_t menu_reset = {
    .is_locked = true,
    .item_name = "Reset",
    .parent_item = &menu_flash,
    .children_items = NULL,
    .children_count = 0,
    .type = MENU_TYPE_NORMAL,
    .app_func = Reset_App_Callback,
    .app_args = NULL
};


/* ============================ 兼容性函数 ============================  */
static menu_item_t* Create_Main_Menu(void) {
    return &menu_main;
}

/* ============================ 获取主菜单项 ============================  */
void* getMainItem(void) {
    return Create_Main_Menu();
}

