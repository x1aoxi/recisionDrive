/**
 * @file lvgl_app.c
 * LVGL Application Implementation
 */

#include "lvgl_app.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "oled.h"
#include "uart_driver.h"  // 串口调试

void LVGL_Init(void)
{
    // 清空OLED
    OLED_Clear();

    // 初始化LVGL
    lv_init();
    lv_port_disp_init();

    /* 创建测试标签 */
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "LVGL OK!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // 强制刷新显示
    lv_timer_handler();
}

void LVGL_Task(void)
{
    lv_timer_handler();
}
