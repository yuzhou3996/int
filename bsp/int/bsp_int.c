#include "bsp_int.h"

/* 中断初始化函数 */
void int_init()
{
    GIC_Init();

    /* 中断向量偏移设置 */
    __set_VBAR(0x87800000);
    
}