#include "cors.h"
#include "cm_modem.h"
#include "uart.h"
#include "cm_pm.h"
#include "nema.h"
#include <cors_socket.h>
#include "sockets.h"
#include "string.h"
#include "stdio.h"
#include "baidumap.h"

uint8_t CorsTCPRxBuf[1200];

/*获取差分数据任务*/
void GetCors(void)
{
    int timeout = 0;

    u0_printf("Waiting for the connection...\r\n");
    // 等待连接...
    while (1)
    {
        // 获取网络连接状态
        if (cm_modem_get_pdp_state(1) == 1)
        {
            u0_printf("network ok!\r\n");
            break;
        }
        else
        {
            timeout++;
        }

        // 超时
        if (timeout > 10)
        {
            // 重启
            cm_pm_reboot();
            u0_printf("The system will restart!\r\n");
        }

        u0_printf("Waiting for networking  %d\r\n", timeout);
        Delay(500);
    }
    // 创建socket连接
    CreateCorsSocket();

    // 定义要发送的GGA消息
    uint8_t GGAString1[200];
    while (1)
    {
        // 等待GGS数据有效
        if (strlen((char *)GGAString) > 41)
        {
            // 加上回车换行
            sprintf((char *)GGAString1, "%s%c%c", (char *)GGAString, 0x0D, 0x0A);
            // 发送数据
            if (send(Corssockfd, GGAString1, strlen((char *)GGAString1), 0) < 0)
            {
                u1_printf("send failed\r\n");
            }

            Delay(10);
            // 接收数据
            if (recv(Corssockfd, CorsTCPRxBuf, sizeof(CorsTCPRxBuf), 0) < 0)
            {
                u1_printf("recive failed\r\n");
            }
            // 将差分数据发送到RTK内部
            cm_uart_write(CM_UART_DEV_0, CorsTCPRxBuf, sizeof(CorsTCPRxBuf), 1000);

            Delay(1000);
        }
        Delay(10);
    }
}
