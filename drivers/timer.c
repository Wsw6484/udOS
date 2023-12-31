//定时器中断函数，对应头文件timer.h
#include "timer.h"
#include "printk.h"
#include "common.h"
#include "idt.h"
#include "sched.h"

void timer_callback(pt_regs *regs)
{
	schedule();
}

void init_timer(uint32_t frequency)
{
	
	register_interrupt_handler(IRQ0, timer_callback);

	// Intel 8253/8254 PIT芯片 I/O端口地址范围是40h~43h
	// 输入频率为 1193180，frequency 即每秒中断次数
	uint32_t divisor = 1193180 / frequency;

	// D7 D6 D5 D4 D3 D2 D1 D0
	// 0  0  1  1  0  1  1  0
	// 即就是 36 H
	// 设置 8253/8254 芯片工作在模式 3 下
	outb(0x43, 0x36);

	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t hign = (uint8_t)((divisor >> 8) & 0xFF);
	
	outb(0x40, low);
	outb(0x40, hign);
}
