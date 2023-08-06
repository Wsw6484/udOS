//线程调度相关函数，函数实现位于udOS/lib/task/sched.h
#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_

#include "task.h"

void init_sched();				//初始化任务调度
void schedule();				//任务调度
void change_task_to(task_t* next);  		//任务切换准备
void switch_to(context_t* prev,context_t* next);//任务切换	

#endif 	
