//虚拟内存相关函数与定义，函数实现位于udOS/lib/mm/vmm.c
#ifndef INCLUDE_VMM_H
#define INCLUDE_VMM_H

#include "types.h"
#include "idt.h"
#include "vmm.h"

#define PAGE_OFFSET	0xC0000000		//内核偏移
#define PAGE_PRESENT 	0x1			//Present位为1，表明地址转换有效
#define PAGE_WRITE    	0x2			//R/W位为1,表明页面可进行读写
#define PAGE_USER	0x4			//U/S位为1,表明目前是超级用户
#define PAGE_SIZE	4096			//4kB对齐
#define PAGE_MASK	0xFFFFF000		//页掩码

#define PGD_INDEX(x)	(((x)>>22)&0x3FF)	//高10位为页目录偏移
#define PTE_INDEX(x)	(((x)>>12)&0x3FF)	//中间10位为页表偏移
#define OFFSET_INDEX(x)	((x)&0xFFF)		//低12位为页内偏移

#define PGD_SIZE	(PAGE_SIZE/sizeof(uint32_t))	//页目录成员数
#define PTE_SIZE    	(PAGE_SIZE/sizeof(uint32_t))	//页表成员数
#define PTE_COUNT	128				//本项目只支持512MB内存，512MB需要的页表数为128（512M/4K）

typedef uint32_t pgd_t;
typedef uint32_t pte_t;

//初始化虚拟内存
void init_vmm();

//更换当前页目录
void switch_pgd(uint32_t pd);

//将物理内存映射到虚拟内存
void map(pgd_t* pgd_now,uint32_t va,uint32_t pa,uint32_t flags);

//取消映射
void unmap(pgd_t* pgd_now,uint32_t va);

//写入物理地址
uint32_t get_mapping(pgd_t* pgd_now,uint32_t va,uint32_t *pa);

//页错误中断
void page_fault(pt_regs* regs);

#endif
