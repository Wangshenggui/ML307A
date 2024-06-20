#include "baidumap.h"
#include <stdarg.h>
#include <stdio.h>
#include "nema.h"
#include "string.h"
#include <mcu_uart.h>
#include "uart.h"
#include "cm_modem.h"
#include <sockets.h>
#include "stdlib.h"
#include "cors_socket.h"
#include "netdb.h"
#include "coor_trans.h"

// 创建BaiduMap嵌套字
int BaiduMapsockfd;
struct sockaddr_in BaiduMapserv_addr;
char Baiduipstr[20];

char strx1[200];
char strx2[200];

double wgs84_lon = 0;
double wgs84_lat = 0;

double gcj02_lon = 0;
double gcj02_lat = 0;

double bd09_lon = 0;
double bd09_lat = 0;

void UpBaiduMap(void)
{
    while (cm_modem_get_pdp_state(1) != 1)
    {
        Delay(100);
    }

    CreateBaiduMapSocket();

    while (1)
    {
        u1_printf("666666666\r\n");
        Delay(100);
    }
}

void print_ipv4_addressBauduMap(const char *hostname)
{
    struct addrinfo hints, *res, *p;
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
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            if (inet_ntop(AF_INET, &ipv4->sin_addr, (void *)Baiduipstr, sizeof Baiduipstr) != NULL)
            {
                found = 0;
                break; // 找到后退出循环
            }
        }

        freeaddrinfo(res); // 释放getaddrinfo分配的内存
    }
}

void CreateBaiduMapSocket(void)
{
input:
    print_ipv4_addressBauduMap("www.xmap.ltd"); // 获取动态IP
    if ((BaiduMapsockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        u1_printf("socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    // 初始化服务器地址结构
    memset(&BaiduMapserv_addr, 0, sizeof(BaiduMapserv_addr));
    BaiduMapserv_addr.sin_family = AF_INET;
    BaiduMapserv_addr.sin_addr.s_addr = inet_addr(Baiduipstr);
    BaiduMapserv_addr.sin_port = htons(8881); // 百度服务器端口号

    u1_printf("SERVER_IP: %s\n", Baiduipstr);

    // 尝试连接到服务器
    if (connect(BaiduMapsockfd, (struct sockaddr *)&BaiduMapserv_addr, sizeof(BaiduMapserv_addr)) < 0)
    {
        u1_printf("connection failed\n");
        goto input;
    }

    // 连接成功，可以在此发送和接收数据
    while (1)
    {
        ParseGPGGA((char *)GGAString, 2);
        ParseGPGGA((char *)GGAString, 4);
        ParseGPGGA((char *)GGAString, 6);
        ParseGPGGA((char *)GGAString, 7);
        ParseGPGGA((char *)GGAString, 9);

        ParseGPRMC((char *)RMCString, 7);

        wgs84_lon = dms_to_degrees(GPGGA_Struct.Longitude);
        wgs84_lat = dms_to_degrees(GPGGA_Struct.Latitude);

        gcj02_lon = wgs84togcj02(wgs84_lon, wgs84_lat).longitude;
        gcj02_lat = wgs84togcj02(wgs84_lon, wgs84_lat).latitude;

        bd09_lon = gcj02tobd09(gcj02_lon, gcj02_lat).longitude;
        bd09_lat = gcj02tobd09(gcj02_lon, gcj02_lat).latitude;

        u1_printf("%f----%f\r\n",bd09_lon,bd09_lat);

        if (1)
        {
            sprintf(strx1, "\
{\"lng\":%0.7f,\
\"lat\":%0.7f,\
\"rtkstate\":%d,\
\"speed\":%f,\
\"rtknum\":%d,\
\"altitude\":%0.2f\
}",
                    1.23,
                    4.56,
                    2,
                    5.67,
                    1,
                    2.58);

            sprintf(strx2, "\
{\"S1\":%d,\
\"S2\":%d,\
\"S3\":%d,\
\"S4\":%d,\
\"r1\":%d,\
\"r2\":%d,\
\"r3\":%d,\
\"r4\":%d,\
\"r5\":%d,\
\"r6\":%d,\
\"r7\":%d,\
\"r8\":%d\
}",
                    1,
                    1,
                    0,
                    1,
                    12,
                    23,
                    34,
                    45,
                    56,
                    67,
                    78,
                    rand() % 100);

            if (send(BaiduMapsockfd, strx1, strlen((char *)strx1), 0) < 0)
            {
                u1_printf("send failed\r\n");
                goto input;
            }
            Delay(5);
            if (send(BaiduMapsockfd, strx2, strlen((char *)strx2), 0) < 0)
            {
                u1_printf("send failed\r\n");
                goto input;
            }
            Delay(500);
        }
    }

    close(BaiduMapsockfd);
}
