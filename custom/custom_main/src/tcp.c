
#include "tcp.h"

char info1[100];
char info2[100];
char info3[100];

int sockfd;
struct sockaddr_in serv_addr;

struct addrinfo hints, * res, * p;
int status;
// 移动cors 120.253.226.97
// 百度鹰眼  163.177.17.114
// TCP助手 112.125.89.8

int SERVER_PORT = 8881;

const char ipstr1[16];
// const char ipstr1[16] = "8.137.81.229";
char Corsipstr[20];

// const char Corsipstr[16] = "120.253.226.97";
// #define Cors_PORT 8002
int Corssockfd;
struct sockaddr_in Corsserv_addr;
char TCPrxBuff[1002];

char ipstr[20];
char baiduipstr[20];
int CORSport;

char CORScount[20];
char CORSaccpass[100];

char part1[20];
char part2[150];
char part3[10];
char part4[400];

char strx1[200];
char strx2[200];

///////////////////////////////
int tcp1sockfd;
struct sockaddr_in tcp1serv_addr;

uint8_t TCPrxBuf1[400];
uint8_t TCPrxBuf2[1200];
uint8_t TCPrxBuf3[400];

char SendStr[1000] = { 0 };
char tempString[200];

int timeout = 0;
int timeout1 = 0;

char* delimiter = "\r\n";
double wgs84_lon = 0;
double wgs84_lat = 0;

double gcj02_lon = 0;
double gcj02_lat = 0;

double bd09_lon = 0;
double bd09_lat = 0;

int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;
void print_ipv4_address(const char* hostname)
{

    struct addrinfo hints, * res, * p;
    int status;
    // u1_printf("1\r\n\r\n");
    int found = 1; // 用于标记是否找到了IPv4地址

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // 只关注IPv4
    hints.ai_socktype = SOCK_STREAM;

    while (found)
    { // 无限循环，持续查询并打印IP地址
        status = getaddrinfo(hostname, NULL, &hints, &res);
        // u1_printf("2\r\n\r\n");
        if (status != 0)
        {
            fprintf(stderr, "getaddrinfo error: %s\n", strerror(status));
            //                u1_printf("3\r\n\r\n");
            continue; // 查询失败，继续下一次循环
        }

        // 遍历返回的结果，直到找到IPv4地址
        for (p = res; p != NULL; p = p->ai_next)
        {

            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            if (inet_ntop(AF_INET, &ipv4->sin_addr, (void*)ipstr, sizeof ipstr) != NULL)
            {

                //  u1_printf("%s  IPv4 Address:%s",hostname,ipstr);
                found = 0;
                break; // 找到后退出循环
            }
        }

        freeaddrinfo(res); // 释放getaddrinfo分配的内存
    }
}

void print_ipv4_address1(const char* hostname)
{

    struct addrinfo hints, * res, * p;
    int status;
    // u1_printf("1\r\n\r\n");
    int found = 1; // 用于标记是否找到了IPv4地址

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // 只关注IPv4
    hints.ai_socktype = SOCK_STREAM;

    while (found)
    { // 无限循环，持续查询并打印IP地址
        status = getaddrinfo(hostname, NULL, &hints, &res);
        // u1_printf("2\r\n\r\n");
        if (status != 0)
        {
            fprintf(stderr, "getaddrinfo error: %s\n", strerror(status));
            //                       u1_printf("3\r\n\r\n");
            continue; // 查询失败，继续下一次循环
        }

        // 遍历返回的结果，直到找到IPv4地址
        for (p = res; p != NULL; p = p->ai_next)
        {

            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            if (inet_ntop(AF_INET, &ipv4->sin_addr, (void*)baiduipstr, sizeof baiduipstr) != NULL)
            {

                //  u1_printf("%s  IPv4 Address:%s",hostname,ipstr);
                found = 0;
                break; // 找到后退出循环
            }
        }

        freeaddrinfo(res); // 释放getaddrinfo分配的内存
    }
}

