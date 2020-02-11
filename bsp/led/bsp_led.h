#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#define LED0	0

/* 函数声明 */
void led_init(void);
void led_switch(int led, int status);
#endif

