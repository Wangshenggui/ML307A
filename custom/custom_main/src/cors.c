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


uint8_t TCPrxBuf2[1200];


/*获取差分数据任务*/
void GetCors(void)
{
    int timeout = 0;
    
    u0_printf("Waiting for the connection...\r\n");
    //等待连接...
    while(1)
    {
        //获取网络连接状态
        if (cm_modem_get_pdp_state(1) == 1)
        {
            u0_printf("network ok!\r\n");
            break;
        }
        else
        {
            timeout++;
        }

        //超时
        if (timeout > 10)
        {
            //重启
            cm_pm_reboot();
            u0_printf("The system will restart!\r\n");
        }

        u0_printf("Waiting for networking  %d\r\n", timeout);
        Delay(500);
    }
    //创建socket连接
    CreateCorsSocket();

    uint8_t GGAString1[200];
    while (1)
    {
        if (strlen((char*)GGAString) > 41)
        {
            sprintf((char*)GGAString1, "%s%c%c", (char*)GGAString, 0x0D, 0x0A);
            if (send(Corssockfd, GGAString1, strlen((char*)GGAString1), 0) < 0)
            {
                u1_printf("send failed\r\n");
            }
            
            Delay(10);
            if (recv(Corssockfd, TCPrxBuf2, sizeof(TCPrxBuf2), 0) < 0)
            {
                u1_printf("recive failed\r\n");
            }
            cm_uart_write(CM_UART_DEV_0, TCPrxBuf2, sizeof(TCPrxBuf2), 1000);
            
            Delay(1000);
        }
    }

    while(1)
    {
        Delay(500);
    }
}