void Waitnet()
{
    u1_printf("Waiting for inquire... \r\n\r\n");
    while (1) // 等待模块联网,死循环直到模块联网
    {
        if (timeout > 4)
        {
            cm_pm_reboot();
            u1_printf("The system will restart!\r\n");
        }
        if (cm_modem_get_pdp_state(1) == 1)
        {
            u1_printf("network ok!\r\n");
            break;
        }
        timeout++;
        u1_printf("Waiting for networking  %d\r\n", timeout);

        Delay(1000);
    }
}

void Waitnet1()
{
    u0_printf("Waiting for inquire... \r\n\r\n");
    while (1) // 等待模块联网,死循环直到模块联网
    {
        if (timeout1 > 4)
        {
            cm_pm_reboot();
            u0_printf("The system will restart!\r\n");
        }
        if (cm_modem_get_pdp_state(1) == 1)
        {
            u0_printf("network ok!\r\n");

            break;
        }
        timeout1++;
        u0_printf("Waiting for networking  %d\r\n", timeout1);

        Delay(1000);
    }
}

void UpBaidu(void)
{
    Waitnet();

    //   u1_printf("$Baidu_STAR\r\n");
input2:
    print_ipv4_address1("www.xmap.ltd"); // 获取动态IP
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        u1_printf("socket creation failed\n");
        // count4++;
        // if (count4 >= 3)
        // {
        //     count4 = 0;
        //     cm_pm_reboot();
        // }
        // goto input2;
        exit(EXIT_FAILURE);
    }
    // 初始化服务器地址结构
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(baiduipstr);
    serv_addr.sin_port = htons(SERVER_PORT);

    // u1_printf("SERVER_PORT: %d\n", SERVER_PORT);
    u1_printf("SERVER_IP: %s\n", baiduipstr);

    // 尝试连接到服务器
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        u1_printf("connection failed\n");
        goto input2;
        // exit(EXIT_FAILURE);
    }
    // 连接成功，可以在此发送和接收数据

    while (1)
    {

        ParseGPGGA((char*)GGAString, 2);
        ParseGPGGA((char*)GGAString, 4);
        ParseGPGGA((char*)GGAString, 6);
        ParseGPGGA((char*)GGAString, 7);
        ParseGPGGA((char*)GGAString, 9);

        ParseGPRMC((char*)RMCString, 7);

        wgs84_lon = dms_to_degrees(GPGGA_Struct.Longitude);
        wgs84_lat = dms_to_degrees(GPGGA_Struct.Latitude);

        gcj02_lon = wgs84togcj02(wgs84_lon, wgs84_lat).longitude;
        gcj02_lat = wgs84togcj02(wgs84_lon, wgs84_lat).latitude;

        bd09_lon = gcj02tobd09(gcj02_lon, gcj02_lat).longitude;
        bd09_lat = gcj02tobd09(gcj02_lon, gcj02_lat).latitude;
        if (1)

        {

            // char p1[9] = "{\"lng\":";
            // char p2[7] = ",\"lat\":";
            // char p3[1] = "}";
            //{"lng": 106.621214,"lat": 26.38120,"rtkstate":4,"speed":5,"rtknum":20}
            sprintf(strx1, "\
{\"lng\":%0.7f,\
\"lat\":%0.7f,\
\"rtkstate\":%d,\
\"speed\":%f,\
\"rtknum\":%d,\
\"altitude\":%0.2f\
}",
bd09_lon,
bd09_lat,
GPGGA_Struct.PositioningMode,
GPRMC_Struct.Speed,
GPGGA_Struct.sats,
// SLAVE_Struct.Slave_State[0],
// SLAVE_Struct.Slave_State[1],
// SLAVE_Struct.Slave_State[2],
// SLAVE_Struct.Slave_State[3],
// SLAVE_Struct.ReadSpeed1[0],
// SLAVE_Struct.ReadSpeed1[1],
// SLAVE_Struct.ReadSpeed1[2],
// SLAVE_Struct.ReadSpeed1[3],
// SLAVE_Struct.ReadSpeed2[0],
// SLAVE_Struct.ReadSpeed2[1],
// SLAVE_Struct.ReadSpeed2[2],
// SLAVE_Struct.ReadSpeed2[3],

GPGGA_Struct.Height);

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
SLAVE_Struct.Slave_State[0],
SLAVE_Struct.Slave_State[1],
SLAVE_Struct.Slave_State[2],
SLAVE_Struct.Slave_State[3],
SLAVE_Struct.ReadSpeed1[0],
SLAVE_Struct.ReadSpeed1[1],
SLAVE_Struct.ReadSpeed1[2],
SLAVE_Struct.ReadSpeed1[3],
SLAVE_Struct.ReadSpeed2[0],
SLAVE_Struct.ReadSpeed2[1],
SLAVE_Struct.ReadSpeed2[2],
SLAVE_Struct.ReadSpeed2[3]);

            if (send(sockfd, strx1, strlen((char*)strx1), 0) < 0)
            {
                u1_printf("send failed\r\n");
                goto input2;
            }
            Delay(0);
            if (send(sockfd, strx2, strlen((char*)strx2), 0) < 0)
            {
                u1_printf("send failed\r\n");
                goto input2;
            }

            // u1_printf("%s\n", strx1);
            // u1_printf("%s\n", strx2);
            memset(TCPrxBuf3, 0, sizeof(TCPrxBuf3));
            // sprintf(TCPrxBuf3,"%s","0");

            if (recv(sockfd, TCPrxBuf3, sizeof(TCPrxBuf3), 0) < 0)
            {
                u1_printf("recive failed\r\n");
                goto input2;
                // exit(EXIT_FAILURE);
            }
            cm_uart_write(CM_UART_DEV_1, TCPrxBuf3, strlen((char*)TCPrxBuf3), 1000);

            // uint32_t a1 = osThreadGetStackSpace(GetCors);
            //              u1_printf("GetCors:%ld",a1 );
            //       uint32_t a2 =osThreadGetStackSpace(UpBaidu);       
            //              u1_printf("UpBaidu:%ld", a2);
            memset(TCPrxBuf3, 0, sizeof(TCPrxBuf3));
            Delay(350);
        }

        else
        {
            count3++;
            if (count3 >= 15)
            {
                count3 = 0;
                goto input2;
            }

            Delay(200);
            // goto input2;
        }
    }

    close(sockfd);
}

