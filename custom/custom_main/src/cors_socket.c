#include "cors_socket.h"
#include "sockets.h"
#include "stdlib.h"
#include "inet.h"
#include "netdb.h"
#include "stdio.h"
#include "string.h"
#include "uart.h"
#include "sockets.h"

//先默认
char Corsipstr[20] = "sdk.pnt.10086.cn";
int Corssockfd;
struct sockaddr_in Corsserv_addr;
int CORSport = 8002;
char ipstr[20];

char part1[20];
char part2[150];
char part3[10];
char part4[400];

char CORScount[20] = "RTCM33_GRCEpro";

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

void CreateCorsSocket(void)
{
    input:
    print_ipv4_address(Corsipstr); // 获取动态IP
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
    Corsserv_addr.sin_port = htons(CORSport);

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

    // 连接成功，可以在此发送和接收数据
    sprintf((char*)part1, "%s", "GET /");
    sprintf((char*)part2, "%s", " HTTP/1.0\r\nUser-Agent: NTRIP GNSSInternetRadio/1.4.10\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic ");
    sprintf((char*)part3, "%s", "\r\n\r\n");
    sprintf((char*)part4, "%s%s%s%s%s", part1, CORScount, part2, "Y2VkcjIxNTEzOmZ5eDY5NzQ2", part3);

    if (send(Corssockfd, part4, strlen((char*)part4), 0) < 0)
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




