//虚拟内存相关函数与定义，对应头文件“vmm.h”

#include "idt.h"
#include "string.h"
#include "printk.h"
#include "vmm.h"
#include "pmm.h"

// 内核页目录区域
pgd_t pgd_kern[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

// 内核页表区域
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

void init_vmm()
{
	register_interrupt_handler(14,&page_fault);
	
	uint32_t kern_pte_first_idx=PGD_INDEX(PAGE_OFFSET);
	uint32_t i=kern_pte_first_idx;
	uint32_t j=0;
	for(;i<PTE_COUNT+kern_pte_first_idx;i++,j++)
	{
		pgd_kern[i]=((uint32_t)pte_kern[j]-PAGE_OFFSET)|PAGE_PRESENT|PAGE_WRITE;
	}
	
	uint32_t* pte=(uint32_t*)pte_kern;
	
	for(int i=1;i<PTE_COUNT*PTE_SIZE;i++)
	{	
		pte[i]=(i<<12)|PAGE_PRESENT|PAGE_WRITE;
	}
	
	uint32_t pgd_kern_phy_addr=(uint32_t)pgd_kern-PAGE_OFFSET;
	switch_pgd(pgd_kern_phy_addr);
	
}

void switch_pgd(uint32_t pd)
{
	asm volatile("mov %0,%%cr3"::"r"(pd));
}

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags)
{ 	
	uint32_t pgd_idx=PGD_INDEX(va);
	uint32_t pte_idx=PTE_INDEX(va);
	
	pte_t* pte=(pte_t*)(pgd_now[pgd_idx]&PAGE_MASK);
	if(!pte)
	{
		pte=(pte_t*)pmm_alloc_page();
		pgd_now[pgd_idx]=(uint32_t)pte|PAGE_PRESENT|PAGE_WRITE;
		
		pte=(pte_t*)((uint32_t)pte+PAGE_OFFSET);
		bzero(pte,PAGE_SIZE);	
	}
	else	pte=(pte_t*)((uint32_t)pte+PAGE_OFFSET);
	
	pte[pte_idx]=(pa&PAGE_MASK)|flags;

	asm volatile("invlpg (%0)"::"a"(va));	

}

void unmap(pgd_t *pgd_now, uint32_t va)
{
	uint32_t pgd_idx=PGD_INDEX(va);
	uint32_t pte_idx=PTE_INDEX(va);
	
	pte_t* pte=(pte_t*)(pgd_now[pgd_idx]&PAGE_MASK);
	if(!pte) return;
	pte=(pte_t*)((uint32_t)pte+PAGE_OFFSET);
	
	pte[pte_idx]=0;

	asm volatile("invlpg (%0)"::"a"(va));	
}

uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa)
{
	uint32_t pgd_idx=PGD_INDEX(va);
	uint32_t pte_idx=PTE_INDEX(va);
	
	pte_t* pte=(pte_t*)(pgd_now[pgd_idx]&PAGE_MASK);
	if(!pte) return 0;
	pte=(pte_t*)((uint32_t)pte+PAGE_OFFSET);

	if(pte[pte_idx]!=0&&pa) 
	{
		*pa=pte[pte_idx]&PAGE_MASK;
		return 1;
	} 
	return 0;
}

void page_fault(pt_regs *regs)
{
	uint32_t cr2;
	asm volatile ("mov %%cr2,%0":"=r"(cr2));

	printk("Page fault at EIP: 0x%x, virtual faulting address 0x%x\n",regs->eip,cr2);
	printk("Error code: %x\n",regs->err_code);

	if(!(regs->err_code&0x1)) 	printk_color(rc_white,rc_red,"Because the page wasn't present.\n");
	
	if(regs->err_code&0x2) 		printk_color(rc_white,rc_red,"Write error.\n");
	else 				printk_color(rc_white,rc_red,"Read error.\n");
		
	if(regs->err_code&0x4)	 	printk_color(rc_white,rc_red,"In user mode.\n");
	else				printk_color(rc_white,rc_red,"In kernel mode.\n");
	
	if(regs->err_code&0x8) 		printk_color(rc_white,rc_red,"Reserved bits being overwritten.\n");

	if(regs->err_code&0x10) 	printk_color(rc_white,rc_red,"The fault occurred during an instruction fetch.\n");
	
	while (1);
}

