#include "cors_socket.h"
#include "sockets.h"
#include "stdlib.h"
#include "inet.h"
#include "netdb.h"
#include "stdio.h"
#include "string.h"
#include "uart.h"
#include "sockets.h"
#include <mcu_uart.h>

//创建Cors嵌套字
int Corssockfd;

struct sockaddr_in Corsserv_addr;
char ipstr[20];



uint8_t TCPrxBuf1[400];

int count1 = 0;


void print_ipv4_address(const char* hostname)
{
    struct addrinfo hints, * res, * p;
    int status;
    int found = 1; // 用于标记是否找到了IPv4地址

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // 只关注IPv4
    hints.ai_socktype = SOCK_STREAM;

    while (found)
    { 
        // 无限循环，持续查询并打印IP地址
        status = getaddrinfo(hostname, NULL, &hints, &res);
        if (status != 0)
        {
            // fprintf(stderr, "getaddrinfo error: %s\n", strerror(status));
            continue; // 查询失败，继续下一次循环
        }

        // 遍历返回的结果，直到找到IPv4地址
        for (p = res; p != NULL; p = p->ai_next)
        {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            if (inet_ntop(AF_INET, &ipv4->sin_addr, (void*)ipstr, sizeof ipstr) != NULL)
            {
                found = 0;
                break; // 找到后退出循环
            }
        }

        freeaddrinfo(res); // 释放getaddrinfo分配的内存
    }
}


/*
Corsipstr       ip
CORSport        端口
CORSMount       挂载点
CORSAccPass     账号密码密文
*/
void CreateCorsSocket(void)
{
    input:
    print_ipv4_address(CORS_Struct.Corsipstr); // 获取动态IP
    // 创建TCP套接字
    if ((Corssockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        u0_printf("socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        u0_printf("socket creation Success\n");
    }
    // 初始化服务器地址结构
    memset(&Corsserv_addr, 0, sizeof(Corsserv_addr));
    Corsserv_addr.sin_family = AF_INET;
    Corsserv_addr.sin_addr.s_addr = inet_addr(ipstr);
    Corsserv_addr.sin_port = htons(CORS_Struct.CORSport);

    // 尝试连接到服务器
    if (connect(Corssockfd, (struct sockaddr*)&Corsserv_addr, sizeof(Corsserv_addr)) < 0)
    {
        u0_printf("connection failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        u0_printf("connection Success\n");
    }

    char temp_str[400];
    // 连接成功，可以在此发送和接收数据
    sprintf((char*)temp_str, "%s%s%s%s%s", "GET /", CORS_Struct.CORSMount, " HTTP/1.0\r\nUser-Agent: NTRIP GNSSInternetRadio/1.4.10\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic ", CORS_Struct.AccPassCiphertext, "\r\n\r\n");

    if (send(Corssockfd, temp_str, strlen((char*)temp_str), 0) < 0)
    {
        u0_printf("failed\r\n");
        goto input;
        exit(EXIT_FAILURE);
    }
    else
    {
        u0_printf("success\r\n");
    }

    if (recv(Corssockfd, TCPrxBuf1, sizeof(TCPrxBuf1), 0) < 0)
    {
        u0_printf("receive failed\r\n");
        goto input;
        exit(EXIT_FAILURE);
    }
    else 
    {
        u0_printf("receive success\r\n");
    }

    if (strcmp((char*)TCPrxBuf1, "ICY 200 OK\r\n") == 0)
    {
        u0_printf("$CORS,OK\r\n");
    }
    else if (strcmp((char*)TCPrxBuf1, "ERROR - Bad Password\r\n") == 0)
    {
        u0_printf("$CORS,ERRORpassword\r\n");
        count1++;
        if (count1 > 4)
        {
            while (1)
            {
                Delay(10000);
            }
        }
        goto input;
    }

    
}




