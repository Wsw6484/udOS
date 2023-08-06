//线程调度的函数,对应头文件"sched.h"
#include "sched.h"
#include "heap.h"
#include "printk.h"

task_t* running_proc_head=NULL;		//可调度进程链表
task_t* wait_proc_head=NULL;		//等待进程链表
task_t* current=NULL; 			//当前运行的任务
extern pid_t now_pid;

void init_sched()
{
	current = (task_t*)(kern_stack_top-STACK_SIZE);
	current->state=TASK_RUNNABLE;
	current->pid=now_pid++;
	current->stack=current; 	
	current->mm=NULL; 		

	current->next=current;
	running_proc_head=current;
}

void schedule()
{
	if(current) change_task_to(current->next);
}

void change_task_to(task_t *next)
{
	if(current!=next) 
	{
		task_t* prev=current;
		current=next;
		switch_to(&(prev->context), &(current->context));
	}
}

