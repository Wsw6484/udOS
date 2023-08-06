//一些屏幕显示函数,对应头文件“console.h”

#include "console.h"
#include "common.h"
#include "vmm.h"//提供偏移量PAGE_OFFSET==0xC0000000 (3G)

//VGA标准在0xB8000处提供了一个80*25的缓冲区，通过修改这个缓冲区中的值，即可实现显示。一个字符由两个byte表示，第一个
//byte通过ASCII表示字符，第二个byte表示颜色（高4位为背景色，低4位为前景色）


static uint16_t* video_memory=(uint16_t*)(0xB8000+PAGE_OFFSET);
static uint8_t cursor_x=0; 	//光标坐标
static uint8_t cursor_y=0;

//光标移动
static void move_cursor()
{
	uint16_t cursor_location=cursor_y*80+cursor_x;
	
//VGA内部的寄存器多达300多个，显然无法一一映射到I/O端口的地址空间。对此 VGA的解决方案是，将一个端口作为内部寄存器的
//索引：0x3D4，再通过 0x3D5 端口来设置相应寄存器的值。这两个内部寄存器的编号为14与15，分别表示光标位置的高8位与低8位。
	
	outb(0x3D4,14);				// 告诉 VGA 我们要设置光标的高字节
	outb(0x3D5,cursor_location>>8);		// 发送高 8 位
	outb(0x3D4,15);				// 告诉 VGA 我们要设置光标的低字节
	outb(0x3D5,cursor_location);		// 发送低 8 位
}

//屏幕滚动
static void scroll()
{
	if(cursor_y>=25)
	{
		for(int i=0;i<24*80;i++) 	video_memory[i]=video_memory[i+80];
		for(int i=24*80;i<25*80;i++) 	video_memory[i]=0x20;
		cursor_y=24;
	}
}

//清屏函数
void console_clear()
{	
	for(int i=0;i<25*80;i++) 	video_memory[i]=0x20;
	cursor_x=0;
	cursor_y=0;
	move_cursor();
}

//屏幕输出一个彩色字符
void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t back_color=(uint8_t)back;
	uint8_t fore_color=(uint8_t)fore;
	uint8_t color_byte=(back_color<<4)|(fore_color&0x0F);
	uint16_t color=color_byte<<8;
	
	if(c==0x08&&cursor_x) 		cursor_x--;
	else if(c==0x09) 		cursor_x=(cursor_x+8)&~(8-1);
	else if(c=='\r') 		cursor_x=0;
	else if(c=='\n') 		{cursor_x=0;cursor_y++;}
	else if(c>=' ')
	{	
		video_memory[cursor_y*80+cursor_x]=c|color;
		cursor_x++;
	}
	
	if(cursor_x>=80)
	{
		cursor_x=0;
		cursor_y++;
	}
	
	scroll();
	move_cursor();
}

//屏幕打印一个以'\0'结尾的字符串（黑底白字）
void console_write(char* cstr)
{
	while(*cstr)
	{	
		console_putc_color(*cstr,rc_black,rc_white);
		cstr++;
	}
}


//屏幕打印一个以'\0'结尾的彩色字符串
void console_write_color(char* cstr, real_color_t back, real_color_t fore)
{	
	while(*cstr)
	{	
		console_putc_color(*cstr,back,fore);
		cstr++;
	}
}


//输出一个十六进制数（32位）
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore)
{	
	int temp;
	char nozeros=1;
	console_write_color("0x",back,fore);
	
	for(int i=28;i>=0;i-=4)
	{
		temp=(n>>i)&0x0F;
		if(temp==0&&nozeros!=0) continue;
		nozeros=0;
		if(temp>0x0A) console_putc_color(temp-0x0A+'a',back,fore);
		else console_putc_color(temp+'0',back,fore);
	}
}
	
//输出一个十进制数（32位）
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore)
{
	if (n==0) 
	{
		console_putc_color('0',back,fore);
		return;
	}
	
	char c1[32];
	int i=0;
	while(n)
	{
		c1[i]='0'+n%10;
		n/=10;
		i++;
	}
	c1[i]='\0';
	
	char c2[32];
	c2[i]='\0';
	i--;
	int j=0;
	while(i>=0)
	{
		c2[i]=c1[j];
		i--;
		j++;
	}
	
	console_write_color(c2,back,fore);
}
	


	
	
	
	
	
	
	



