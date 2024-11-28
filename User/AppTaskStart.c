/*
 * @Author: MC Wang @ GDUT
 * @Date: 2024-04-18 22:02:40
 * @LastEditTime: 2024-05-02 01:23:28
 * @Description:
 * @FilePath: \AIBE_V1.4\User\AppTaskStart.c
 *
 */

#include "AppTaskStart.h"

#define APPSTART_DEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__)

// 启动任务控制块
OS_TCB AppTaskStartTCB;
// 启动任务堆栈
CPU_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

void AppTaskStart(void *p_arg)
{
	OS_ERR err = OS_ERR_NONE;
	(void)p_arg;
	CPU_SR_ALLOC();

#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err); /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
	CPU_IntDisMeasMaxCurReset();
#endif

/* 使能时间片轮转调度功能 */
#if OS_CFG_SCHED_ROUND_ROBIN_EN
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err); // 设置时间片长度为1个系统时钟节拍，既1*1=1ms
#endif

	OS_CRITICAL_ENTER();
	
	OSTaskCreate((OS_TCB *)&AppTaskTcpServerTCB, /* Create the tcp server recv task                                    */
				 (CPU_CHAR *)"App Task Tcp Server",
				 (OS_TASK_PTR)AppTaskTcpServer,
				 (void *)0u,
				 (OS_PRIO)APP_CFG_TASK_TCP_SERVER_PRIO,
				 (CPU_STK *)&AppTaskTcpServerStk[0u],
				 (CPU_STK_SIZE)APP_CFG_TASK_TCP_SERVER_STK_SIZE / 10u,
				 (CPU_STK_SIZE)APP_CFG_TASK_TCP_SERVER_STK_SIZE,
				 (OS_MSG_QTY)0u,
				 (OS_TICK)0u,
				 (void *)0u,
				 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
				 (OS_ERR *)&err);

	OSTaskCreate((OS_TCB *)&AppTaskLedTCB, /* Create the led task                                    */
				 (CPU_CHAR *)"App Task Led",
				 (OS_TASK_PTR)AppTaskLed,
				 (void *)0u,
				 (OS_PRIO)APP_CFG_TASK_LED_PRIO,
				 (CPU_STK *)&AppTaskLedStk[0u],
				 (CPU_STK_SIZE)APP_CFG_TASK_LED_STK_SIZE / 10u,
				 (CPU_STK_SIZE)APP_CFG_TASK_LED_STK_SIZE,
				 (OS_MSG_QTY)0u,
				 (OS_TICK)0u,
				 (void *)0u,
				 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				 (OS_ERR *)&err);
	OS_CRITICAL_EXIT(); /* 退出临界区 */
	
	while(DEF_TRUE){
//		APPSTART_DEBUG("Delay 500ms\n");
		OSTimeDly(500,OS_OPT_TIME_DLY,&err);
	}
}
