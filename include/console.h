//一些屏幕显示函数，函数实现位于 udOS/drivers/console.c

#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include"types.h"

enum real_color
{
	rc_black =		0,
	rc_blue =		1,
	rc_green =		2,
	rc_cyan =		3,
	rc_red =		4,
	rc_magenta =		5,
	rc_brown =		6,
	rc_lighr_grey =		7,
	rc_dark_grey =		8,
	rc_light_blue =		9,
	rc_light_green =	10,
	rc_light_cyan =		11,
	rc_light_red =		12,
	rc_light_magenta =	13,
	rc_light_brown =	14,
	rc_white =		15
};
typedef enum real_color real_color_t; 
	
//清屏函数
void console_clear();

//屏幕输出一个彩色字符
void console_putc_color(char c, real_color_t back, real_color_t fore);

//屏幕打印一个以'\0'结尾的字符串（黑底白字）
void console_write(char* cstr);

//屏幕打印一个以'\0'结尾的彩色字符串
void console_write_color(char* cstr, real_color_t back, real_color_t fore);

//输出一个十六进制数（32位）
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore);

//输出一个十进制数（32位）
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore);

#endif
