#include "cors.h"
#include "cm_modem.h"
#include "uart.h"
#include "cm_pm.h"


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
        if (timeout > 15)
        {
            //重启
            cm_pm_reboot();
            u0_printf("The system will restart!\r\n");
        }

        u0_printf("Waiting for networking  %d\r\n", timeout);
        Delay(500);
    }
    

    CreateCorsSocket();

    // while (1)
    // {
    //     if (strlen((char*)GGAString) > 41)
    //     {
    //         sprintf((char*)GGAString1, "%s%c%c", (char*)GGAString, 0x0D, 0x0A);
    //         if (send(Corssockfd, GGAString1, strlen((char*)GGAString1), 0) < 0)
    //         {
    //             u1_printf("send failed\r\n");
    //             goto input;
    //         }
            
    //         Delay(10);
    //         if (recv(Corssockfd, TCPrxBuf2, sizeof(TCPrxBuf2), 0) < 0)
    //         {
    //             u1_printf("recive failed\r\n");
    //             goto input;
    //         }
    //         cm_uart_write(CM_UART_DEV_0, TCPrxBuf2, sizeof(TCPrxBuf2), 1000);
            
    //         Delay(1000);
    //         count2 = 0;
    //     }
    //     else
    //     {
    //         count2++;
    //         if (count2 >= 15)
    //         {
    //             count2 = 0;
    //             goto input;
    //         }

    //         Delay(200);
    //         // goto input;
    //     }
    // }
}




