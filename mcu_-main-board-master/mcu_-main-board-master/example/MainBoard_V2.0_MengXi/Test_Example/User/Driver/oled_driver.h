#ifndef __OLED_DRIVER_H__
#define __OLED_DRIVER_H__

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "oled.h"
#include "menu_wrapper.h"

int Oled_Printf(uint8_t x, uint8_t y, const char *format, ...);

extern uint8_t LOGO[128];

#endif
