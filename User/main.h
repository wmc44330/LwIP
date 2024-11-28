#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <os.h>
#include <bsp.h>
#include <os_app_hooks.h>

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "led.h"
#include "usart1.h"
#include "phy.h"
#include "AppTaskStart.h"

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

typedef struct{
#if defined(__LWIPOPTS_H__)
	struct netif netif;
	uint8_t dhcp_sta;
#endif
}device_t;

extern device_t dev;


#endif