void GetCors(void)
{

    Waitnet1();

    // u1_printf("$CORS,STAR\r\n");
    //  Delay(1000);

input:
    // sdk.pnt.10086.cn
    // rtk.ntrip.qxwz.com

    print_ipv4_address(Corsipstr); // 获取动态IP
    // 创建TCP套接字
    if ((Corssockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        u1_printf("socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    // 初始化服务器地址结构
    memset(&Corsserv_addr, 0, sizeof(Corsserv_addr));
    Corsserv_addr.sin_family = AF_INET;
    Corsserv_addr.sin_addr.s_addr = inet_addr(ipstr);
    Corsserv_addr.sin_port = htons(CORSport);

    // u1_printf("SERVER_PORT: %d\n", Cors_PORT);
    // u1_printf("SERVER_IP: %s\n", ipstr);

    // 尝试连接到服务器
    if (connect(Corssockfd, (struct sockaddr*)&Corsserv_addr, sizeof(Corsserv_addr)) < 0)
    {
        u1_printf("connection failed\n");

        exit(EXIT_FAILURE);
    }
    // 连接成功，可以在此发送和接收数据

    sprintf((char*)part1, "%s", "GET /");
    sprintf((char*)part2, "%s", " HTTP/1.0\r\nUser-Agent: NTRIP GNSSInternetRadio/1.4.10\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic ");
    sprintf((char*)part3, "%s", "\r\n\r\n");
    sprintf((char*)part4, "%s%s%s%s%s", part1, CORScount, part2, CORS_Struct.accpass, part3);

    // char message[190] = "GET /RTCM33_GRCEpro HTTP/1.0\r\nUser-Agent: NTRIP GNSSInternetRadio/1.4.10\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic Y2VkcjE3NjIyOmhtMGRjdm50\r\n\r\n";

    if (send(Corssockfd, part4, strlen((char*)part4), 0) < 0)
    {
        u1_printf("failed\r\n");
        goto input;
        exit(EXIT_FAILURE);
    }

    // u0_printf("send: %s\n", part4);
    // u0_printf("$CORS,OK\r\n");

    if (recv(Corssockfd, TCPrxBuf1, sizeof(TCPrxBuf1), 0) < 0)
    {
        u1_printf("receive failed\r\n");
        goto input;
        exit(EXIT_FAILURE);
    }
    // u0_printf("recive: %s\n", TCPrxBuf1);
    if (strcmp((char*)TCPrxBuf1, "ICY 200 OK\r\n") == 0)
    {
        u1_printf("$CORS,OK\r\n");
    }
    else if (strcmp((char*)TCPrxBuf1, "ERROR - Bad Password\r\n") == 0)
    {
        u1_printf("$CORS,ERRORpassword\r\n");
        count1++;
        if (count1 > 4)
        {
            while (1)
            {
                Delay(10000);
            }

            // cm_pm_reboot();
        }
        goto input;
    }

    while (1)
    {
        if (strlen((char*)GGAString) > 41)
        {
            sprintf((char*)GGAString1, "%s%c%c", (char*)GGAString, 0x0D, 0x0A);
            if (send(Corssockfd, GGAString1, strlen((char*)GGAString1), 0) < 0)
            {
                u1_printf("send failed\r\n");
                goto input;
                //exit(EXIT_FAILURE);
            }
            //   u0_printf("%s", GGAString1);
            Delay(10);
            if (recv(Corssockfd, TCPrxBuf2, sizeof(TCPrxBuf2), 0) < 0)
            {
                u1_printf("recive failed\r\n");
                goto input;
                //exit(EXIT_FAILURE);
            }
            cm_uart_write(CM_UART_DEV_0, TCPrxBuf2, sizeof(TCPrxBuf2), 1000);
            // u1_printf("%s", TCPrxBuf3);

            // memset(TCPrxBuf3,0,sizeof(TCPrxBuf3));
            Delay(1000);
            count2 = 0;
        }
        else
        {
            count2++;
            if (count2 >= 15)
            {
                count2 = 0;
                goto input;
            }

            Delay(200);
            // goto input;
        }
    }

    //close(Corssockfd);
}

void swit(void)
{

    ParseCORS((char*)inf1, 1);
    if (CORS_Struct.Domain == '0')
    {
        cm_pm_reboot();
    }
    if (CORS_Struct.Domain == '1')
    {

        sprintf(Corsipstr, "%s", "sdk.pnt.10086.cn");
    }
    if (CORS_Struct.Domain == '2')
    {
        sprintf(Corsipstr, "%s", "rtk.ntrip.qxwz.com");
    }
    ParseCORS((char*)inf1, 2);
    if (CORS_Struct.Port == '3')
    {
        CORSport = 8001;
    }
    if (CORS_Struct.Port == '4')
    {
        CORSport = 8002;
    }
    ParseCORS((char*)inf1, 3);
    if (CORS_Struct.Mount == 5)
    {
        sprintf(CORScount, "%s", "RTCM33_GRCEJ");
    }
    if (CORS_Struct.Mount == 6)
    {
        sprintf(CORScount, "%s", "RTCM33_GRCE");
    }
    if (CORS_Struct.Mount == 7)
    {
        sprintf(CORScount, "%s", "RTCM33_GRC");
    }
    if (CORS_Struct.Mount == 8)
    {
        sprintf(CORScount, "%s", "RTCM30_GR");
    }
    if (CORS_Struct.Mount == 9)
    {
        sprintf(CORScount, "%s", "RTCM33_GRCEpro");
    }
    if (CORS_Struct.Mount == 10)
    {
        sprintf(CORScount, "%s", "RTCM32_GGB");
    }
    if (CORS_Struct.Mount == 11)
    {
        sprintf(CORScount, "%s", "RTCM30_GG");
    }

    ParseCORS((char*)inf1, 4);
}
