#include "mcu_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include "nema.h"
#include "string.h"
#include <cors_socket.h>
#include "uart.h"

uint8_t CORSString[100];

CORS_Structure CORS_Struct;

void ParsingCORS(const char *string)
{
    ParseCORS((char *)string, 1);
    if (CORS_Struct.Domain == '0')
    {
        cm_pm_reboot();
    }
    if (CORS_Struct.Domain == '1')
    {
        sprintf(CORS_Struct.Corsipstr, "%s", "sdk.pnt.10086.cn");
    }
    if (CORS_Struct.Domain == '2')
    {
        sprintf(CORS_Struct.Corsipstr, "%s", "rtk.ntrip.qxwz.com");
    }
    ParseCORS((char *)string, 2);
    if (CORS_Struct.Port == '3')
    {
        CORS_Struct.CORSport = 8001;
    }
    if (CORS_Struct.Port == '4')
    {
        CORS_Struct.CORSport = 8002;
    }
    ParseCORS((char *)string, 3);
    if (CORS_Struct.Mount == 5)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM33_GRCEJ");
    }
    if (CORS_Struct.Mount == 6)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM33_GRCE");
    }
    if (CORS_Struct.Mount == 7)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM33_GRC");
    }
    if (CORS_Struct.Mount == 8)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM30_GR");
    }
    if (CORS_Struct.Mount == 9)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM33_GRCEpro");
    }
    if (CORS_Struct.Mount == 10)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM32_GGB");
    }
    if (CORS_Struct.Mount == 11)
    {
        sprintf(CORS_Struct.CORSMount, "%s", "RTCM30_GG");
    }

    ParseCORS((char *)string, 4);

    u1_printf("%s\r\n",CORS_Struct.Corsipstr);
    u1_printf("%d\r\n",CORS_Struct.CORSport);
    u1_printf("%s\r\n",CORS_Struct.CORSMount);
    u1_printf("%s\r\n",CORS_Struct.AccPassCiphertext);
}

void ParseCORS(const char *string, int n)
{
    char result[200];
    unsigned char max_length = 200;
    const char *start = string;

    int xor_sum = 0; // 校验值
    int i = 0;
    unsigned char len = 0; // 计算字符串长度
    char checksum_str[3];

    // 计算长度
    while (string[len] != '\0')
        len++;
    // 计算数据的异或和
    for (i = 1; i < len; i++)
    {
        if (string[i] == '*')
            break;
        xor_sum ^= string[i];
    }
    sprintf(checksum_str, "%02X", xor_sum);
    if (1) // 通过
    {
        // 定位到第n个逗号
        for (int i = 0; i < n; i++)
        {
            start = strchr(start, ',');
            if (!start)
            {
                // 如果逗号不够n个，将结果设为空字符串
                result[0] = '\0';
                return;
            }

            // 移动到下一个字符
            start++;
        }

        // 计算逗号后字符串的长度
        const char *end = strchr(start, ',');
        size_t length = end ? (size_t)(end - start) : strlen(start);

        // 截取字符串并复制到结果
        if (length < max_length - 1)
        {
            strncpy(result, start, length);
            // 手动添加 null 结尾
            result[length] = '\0';
        }
        else
        {
            // 目标长度不足，截断字符串
            strncpy(result, start, max_length - 1);
            result[max_length - 1] = '\0';
        }

        switch (n)
        {

        case (1): // 域名号

            CORS_Struct.Domain = result[0];
            //   u1_printf("CORS_Struct.Domain :%c\n", CORS_Struct.Domain );
            break;

        case (2): // 端口号
            CORS_Struct.Port = result[0];
            break;
        case (3): // 挂载点号
            CORS_Struct.Mount = atoi(result);
            //   CORS_Struct.Mount = result[0];
            break;

        case (4): // 密文
            sprintf(CORS_Struct.AccPassCiphertext, "%s", result);
            // u0_printf("@%s\n",CORS_Struct.accpass);
            break;
        }
    }
}
