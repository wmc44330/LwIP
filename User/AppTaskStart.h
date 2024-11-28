#ifndef __APP_TASK_START_H__
#define __APP_TASK_START_H__

#include <os.h>
#include <app_cfg.h>
#include "led.h"

#include "AppTaskLed.h"
#include "AppTaskTcpServer.h"


// 启动任务控制块
extern OS_TCB AppTaskStartTCB;
// 启动任务堆栈
extern CPU_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

void AppTaskStart(void *p_arg);

#endif // !__APP_TASK_START_H__
