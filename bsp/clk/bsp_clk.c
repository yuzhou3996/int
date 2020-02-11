#include "bsp_clk.h"

void clk_enable(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
}

void imx6u_clkinit(void)
{
	unsigned int reg = 0;

	/* 1、设置ARM内核时钟为528MHz */
	if ((((CCM->CCSR) >> 2) & 0x1) == 0) /* 当前pll1_sw_clk使用的pll1_main_clk*/
	{
		CCM->CCSR &= ~(1 << 8); /* 配置step_clk时钟源为24MHz OSC */
		CCM->CCSR |= (1 << 2);  /* 配置pll1_sw_clk时钟源为step_clk */
	}

	reg = CCM_ANALOG->PLL_ARM;
	reg &= ~(0x7f << 0); 
	reg |= (58 << 0); /* 设置88为528M  设置58为696M */
	reg |= (1 << 13); /* 使能时钟输出 */
	CCM_ANALOG->PLL_ARM = reg;

	// 下边设置是失败的，原因暂未找到
	// CCM_ANALOG->PLL_ARM &= ~(0x7f);
	// CCM_ANALOG->PLL_ARM|= (58 << 0); /* 设置88为528M  设置58为696M */
	// CCM_ANALOG->PLL_ARM|= (1 << 13); /* 使能时钟输出 */



	//  CCM_ANALOG->PLL_ARM = (1<<13)|((58 << 0) & 0x7f); /* 使能时钟输出 */

	CCM->CACRR = 0 << 0;	/* 1为2分频，对应528M  0为1分频，对应696M */
	CCM->CCSR &= ~(1 << 2); /* 将pll_sw_clk时钟重新切换回pll1_main_clk */

	/* 设置PLL2各个PFD */
	reg = 0;
	reg = CCM_ANALOG->PFD_528;
	reg &= ~(0X3F3F3F3F);	  /* 清除原来的设置 						*/
	reg |= 32 << 24;		   /* PLL2_PFD3=528*18/32=297Mhz 	*/
	reg |= 24 << 16;		   /* PLL2_PFD2=528*18/24=396Mhz(DDR使用的时钟，最大400Mhz) */
	reg |= 16 << 8;			   /* PLL2_PFD1=528*18/16=594Mhz 	*/
	reg |= 27 << 0;			   /* PLL2_PFD0=528*18/27=352Mhz  	*/
	CCM_ANALOG->PFD_528 = reg; /* 设置PLL2_PFD0~3 		 		*/

	/* 设置PLL3各个PFD */
	reg = 0; /* 清零   */
	reg = CCM_ANALOG->PFD_480;
	reg &= ~(0X3F3F3F3F);	  /* 清除原来的设置 							*/
	reg |= 19 << 24;		   /* PLL3_PFD3=480*18/19=454.74Mhz */
	reg |= 17 << 16;		   /* PLL3_PFD2=480*18/17=508.24Mhz */
	reg |= 16 << 8;			   /* PLL3_PFD1=480*18/16=540Mhz */
	reg |= 12 << 0;			   /* PLL3_PFD0=480*18/12=720Mhz */
	CCM_ANALOG->PFD_480 = reg; /* 设置PLL3_PFD0~3 */

	/* 设置AHB_CLK_ROOT = 132MHz */
	CCM->CBCMR &= ~(3 << 18);
	CCM->CBCMR |= (1 << 18);
	CCM->CBCDR &= ~(1 << 25);
	while (CCM->CDHIPR & (1 << 5))
		; /* 等待握手信号 */

	/* 3分频默认系统bootrom已经设置好了，不用设置了 */

	/* 设置IPG_CLK_ROOT = 66MHz */
	CCM->CBCDR &= ~(3 << 8);
	CCM->CBCDR |= (1 << 8); /* 设置2分频 */

	/* PER_CLK_ROOT=66MHz */
	CCM->CSCMR1 &= ~(1 << 6);
	CCM->CSCMR1 &= ~(0x3f << 0); /* 1分频 */
}
