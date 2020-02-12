#ifndef _BSP_INT_H
#define _BSP_INT_H
#include "imx6ull.h"

typedef void (*system_irq_handler_t)(unsigned int gicciar, void *param);


/* 函数声明 */
void int_init();
void default_irqhandler(unsigned int gicciar, void *userParam);
void system_irqtable_init();
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);




/* 中断处理函数结构体 */
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler; /* 中断处理函数 */
    void *userParam;                 /* 中断处理函数的参数 */
} sys_irq_handle_t;

#endif