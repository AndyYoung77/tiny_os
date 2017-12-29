#ifndef OS_TASK_H
#define OS_TASK_H

#define OS_MAX_TASK		100
#define TASK_IDLE		1
#define TASK_READY		2
#define TASK_RUNNING	3


typedef struct TaskControlBlock
{
    unsigned int 	Prio;//任务的优先级
    unsigned char	TaskStatus;//任务的状态
    unsigned long 	Currenttick;//任务当前tick数
    void*			pTask;//任务函数指针
    HANDLE			TaskHandle;
} stTaskControlBlock, pstTaskControlBlock;


void OSTaskCreate(unsigned int Prio, void* pTask);
void DelayHourMinSecMili(int TaskID, unsigned int Hour, unsigned int Minute, unsigned int Sec, unsigned int MiliSec);
void OS_CtxSwitch(void);


#endif // OS_TASK_H
