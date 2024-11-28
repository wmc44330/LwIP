/*
 * @Author: MC Wang @ GDUT
 * @Date: 2023-11-09 18:21:22
 * @LastEditTime: 2024-05-02 00:44:35
 * @Description:
 * @FilePath: \AIBE_V1.4\User\Apps\Misc\AppTaskLed.c
 *
 */
#include "AppTaskLed.h"

// 启动任务控制块
OS_TCB AppTaskLedTCB;
// 启动任务堆栈
CPU_STK AppTaskLedStk[APP_CFG_TASK_LED_STK_SIZE];

void AppTaskLed(void *p_arg)
{
	OS_ERR err = OS_ERR_NONE;

	while (DEF_TRUE)
	{
		LED_BL = !LED_BL;
		OSTimeDly(500u, OS_OPT_TIME_DLY, &err);
	}
}
