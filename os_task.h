#ifndef OS_TASK_H
#define OS_TASK_H

#define OS_MAX_TASK		100
#define TASK_IDLE		1
#define TASK_READY		2
#define TASK_RUNNING	3


typedef struct TaskControlBlock
{
    unsigned int 	Prio;//��������ȼ�
    unsigned char	TaskStatus;//�����״̬
    unsigned long 	Currenttick;//����ǰtick��
    void*			pTask;//������ָ��
    HANDLE			TaskHandle;
} stTaskControlBlock, pstTaskControlBlock;


void OSTaskCreate(unsigned int Prio, void* pTask);
void DelayHourMinSecMili(int TaskID, unsigned int Hour, unsigned int Minute, unsigned int Sec, unsigned int MiliSec);
void OS_CtxSwitch(void);


#endif // OS_TASK_H
