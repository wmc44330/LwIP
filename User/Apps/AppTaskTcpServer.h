/*
 * @Author: MC Wang @ GDUT
 * @Date: 2023-12-24 21:50:08
 * @LastEditTime: 2024-11-25 18:49:26
 * @Description:
 * @FilePath: \lwIP\User\Apps\AppTaskTcpServer.h
 *
 */
#ifndef __APP_TASK_TCP_SERVER_H__
#define __APP_TASK_TCP_SERVER_H__

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include <os.h>

#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "netif/ethernetif.h"
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/sockets.h"

#define SERVER_PORT 8089
#define TCP_SERVER_RX_BUFSIZE 512

#define APP_CFG_TASK_TCP_SERVER_PRIO 4u

#define APP_CFG_TASK_TCP_SERVER_STK_SIZE 256u

// 启动任务控制块
extern OS_TCB AppTaskTcpServerTCB;
// 启动任务堆栈
extern CPU_STK AppTaskTcpServerStk[APP_CFG_TASK_TCP_SERVER_STK_SIZE];

void AppTaskTcpServer(void *p_arg);

#endif // !__APP_TASK_TCP_SERVER_RECV_H__
