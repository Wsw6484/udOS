//debug相关与内核打印printk()函数，函数实现位于udOS/lib/printk/printk.c

#ifndef INCLUDE_PRINTK_H_
#define INCLUDE_PRINTK_H_

#include "console.h"
#include "vargs.h"

//致命错误
void panic(const char* msg);

void assert(int x,const char* msg);

//内核的打印函数
void printk(const char* format,...);

//内核的打印函数 带颜色
void printk_color(real_color_t back,real_color_t fore,const char *format,...);

#endif 	
