#include "custom_main.h"
#include "uart.h"
#include "cors.h"


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




int cm_opencpu_entry(char *param)
{
    // 打开串口
    uart0_init();
    uart1_init();

    /*创建任务*/
    osThreadCreat("u0_read", (void *)u0_read, 2, 2000); // 打开串口读取线程
    osThreadCreat("u1_read", (void *)u1_read, 3, 2000); // 打开串口读取线程
    osThreadCreat("GetCors", (void*)GetCors, 4, 4000); // 打开获取差分数据线程

    return 0;
}
