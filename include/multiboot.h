//multiboot结构体的定义
#ifndef INCLUDE_MULTIBOOT_H_
#define INCLUDE_MULTIBOOT_H_

#include "types.h"


struct multiboot_t {
	uint32_t flags;			//Multiboot的版本信息
	uint32_t mem_lower;
	uint32_t mem_upper;

	uint32_t boot_device;		//指出引导程序从哪个BIOS磁盘设备载入的OS映像
	uint32_t cmdline;		//内核命令行
	uint32_t mods_count;		//boot模块列表
	uint32_t mods_addr;
	
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;

	
	uint32_t mmap_length;		//内存缓冲区限长
	uint32_t mmap_addr;		//内存缓冲区地址
	
	uint32_t drives_length; 	//指出第一个驱动器结构的物理地址	
	uint32_t drives_addr; 		//指出第一个驱动器这个结构的大小
	uint32_t config_table; 		
	uint32_t boot_loader_name;
	uint32_t apm_table; 	    	
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
} __attribute__((packed));
typedef struct multiboot_t multiboot_t;


struct mmap_entry_t {
	uint32_t size; 		
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;			//type为1表示内存可用
} __attribute__((packed));
typedef struct mmap_entry_t mmap_entry_t;


// 内核未建立分页机制前暂存的指针
extern multiboot_t *mboot_ptr_tmp;

// 内核页表建立后的指针
extern multiboot_t *glb_mboot_ptr;

#endif 	
