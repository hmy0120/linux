/*
 * define struct Thread and PCB.
 */

# define MAX_TASK_NUM 4
# define KERNEL_STACK_SIZE 1024*8

struct Thread{
    unsigned long ip;
    unsigned long sp;
};

typedef struct PCB{
    int pid;
    volatile long state;
    char stack[KERNEL_STACK_SIZE]; // every process has its own stack
    struct Thread thread;
    unsigned long task_entry;
    struct PCB *next;
}tPCB;

void my_schedule(void);

