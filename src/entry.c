//内核的入口函数
#include "common.h"
#include "console.h"
#include "string.h"
#include "printk.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "task.h"
#include "sched.h"

#define enable_intr 		asm volatile("sti")
#define disable_intr	 	asm volatile("cli":::"memory")

__attribute__((section(".init.text"))) void kern_entry(); //内核入口函数
void kern_init();	//内核初始化函数
void kern();		//内核函数

//开启分页机制前后的 Multiboot数据指针
extern multiboot_t* mboot_ptr_tmp;
extern multiboot_t* glb_mboot_ptr;

//开启分页机制之后的内核栈
char kern_stack[STACK_SIZE]  __attribute__ ((aligned(16)));

//内核栈的栈顶
uint32_t kern_stack_top;

void kern()
{
	console_clear();
	printk_color(rc_white, rc_magenta, "Hello, OS kernel!\n\n");
	printk("kernel in memory start: 0x%08X\n", kern_start);
	printk("kernel in memory end:   0x%08X\n", kern_end);
	printk("kernel in memory used:   %d KB\n\n", (kern_end - kern_start) / 1024);
	printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);
	
	test_heap();
	
	kernel_thread(threadm, NULL);
	kernel_thread(threadm,NULL);
	
	enable_intr;
	while(1) asm volatile ("hlt");
}

//内核使用的临时页表和页目录
__attribute__((section(".init.data"))) pgd_t *pgd_tmp  = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;
__attribute__((section(".init.text"))) void kern_entry()
{
	pgd_tmp[0]=(uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)]=(uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;
	
	for(int i=0;i<1024;i++) 
	{
		pte_low[i]=(i<<12) | PAGE_PRESENT | PAGE_WRITE;
		pte_high[i]=(i<<12) | PAGE_PRESENT | PAGE_WRITE;
	}
	
	asm volatile("mov %0,%%cr3"::"r"(pgd_tmp));
	uint32_t cr0;
	asm volatile("mov %%cr0,%0":"=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0,%%cr0"::"r"(cr0));
	
	kern_stack_top=((uint32_t)kern_stack+STACK_SIZE);
	asm volatile("mov %0,%%esp\n\t" "xor %%ebp,%%ebp"::"r"(kern_stack_top));
	
	glb_mboot_ptr=mboot_ptr_tmp+PAGE_OFFSET;
	
	kern_init();
}

void kern_init()//内核的初始化函数
{
	console_clear();
	init_gdt();
	init_idt();
	init_timer(200);
	init_pmm(); 
	init_vmm();
	init_heap();
	init_sched();
	
	kern();
}	


