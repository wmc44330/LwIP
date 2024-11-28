#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define LED_BL PGout(6)

void led_init(void);


#endif
