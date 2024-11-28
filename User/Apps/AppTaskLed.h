#ifndef __APP_TASK_LED_H__
#define __APP_TASK_LED_H__

#include <os.h>
#include <string.h>
#include <stdio.h>

#include "led.h"

#define APP_CFG_TASK_LED_PRIO (OS_CFG_PRIO_MAX - 3u)

#define APP_CFG_TASK_LED_STK_SIZE 64u

// 启动任务控制块
extern OS_TCB AppTaskLedTCB;
// 启动任务堆栈
extern CPU_STK AppTaskLedStk[APP_CFG_TASK_LED_STK_SIZE];

void AppTaskLed(void *p_arg);

#endif // !__APP_TASK_LED_H__
