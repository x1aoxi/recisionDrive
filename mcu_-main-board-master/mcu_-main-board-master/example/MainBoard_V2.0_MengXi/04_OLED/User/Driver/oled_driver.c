#include "oled_driver.h"

/**
 * @brief	使用类似printf的方式显示字符串，显示6x8大小的ASCII字符
 * @param {uint8_t} x待显示字符串的开始横坐标x:0~127
 * @param {uint8_t} y待显示字符串的开始纵坐标 y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {uint8_t} Char_Size待显示字符串的字体大小,选择字体 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @param {const char *} *format 待显示的格式化字符串
 * 例如：Oled_Printf(0, 0, 12, 0, "Data = %d", dat);
**/
int Oled_Printf(uint8_t x, uint8_t y, const char *format, ...)
{
	char buffer[128]; // 临时存储格式化后的字符串
	va_list arg;      // 处理可变参数
	int len;          // 最终字符串长度

	va_start(arg, format);
	// 安全地格式化字符串到 buffer
	len = vsnprintf(buffer, sizeof(buffer), format, arg);
	va_end(arg);

	OLED_ShowStr(x, y, buffer, 8);

	return len;
}
