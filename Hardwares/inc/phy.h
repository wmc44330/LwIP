#ifndef __PHY_H__
#define __PHY_H__

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_conf.h"
#include "lwipopts.h"

#define PHY_ADDR 0X0
#define LAN8720_RST PGout(8)

int phy_init(void);

#if (PHY == LAN8720A)
static inline uint8_t lan8720_get_speed(void)
{
	return ((ETH_ReadPHYRegister(PHY_ADDR, PHY_SR) & 0x1C) >> 2); // 从LAN8720的31号寄存器中读取网络速度和双工模式
}
#endif

#endif
