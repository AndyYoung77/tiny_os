#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "os_task.h"

extern DWORD targetThreadID[OS_MAX_TASK];
extern unsigned int gTaskSwitchTimes;

stTaskControlBlock TCB[OS_MAX_TASK];
unsigned int gTaskNum = 0;
unsigned int gCurrentRunningTaskID = 0;

void OS_CtxSwitch(void)
{
	int i = 0;
	int flag = 0;
	unsigned int gTmpTaskPrio = OS_MAX_TASK;
	unsigned int gTmpId = 0xFF;
	
	/* Ѱ�ҳ���ǰ���ȼ���ߵ����� */
	for (i = 0; i < gTaskNum; i++)
	{
		if (TCB[i].TaskStatus == TASK_READY)
		{
			if (TCB[i].Prio < gTmpTaskPrio)
			{
				gTmpTaskPrio = TCB[i].Prio;
				gTmpId = i;
			}
		}
		else if (TCB[i].TaskStatus == TASK_RUNNING)
		{
			flag = 1;
		}
	}

	if (flag)//ǿ�Ʒ���cpuʹ��Ȩ
	{
		if (gTmpTaskPrio < TCB[gCurrentRunningTaskID].Prio)
		{
			printf("1task switch %d to %d\r\n", gCurrentRunningTaskID, gTmpId);
			gTaskSwitchTimes++;
			SuspendThread(TCB[gCurrentRunningTaskID].TaskHandle);
			TCB[gCurrentRunningTaskID].TaskStatus = TASK_READY;
			
			gCurrentRunningTaskID = gTmpId;
			TCB[gCurrentRunningTaskID].TaskStatus = TASK_RUNNING;
			printf("gCurrentRunningTaskID is %d\r\n", gCurrentRunningTaskID);
			ResumeThread(TCB[gCurrentRunningTaskID].TaskHandle);
		}
	}
	else //�Զ�����delay�����
	{
		printf("2task switch %d to %d\r\n", gCurrentRunningTaskID, gTmpId);
		gTaskSwitchTimes++;
		
		gCurrentRunningTaskID = gTmpId;
		TCB[gCurrentRunningTaskID].TaskStatus = TASK_RUNNING;
		printf("gCurrentRunningTaskID is %d\r\n", gCurrentRunningTaskID);
		ResumeThread(TCB[gCurrentRunningTaskID].TaskHandle);
	}
}

void DelayHourMinSecMili(int TaskID, unsigned int Hour, unsigned int Minute, unsigned int Sec, unsigned int MiliSec)
{
	unsigned int time = (3600 * Hour + 60 * Minute + Sec) * 1000 + MiliSec;
	unsigned int systemtick = time / 10;

	TCB[TaskID].Currenttick = systemtick;
	TCB[TaskID].TaskStatus = TASK_IDLE;
	printf("in delay suspend task %d\r\n", TaskID);
	SuspendThread(TCB[TaskID].TaskHandle);
}
/* ÿ����һ�����������ΪREADY״̬����OS start֮��ʼ������� */
void OSTaskCreate(unsigned int Prio, void* pTask)
{
	TCB[gTaskNum].Prio			= Prio;
	TCB[gTaskNum].pTask			= pTask;
	TCB[gTaskNum].TaskStatus	= TASK_READY;
	TCB[gTaskNum].Currenttick	= 0;
	TCB[gTaskNum].TaskHandle	= CreateThread(NULL, 100, pTask, NULL, CREATE_SUSPENDED, NULL);

	gTaskNum++;
}
