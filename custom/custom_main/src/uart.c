#include "uart.h"

char* delimiter1= "\r\n";

osSemaphoreId_t u0_uart_sem = NULL;
osSemaphoreId_t u1_uart_sem = NULL;
uint8_t corsString[100];

    char inf1[100];
      char inf2[100];
      char inf3[100];
void u0_printf(char *str, ...)
{
    va_list args;
    int len;

    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start(args, str);
    len = vsnprintf((char *)txBuff0, 400, str, args);
    va_end(args);
    cm_uart_write(CM_UART_DEV_0, txBuff0, len, 1000);
}

void u1_printf(char *str, ...)
{
    va_list args1;
    int len;

    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start(args1, str);
    len = vsnprintf((char *)txBuff1, 400, str, args1);
    va_end(args1);
    cm_uart_write(CM_UART_DEV_1, txBuff1, len, 1000);
}

void u0_callback(void *param, uint32_t type)
{

    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {

        /* 接收到新的数据  */
        osSemaphoreRelease(u0_uart_sem);
    }
    else if (CM_UART_EVENT_TYPE_RX_OVERFLOW & type)
    {
        /* 接收FIFO缓存溢出 */
    }
}

void u1_callback(void *param, uint32_t type)
{

    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        /* 接收到新的数据  */
        osSemaphoreRelease(u1_uart_sem);
    }
    else if (CM_UART_EVENT_TYPE_RX_OVERFLOW & type)
    {
        /* 接收FIFO缓存溢出 */
    }
}

void uart_open(cm_uart_dev_e dev, int baudrate, void *callback)
{
    cm_uart_cfg_t config =
        {
            CM_UART_BYTE_SIZE_8,
            CM_UART_PARITY_NONE,
            CM_UART_STOP_BIT_ONE,
            CM_UART_FLOW_CTRL_NONE,
            baudrate};
    cm_uart_event_t event =
        {
            CM_UART_EVENT_TYPE_RX_ARRIVED | CM_UART_EVENT_TYPE_RX_OVERFLOW,
            "NULL",
            callback};

    if (dev == CM_UART_DEV_0)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1);
        event.event_param = "uart0";
    }
    else if (dev == CM_UART_DEV_1)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1);
        event.event_param = "uart1";
    }
    else if (dev == CM_UART_DEV_2)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_50, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_51, CM_IOMUX_FUNC_FUNCTION1);
        event.event_param = "uart2";
    }

    cm_uart_register_event(dev, &event);

    cm_uart_open(dev, &config);

    if (dev == CM_UART_DEV_0)
    {
        /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
        cm_iomux_set_pin_cmd(CM_IOMUX_PIN_17, CM_IOMUX_PINCMD1_LPMEDEG, CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
    }
}

int u0_uart_read(char *data)
{
    int ret = EOF;
    memset(data, 0, strlen(data));
    ret = cm_uart_read(CM_UART_DEV_0, (void *)data, 256, 1000);
    return ret;
}

int u1_uart_read(char *data)
{
    int ret = EOF;
    memset(data, 0, strlen(data));
    ret = cm_uart_read(CM_UART_DEV_1, (void *)data, 256, 1000);
    return ret;
}

void open_uart(void)
{
//cm_mem_get_heap_stats();
    uart_open(CM_UART_DEV_0, 115200, u0_callback);
    uart_open(CM_UART_DEV_1, 115200, u1_callback);
   // cm_uart_write(CM_UART_DEV_0, "System Star!" ,12, 1000);
   // cm_uart_write(CM_UART_DEV_1, "System Star!" ,12, 1000);

    // u1_printf("System ok!\r\n");
    // u0_printf("System ok!\r\n");
    // u0_uart_read(rxBuff0);
    // u1_uart_read(rxBuff1);

 
}

