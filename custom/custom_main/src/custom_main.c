
#include "custom_main.h"

osThreadId_t OC_APP_TaskHandle;
osEventFlagsId_t cmd_task_flag;

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


// osThreadSuspend();//挂起任务   
// osThreadResume();//恢复任务
// osThreadTerminate();//终止任务


    return osThreadNew(func, NULL, (const osThreadAttr_t *)&thread_cfg);
}

int cm_opencpu_entry(char *param)
{

    // cmd_task_flag = osEventFlagsNew(0);
    // if (cmd_task_flag == 0)
    // {
    //    // cm_log_printf(0, "event flag init failed");
    //     return -1;
    // }
    // open_uart();

    /* 创建进程 */
    osThreadCreat("uart", (void *)open_uart, 0, 800); // 打开串口线程

    osThreadCreat("u1_read", (void *)u1_read, 2, 2000); // 打开串口读取线程

    osThreadCreat("GetCors", (void *)GetCors, 3, 4000); // 打开获取差分数据线程
    osThreadCreat("UpBaidu", (void *)UpBaidu, 3, 4000); // 打开上传位置线程

    return 0;
}
