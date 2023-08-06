//任务管理相关函数，对应"task.h"
#include "gdt.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "task.h"
#include "sched.h"
#include "string.h"
#include "printk.h"


pid_t now_pid = 0;			//全局pid值
extern task_t* running_proc_head;		
extern task_t* wait_proc_head;		
extern task_t* current; 	
		
// 内核线程创建
int32_t kernel_thread(int (*fn)(void*), void *arg)
{
	task_t *new_task=(task_t*)kmalloc(STACK_SIZE);
	assert(new_task!=NULL,"kern_therad: kmalloc error!");
	bzero(new_task,sizeof(task_t));
	
	new_task->state=TASK_RUNNABLE;
	new_task->stack=current;
	new_task->pid=now_pid++;
	new_task->mm=NULL;
	
	uint32_t *stack_top=(uint32_t*)((uint32_t)new_task+STACK_SIZE);
	
	*(--stack_top)=(uint32_t)arg;
	*(--stack_top)=(uint32_t)kthread_exit;
	*(--stack_top)=(uint32_t)fn;

	new_task->context.esp=(uint32_t)new_task+STACK_SIZE-sizeof(uint32_t)*3;
	new_task->context.eflags=0x200;
	new_task->next=running_proc_head;
	
	task_t* tail=running_proc_head;
	assert(tail!=NULL,"Must init sched!");
	
	while(tail->next!=running_proc_head) tail=tail->next;
	tail->next=new_task;
	
	return new_task->pid;
}

void kthread_exit()
{
	register uint32_t val  asm("eax");
	printk("Thread exited with value %d\n",val);
	while(1);
}

