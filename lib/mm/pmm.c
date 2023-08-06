// 物理内存管理相关函数，对应头文件"pmm.h"

#include "multiboot.h"
#include "common.h"
#include "printk.h"
#include "pmm.h"

// 物理内存页面管理的栈
static uint32_t pmm_stack[PAGE_MAX_SIZE+1];

// 物理内存管理的栈指针
static uint32_t pmm_stack_top=0;

// 物理内存页的数量
uint32_t phy_page_count=0;

void show_memory_map()
{
	uint32_t addr=glb_mboot_ptr->mmap_addr;
	uint32_t len=glb_mboot_ptr->mmap_length;
	
	printk("Memory map::\n");
	
	for(mmap_entry_t *mmap=(mmap_entry_t*)addr;(uint32_t)mmap<addr+len;mmap++)
	{
		printk("base_addr = 0x%X%08X, length=0x%X%08X, type=0x%X\n",
			(uint32_t)mmap->base_addr_high,(uint32_t)mmap->base_addr_low,
			(uint32_t)mmap->length_high,(uint32_t)mmap->length_low,
			(uint32_t)mmap->type);
	}
}

void init_pmm()
{
	mmap_entry_t* start_addr=(mmap_entry_t*)glb_mboot_ptr->mmap_addr;
	mmap_entry_t* end_addr=start_addr+glb_mboot_ptr->mmap_length;
	
	for(mmap_entry_t* entry=start_addr;entry<end_addr;entry++)
	{
		if(entry->type==1&&entry->base_addr_low==0x100000)//type==1表示内存可用		
		{
			uint32_t page_addr=entry->base_addr_low+(uint32_t)(kern_end-kern_start);
			uint32_t len=entry->base_addr_low+entry->length_low;
			
			while(page_addr<len&&page_addr<=PMM_MAX_SIZE)
			{
				pmm_free_page(page_addr);
				page_addr+=PMM_PAGE_SIZE;
				phy_page_count++;
			}
		}
	}
	
}

uint32_t pmm_alloc_page()
{
	assert(pmm_stack_top!=0,"Out of memory!");
	uint32_t page=pmm_stack[pmm_stack_top--];
	return page;
}

void pmm_free_page(uint32_t p)
{
	assert(pmm_stack_top!=PMM_MAX_SIZE,"Out of stack!");
	pmm_stack[++pmm_stack_top]=p;
	
}