void u1_read(char *param)
{
    // int ret0=u0_uart_read(rxBuff0);
    // int ret1=u1_uart_read(rxBuff1);
   //open_uart();
    while (1)
    {

        memset(rxBuff1, 0, sizeof(rxBuff1));
        u0_uart_read((char*)rxBuff1);
        u1_uart_read((char*)rxBuff2);
        // u0_printf("%s",rxBuff1);

        
        separateString((char*)rxBuff1, (char*)delimiter1, (char*)info1, (char*)info2, (char*)info3);
               // u0_printf("\n%s\n",info1);
        processInfo((uint8_t*)info1, (uint8_t*)info2, (uint8_t*)info1, RMCString, GGAString, GGAString, 102);
       separateString((char*)rxBuff2, (char*)delimiter1, (char*)inf1, (char*)inf2, (char*)inf3);
          // u1_printf("inf1:%s\n", inf1);
       if (inf1[0] == '$' && inf1[1] == 'C' && inf1[2] == 'O' && inf1[3] == 'R' && inf1[4] == 'S')
       {
           swit();
       }
//cm_uart_write(CM_UART_DEV_1, "SystemOK!" ,12, 1000);
        // u1_printf("recive GGAString:  %s",GGAString);

       else if (inf1[0] == '$' && inf1[1] == 'S' && inf1[2] == 'L' && inf1[3] == 'A' && inf1[4] == 'V')
       {
          // u1_printf(">>>>>%s\r\n", inf1);

           for (int i = 1; i < 24; i++)
           {
               ParseSLAVE(inf1, i);
           }

       }
       

        Delay(300);
    }
}

void separateCORS(char* inputString, const char* delimiter, char* inf1, char* inf2, char* inf3) {
    char* token = strtok(inputString, delimiter);

    // 使用 strncpy 将每个信息复制到相应的指针指向的缓冲区
    if (token != NULL) {
        strncpy(info1, token, strlen(token));
        info1[strlen(token)] = '\0';
    }
    else
    {
        info1[0] = '\0';
    }

    token = strtok(NULL, delimiter);

    if (token != NULL) {
        strncpy(info2, token, strlen(token));
        info2[strlen(token)] = '\0';
    }
    else
    {
        info2[0] = '\0';
    }

    token = strtok(NULL, delimiter);

    if (token != NULL) {
        strncpy(info3, token, strlen(token));
        info3[strlen(token)] = '\0';
    }
    else
    {
        info3[0] = '\0';
    }
}









void ParseCORS(const char* string, int n)
{
    char result[200];
    unsigned char max_length = 200;
    const char* start = string;

    int xor_sum = 0;//校验值
    int i = 0;
    unsigned char len = 0;//计算字符串长度
    char checksum_str[3];

    //计算长度
    while (string[len] != '\0')len++;
    // 计算数据的异或和
    for (i = 1; i < len; i++)
    {
        if (string[i] == '*')
            break;
        xor_sum ^= string[i];
    }
    sprintf(checksum_str, "%02X", xor_sum);
    if (1) //通过 
    {



        // 定位到第n个逗号
        for (int i = 0; i < n; i++) {
            start = strchr(start, ',');
            if (!start) {
                // 如果逗号不够n个，将结果设为空字符串
                result[0] = '\0';
                return;
            }

            // 移动到下一个字符
            start++;
        }

        // 计算逗号后字符串的长度
        const char* end = strchr(start, ',');
        size_t length = end ? (size_t)(end - start) : strlen(start);

        // 截取字符串并复制到结果
        if (length < max_length - 1) {
            strncpy(result, start, length);
            // 手动添加 null 结尾
            result[length] = '\0';
        }
        else {
            // 目标长度不足，截断字符串
            strncpy(result, start, max_length - 1);
            result[max_length - 1] = '\0';
        }

        switch (n)
        {
       
        case(1)://域名号
          
            CORS_Struct.Domain = result[0];
         //   u1_printf("CORS_Struct.Domain :%c\n", CORS_Struct.Domain );
            break;
      
        case(2)://端口号
             CORS_Struct.Port = result[0];
            break;
        case(3)://挂载点号
        CORS_Struct.Mount = atoi(result);
          //   CORS_Struct.Mount = result[0];
            break;
      
        case(4)://密文
             sprintf(CORS_Struct.accpass,"%s",result);
            // u0_printf("@%s\n",CORS_Struct.accpass);
            break;

        }
        
    }
}

