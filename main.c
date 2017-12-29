#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "os_task.h"

DWORD dwStart, dwStop ;
DWORD targetThreadID[OS_MAX_TASK];
int tmpInt[100];
int privatedata[100];
HANDLE threadhandle[100];
HANDLE OSCtxSwW32Event;
HANDLE SystemTickTaskHandle;
unsigned int gTaskSwitchTimes = 0;

extern stTaskControlBlock TCB[OS_MAX_TASK];
extern unsigned int gTaskNum;
extern unsigned int gCurrentRunningTaskID;

void SystemTickTaskHook(void)
{
	unsigned int i = 0;
	//printf("enter hook\r\n");
	/* 给每个任务tick数都减一 */
	for (i = 0; i < gTaskNum; i++)
	{
		if (TCB[i].TaskStatus == TASK_IDLE)
		{
			TCB[i].Currenttick--;
			
			if (TCB[i].Currenttick == 0)
			{
				TCB[i].TaskStatus = TASK_READY;
			}
		}
	}
	OS_CtxSwitch();
}
/* 10ms os tick */
DWORD WINAPI SystemTickTask(LPVOID threadNo)
{
    while(1)
    {
		dwStart = dwStop;
		do
		{
			dwStop = GetTickCount();
		}while(dwStop - 10 < dwStart);
		SystemTickTaskHook();
    }
}
int SystemTickTaskID = 1000;
void OSIdleTask(void)
{
	while(1)
	{
		printf("Idle task total switch times:%d\r\n", gTaskSwitchTimes);
		Sleep(1000);
	}
}

void OSTaskInit(void)
{
	//创建系统时钟任务，不参与任务调度
	SystemTickTaskHandle = CreateThread(NULL, 100, SystemTickTask, &SystemTickTaskID, CREATE_SUSPENDED, &targetThreadID[0]);
	gTaskNum = 0;
	TCB[gTaskNum].Currenttick	= 0;
	TCB[gTaskNum].Prio			= OS_MAX_TASK - 1;
	TCB[gTaskNum].pTask 		= OSIdleTask;
	TCB[gTaskNum].TaskStatus	= TASK_RUNNING;
	//创建系统空闲任务，参与任务调度
	TCB[gTaskNum].TaskHandle	= CreateThread(NULL, 100, TCB[gTaskNum].pTask, NULL, 0, NULL);
	gCurrentRunningTaskID = gTaskNum;

	gTaskNum = 1;
}

void OSTaskStart(void)
{
	ResumeThread(SystemTickTaskHandle);
}

void Task1(void)
{
	while(1)
	{
		printf("in in in in task1\r\n");
		DelayHourMinSecMili(1,0,0,5,0);
	}
}

void Task2(void)
{
	while(1)
	{
		printf("&&&&& task2\r\n");
		DelayHourMinSecMili(2,0,0,4,10);
	}
}

void Task3(void)
{
	while(1)
	{
		printf("****** task3\r\n");
		DelayHourMinSecMili(3,0,0,3,20);
	}
}


int main()
{
    printf("Hello world!\n");
    
	OSTaskInit();
	OSTaskCreate(1, Task1);
	OSTaskCreate(2, Task2);
	OSTaskCreate(3, Task3);
	OSTaskStart();
	Sleep(14030);
    return 0;
}
