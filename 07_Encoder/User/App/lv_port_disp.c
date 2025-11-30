/**
 * @file lv_port_disp.c
 * LVGL Display Port Implementation
 */

#include "lv_port_disp.h"
#include "oled.h"
#include "uart_driver.h"  // 添加串口调试
#include <string.h>
#include <stdio.h>

#define MY_DISP_HOR_RES 128
#define MY_DISP_VER_RES 32

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

static lv_disp_draw_buf_t draw_buf_dsc;
static uint8_t buf_1[MY_DISP_HOR_RES * MY_DISP_VER_RES / 8];  // 单色:128*32/8 = 512字节

void lv_port_disp_init(void)
{
    // 初始化绘制缓冲区,单位为像素(LVGL内部会处理位打包)
    lv_disp_draw_buf_init(&draw_buf_dsc, buf_1, NULL, MY_DISP_HOR_RES * MY_DISP_VER_RES);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf_dsc;
    lv_disp_drv_register(&disp_drv);
}

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    // LVGL单色模式:数据按位打包,每8个像素一个字节
    uint8_t *buf = (uint8_t*)color_p;
    int32_t x, y;
    int32_t buf_width = area->x2 - area->x1 + 1;

    for (uint8_t page = area->y1 / 8; page <= area->y2 / 8; page++) {
        OLED_Set_Position(area->x1, page);

        for (x = area->x1; x <= area->x2; x++) {
            uint8_t byte_data = 0;

            for (uint8_t bit = 0; bit < 8; bit++) {
                y = page * 8 + bit;

                if (y >= area->y1 && y <= area->y2) {
                    int32_t buf_x = x - area->x1;
                    int32_t buf_y = y - area->y1;

                    // LVGL按行存储,横向打包(每行的像素从左到右打包)
                    int32_t pixel_pos = buf_y * buf_width + buf_x;
                    int32_t byte_index = pixel_pos / 8;
                    int32_t bit_index = pixel_pos % 8;

                    if (buf[byte_index] & (1 << bit_index)) {
                        byte_data |= (1 << bit);
                    }
                }
            }

            OLED_Write_data(byte_data);
        }
    }

    lv_disp_flush_ready(disp_drv);
}
