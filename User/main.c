#include "main.h"

#define DEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__)

device_t dev;

int main(void){
	OS_ERR err = OS_ERR_NONE;
	struct ip_addr ipaddr = {0}, netmask = {0}, gw = {0}; // ip地址,子网掩码,默认网关
	
	CPU_SR_ALLOC();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	led_init();
	usart1_init(115200);
	phy_init();
	
	printf("Hello , World! \r\n");
	
	BSP_Init();
	BSP_Tick_Init();
	CPU_Init();
	OSInit(&err);
	App_OS_SetAllHooks();
	
	tcpip_init(NULL, NULL);

#if !LWIP_DHCP // 使用静态IP
	// 设置静态IP
	IP4_ADDR(&ipaddr, 10, 0, 0, 5);
	IP4_ADDR(&netmask, 255, 0, 0, 0);
	IP4_ADDR(&gw, 10, 0, 0, 1);
	netif_add(&dev.netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
	DEBUG("\r\nStatic IP <<-------------------------- %u.%u.%u.%u\r\n", (uint8_t)(dev.netif.ip_addr.addr & 0XFF), (uint8_t)((dev.netif.ip_addr.addr >> 8) & 0XFF), (uint8_t)((dev.netif.ip_addr.addr >> 16) & 0XFF), (uint8_t)((dev.netif.ip_addr.addr >> 24) & 0XFF));
	DEBUG("Net Mask  <<-------------------------- %u.%u.%u.%u\r\n", (uint8_t)(dev.netif.netmask.addr & 0XFF), (uint8_t)((dev.netif.netmask.addr >> 8) & 0XFF), (uint8_t)((dev.netif.netmask.addr >> 16) & 0XFF), (uint8_t)((dev.netif.netmask.addr >> 24) & 0XFF));
	DEBUG("Default Gateway       <<-------------- %u.%u.%u.%u\r\n", (uint8_t)(dev.netif.gw.addr & 0XFF), (uint8_t)((dev.netif.gw.addr >> 8) & 0XFF), (uint8_t)((dev.netif.gw.addr >> 16) & 0XFF), (uint8_t)((dev.netif.gw.addr >> 24) & 0XFF));
	DEBUG("Network Card [%c%c] MAC <<-------------- %02X:%02X:%02X:%02X:%02X:%02X\r\n", dev.netif.name[0], dev.netif.name[1], dev.netif.hwaddr[0], dev.netif.hwaddr[1], dev.netif.hwaddr[2], dev.netif.hwaddr[3], dev.netif.hwaddr[4], dev.netif.hwaddr[5]);
#else
	netif_add(&aibe.netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
#endif
	netif_set_default(&dev.netif); // 设置netif为默认网口
	netif_set_up(&dev.netif);		// 打开netif网口
	
	CPU_CRITICAL_ENTER();
	OSTaskCreate((OS_TCB *)&AppTaskStartTCB,
				 (CPU_CHAR *)"App Task Start",
				 (OS_TASK_PTR)AppTaskStart,
				 (void *)0,
				 (OS_PRIO)APP_CFG_TASK_START_PRIO,
				 (CPU_STK *)&AppTaskStartStk[0],
				 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE / 10,
				 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,
				 (OS_MSG_QTY)0,
				 (OS_TICK)0,
				 (void *)0,
				 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
				 (OS_ERR *)&err);
	CPU_CRITICAL_EXIT();

	OSStart(&err);
}
