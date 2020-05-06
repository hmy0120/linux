/*
 *
 *  my_start_kernel here
 *
 */

#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/tty.h>
#include <linux/vmalloc.h>

#include "mypcb.h"

tPCB task[MAX_TASK_NUM];
tPCB *my_current_task = NULL;
volatile int my_need_sched = 0;

void my_process(void);

// when kernel initialize, linux's start_kernel() function call my_start_kernel()
// this is the entry of all
void __init my_start_kernel(void){
    int pid = 0;
    int i;
    // Initialize process 0
    task[pid].pid = pid;
    task[pid].state =  0;
    task[pid].task_entry = task[pid].thread.ip = (unsigned long)my_process;
    task[pid].thread.sp = (unsigned long)(&task[pid].stack[KERNEL_STACK_SIZE-1]);
    task[pid].next = &task[pid];
    // Fork more process
    for(i=1;i<MAX_TASK_NUM;i++){
        memcpy(&task[i],&task[0],sizeof(tPCB));
        task[i].pid = i;
        task[i].thread.sp = (unsigned long)(&task[i].stack[KERNEL_STACK_SIZE-1]);
        // tricky : form a loop
        task[i].next = task[i-1].next;
        task[i-1].next = &task[i];
    }
    // start process 0 by task[0]
    pid = 0;
    my_current_task = &task[pid];
    asm volatile(
        "movq %1,%%rsp\n\t"
        "pushq %1\n\t"
        "pushq %0\n\t"
        "ret\n\t"
        :
        : "c" (task[pid].thread.ip), "d" (task[pid].thread.sp)
    );
}


int i = 0;

void my_process(void){
    while (1)
    {
        i++;
        if (i%10000000 == 0)
        {
            printk(KERN_NOTICE "this is process %d -\n",my_current_task->pid);
            if(my_need_sched == 1){
                my_need_sched = 0;
                my_schedule();
            }
            printk(KERN_NOTICE "this is process %d +\n",my_current_task->pid);
        }
    }
}

