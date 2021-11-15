#include "sched.h"
#include "rand.h"
#include "put.h"

long RR_count = time_quantum;

extern void epc_init();
extern void switch_save(struct task_struct*, struct task_struct*);

struct task_struct *current;
struct task_struct *task[NR_TASKS];
long PRIORITY_P[NR_TASKS] = {0,7,6,5,4};

void task_init(void)
{
    puts("task init...\n");
    //init current and task[0]
    current = (struct task_struct*)0x80010000;
    task[0] = current;
    task[0]->state = TASK_RUNNING;
    task[0]->counter = 0;
    task[0]->priority = 5;
    task[0]->blocked = 0;
    task[0]->pid = 0;
    task[0]->thread.sp = 0x80010000 + 0x1000;

    //init task[1-4]
    #ifdef SJF
    for(int i = 1; i <= LAB_TEST_NUM; i++)
    {
        task[i] = (struct task_struct*)(0x80011000 + i * 0x1000);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = rand();
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = 0x80011000 + i * 0x1000;
        task[i]->thread.ra = (unsigned long long)epc_init;
        puts("[PID = ");
        puti(i);
        puts("] Process Create Successfully! counter = ");
        puti(task[i]->counter);
        puts(" priority = ");
        puti(task[i]->priority);
        puts("\n");
    }
    #endif

    #ifdef PRIORITY
    for(int i = 1; i <= LAB_TEST_NUM; i++)
    {
        task[i] = (struct task_struct*)(0x80011000 + i * 0x1000);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 8 - i;
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = 0x80011000 + i * 0x1000;
        task[i]->thread.ra = (unsigned long long)epc_init;
        puts("[PID = ");
        puti(i);
        puts("] Process Create Successfully! counter = ");
        puti(task[i]->counter);
        puts(" priority = ");
        puti(task[i]->priority);
        puts("\n");
    }
    #endif

    #ifdef RR
    RR_count = time_quantum;
    for(int i = 1; i <= LAB_TEST_NUM; i++)
    {
        task[i] = (struct task_struct*)(0x80011000 + i * 0x1000);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = rand();
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = 0x80011000 + i * 0x1000;
        task[i]->thread.ra = (unsigned long long)epc_init;
        puts("[PID = ");
        puti(i);
        puts("] Process Create Successfully! counter = ");
        puti(task[i]->counter);
        puts(" priority = ");
        puti(task[i]->priority);
        puts("\n");
    }
    #endif
}

void do_timer(void)
{
    #ifdef SJF
    //decrease current task counter
    current->counter --;
    puts("[PID = ");
    puti(current->pid);
    puts("] Context Calculation: counter = ");
    puti(current->counter + 1);
    puts("\n");
    if(current->counter <= 0)
    {
        schedule();
    }
    #endif

    #ifdef PRIORITY
    current->counter --;
    if(current->counter <= 0)
    {
        if(current != task[0])
            current->counter = 8 - current->pid;
    }
    schedule();
    #endif

    #ifdef RR
    RR_count --;
    current->counter --;
    puts("[PID = ");
    puti(current->pid);
    puts("] Context Calculation: counter = ");
    puti(current->counter + 1);
    puts(" RR-time-quantum: ");
    puti(RR_count + 1);
    puts("\n");
    if(RR_count <= 0 || current->counter <= 0)
    {
        RR_count = time_quantum;
        schedule();
    }
    #endif
}

void schedule(void)
{
    #ifdef SJF
    int minIndex = LAB_TEST_NUM;
    long minCounter = 100;
    for(int i = LAB_TEST_NUM; i >= 1; i--)
    {
        if(task[i]->counter < minCounter && task[i]->counter != 0)
        {
            minIndex = i;
            minCounter = task[minIndex]->counter;
        }
    }
    //if all process are finished
    if(minCounter == 100)
    {
        for(int i = 1; i <= LAB_TEST_NUM; i++)
        {
            task[i]->counter = rand();
            puts("[PID = ");
            puti(i);
            puts("] Reset counter = ");
            puti(task[i]->counter);
            puts("\n");
        }
        schedule();
        return;
    }
    switch_to(task[minIndex]);
    #endif

    #ifdef PRIORITY
    int minIndex = LAB_TEST_NUM;
    long highestPriority = 100;
    long minCounter = 100;
    for(int i = LAB_TEST_NUM; i >= 1; i--)
    {
        if(task[i]->counter != 0 && task[i]->priority <= highestPriority)
        {
            if(task[i]->priority == highestPriority && task[i]->counter >= minCounter)
    continue;
            minIndex = i;
            minCounter = task[minIndex]->counter;
            highestPriority = task[minIndex]->priority;
        }
    }
    switch_to(task[minIndex]);
    #endif

    #ifdef RR
    int nextIndex = 0;
    int currentIndex = current->pid;
    if(currentIndex == 0)
    {
        for(int i = 1; i <= LAB_TEST_NUM; i++)
        {
            if(task[i]->counter != 0)
            {
                nextIndex = i;
                break;
            }
        }
    }
    else
    {
        for(int i = LAB_TEST_NUM - currentIndex; i > 0; i --)
        {
            if(task[currentIndex + i]->counter != 0)
            {
                nextIndex = currentIndex + i;
            }
        }
        if(nextIndex == 0)
        {
            for(int i = 1 - currentIndex; i <= 0; i ++)
            {
                if(task[currentIndex + i]->counter != 0)
                {
                    nextIndex = currentIndex + i;
                    break;
                }
            }
        }
    }
    if(nextIndex == 0)
    {
        for(int i = 1; i <= LAB_TEST_NUM; i++)
        {
            task[i]->counter = rand();
            puts("[PID = ");
            puti(i);
            puts("] Reset counter = ");
            puti(task[i]->counter);
            puts("\n");
        }
        schedule();
        return;
    }
    switch_to(task[nextIndex]);
    #endif
}

void switch_to(struct task_struct* next)
{
    if(next == current)
    {
        #ifdef PRIORITY
        //update tasks' priorities
        puts("tasks' priority changed\n");
        for(int i = 1; i <= LAB_TEST_NUM; i++)
        {
            task[i]->priority = rand();
            puts("[PID = ");
            puti(i);
            puts("] counter = ");
            puti(task[i]->counter);
            puts(" priority = ");
            puti(task[i]->priority);
            puts("\n");
        }
        #endif
        return;
    }

    puts("[!] Switch from task ");
    puti(current->pid);
    puts(" to task ");
    puti(next->pid);
    puts(", prio: ");
    puti(next->priority);
    puts(", counter: ");
    puti(next->counter);
    puts("\n");
    #ifdef PRIORITY
    //update tasks' priorities
    puts("tasks' priority changed\n");
    for(int i = 1; i <= LAB_TEST_NUM; i++)
    {
        task[i]->priority = rand();
        puts("[PID = ");
        puti(i);
        puts("] counter = ");
        puti(task[i]->counter);
        puts(" priority = ");
        puti(task[i]->priority);
        puts("\n");
    }
    #endif

    struct task_struct* tmp = current;
    current = next;
    switch_save(tmp, next);
}

void dead_loop(void)
{
   // int i = 0;
   // while(i <=1126073120) 
   // {
   //     i++;
   // }
   // puti(current->pid);
   // puts(" loop 1");
   // puts("\n");
   // while(i <= 2222222222)
   // {
   //     i++;
   // }
    int count = 0;
    int i = 0;
    while(1)
    {
        i++;
        if(i%100000000==0)
        {
            i = 0;
            count ++;
//             puti(count);
//             puts("\n");  
        }
    }
}
