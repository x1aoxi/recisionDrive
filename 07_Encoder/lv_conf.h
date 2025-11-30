/**
 * @file lv_conf.h
 * LVGL v8.3.11 Configuration for 128x32 OLED
 */

#if 1 /*启用配置*/

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   颜色设置
 *====================*/
#define LV_COLOR_DEPTH 1  /* 单色OLED */
#define LV_COLOR_SCREEN_TRANSP 0
#define LV_COLOR_16_SWAP 0

/* 单色显示设置 */
#define LV_COLOR_1_CHROMA_KEY LV_COLOR_LIME  /* 透明色 */

/*====================
   内存设置
 *====================*/
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
    #define LV_MEM_SIZE (16U * 1024U)  /* 16KB */
    #define LV_MEM_ADR 0
#endif

/*====================
   HAL设置
 *====================*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE "main.h"
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (HAL_GetTick())
#endif

/*====================
   功能配置
 *====================*/
#define LV_FONT_MONTSERRAT_12 1  /* 启用12号字体 */
#define LV_USE_LOG 0             /* 禁用日志 */

/* 启用基础组件 */
#define LV_USE_BTN 1
#define LV_USE_LABEL 1
#define LV_USE_IMG 0
#define LV_USE_LINE 1
#define LV_USE_ARC 0
#define LV_USE_BAR 1
#define LV_USE_SLIDER 0
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_ROLLER 0
#define LV_USE_TEXTAREA 0
#define LV_USE_SWITCH 0

#endif /*LV_CONF_H*/
#endif /*Content enable*/
