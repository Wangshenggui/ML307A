#include "custom_main.h"
#include "uart.h"


void Delay(uint32_t ms)
{
    /* ML307A心跳周期为5ms */
    osDelay(ms / 5);
}

osThreadId_t osThreadCreat(const char *name, osThreadFunc_t func, osPriority_t priority, uint32_t stacksize)
{
    osThreadAttr_t thread_cfg =
        {
            /* 进程名 */
            .name = name,
            /* 优先级 */
            .priority = priority,
            /* 栈空间 */
            .stack_size = stacksize,
        };

    /* ML307A模组模组用户线程优先级必须低于或等于osPriorityNormal */
    if (osPriorityNormal > thread_cfg.priority)
    {
        thread_cfg.priority = osPriorityNormal;
    }

    return osThreadNew(func, NULL, (const osThreadAttr_t *)&thread_cfg);
}


void u0_read(char *param)
{
    while (1)
    {
        if (osSemaphoreAcquire(uart0_read_sem,osWaitForever) == osOK)
        {
            char receive[1024] = {0};

            cm_uart_read(CM_UART_DEV_0,receive,1024,5000);
            u0_printf("%s",receive);
            
        }
        Delay(500);
    }
}
void u1_read(char *param)
{
    while (1)
    {
        if (osSemaphoreAcquire(uart1_read_sem,osWaitForever) == osOK)
        {
            char receive[1024] = {0};

            cm_uart_read(CM_UART_DEV_1,receive,1024,5000);
            u1_printf("%s",receive);
            
        }
        Delay(500);
    }
}

int cm_opencpu_entry(char *param)
{
    // 打开串口
    uart0_init();
    uart1_init();

    /*创建任务*/
    osThreadCreat("u0_read", (void *)u0_read, 2, 2000); // 打开串口读取线程
    osThreadCreat("u1_read", (void *)u1_read, 3, 2000); // 打开串口读取线程

    return 0;
}
