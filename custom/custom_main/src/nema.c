#include "nema.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// 定义GGA和RMC数组
uint8_t GGAString[100];
uint8_t RMCString[100];

// 定义相关结构体
GPGGA_Structure GPGGA_Struct;
GPRMC_Structure GPRMC_Struct;

// #define MAX_LINE_LENGTH 256

// 提取gga_start参数开头的信息，\r\n结束
int extractFirstxxx(const char *data, char *result, size_t result_size, char *gga_start)
{
    const char *ptr = data;

    // 使用 strstr 查找字符串中第一个 "gga_start"
    ptr = strstr(ptr, gga_start);
    if (ptr != NULL)
    {
        // 使用 strstr 查找 "\r\n" 作为行结束标志
        const char *end = strstr(ptr, "\r\n");
        if (end != NULL)
        {
            size_t length = end - ptr;
            if (length < result_size)
            {
                strncpy(result, ptr, length);
                result[length] = '\0'; // 确保字符串以 null 终止
                return 1;              // 找到并成功复制
            }
            else
            {
                return -1; // 缓冲区太小
            }
        }
        else
        {
            // 如果最后一行没有 "\r\n"
            size_t length = strlen(ptr);
            if (length < result_size)
            {
                strncpy(result, ptr, length);
                result[length] = '\0';
                return 1; // 找到并成功复制
            }
            else
            {
                return -1; // 缓冲区太小
            }
        }
    }

    return 0; // 没有找到 "$GNGGA" 行
}

// 裁剪字符串
static char *substring(char *dst, char *src, int start, int len)
{
    int length = 1000; // ×î´ó³¤¶È
    if (start >= length || start < 0)
        return NULL;
    if (len > length)
        len = length - start;
    src += start;
    while (len--)
    {
        *(dst++) = *(src++);
    }
    *(dst++) = '\0';
    return dst;
}

// 解析GGA信息
void ParseGPGGA(const char *string, int n)
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
    if (strncmp(checksum_str, (const char *)&string[i + 1], 2) == 0) // 通过
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
        case (1): // 时间
            substring(GPGGA_Struct.str, result, 0, 2);
            GPGGA_Struct.Hour = atoi(GPGGA_Struct.str) + 8;
            substring(GPGGA_Struct.str, result, 2, 2);
            GPGGA_Struct.Min = atoi(GPGGA_Struct.str);
            substring(GPGGA_Struct.str, result, 4, 2);
            GPGGA_Struct.Sec = atoi(GPGGA_Struct.str);
            break;
        case (2): // 纬度
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Latitude = atof(GPGGA_Struct.str);
            break;
        case (4): // 经度
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Longitude = atof(GPGGA_Struct.str);
            break;
        case (6): // 定位状态
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.PositioningMode = (int)atoi(GPGGA_Struct.str);
            break;
        case (7): // 使用的卫星数量
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.sats = (int)atoi(GPGGA_Struct.str);
            break;
        case (8): // 水平精度因子
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Radius = atof(GPGGA_Struct.str);
            break;
        case (9): // 海拔高度
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Height = atof(GPGGA_Struct.str);
            break;
        }
    }
}

// 解析RMC信息
void ParseGPRMC(const char *string, int n)
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
    if (strncmp(checksum_str, (const char *)&string[i + 1], 2) == 0) // 通过
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
        case (1): // 时间
            // 解析时间
            substring(GPRMC_Struct.str, result, 0, 2);
            GPRMC_Struct.Hour = (uint8_t)atof(GPRMC_Struct.str) + 8;
            substring(GPRMC_Struct.str, result, 2, 2);
            GPRMC_Struct.Min = (uint8_t)atof(GPRMC_Struct.str);
            substring(GPRMC_Struct.str, result, 4, 2);
            GPRMC_Struct.Sec = (uint8_t)atof(GPRMC_Struct.str);
            break;
        case (2): // 定位状态
            GPRMC_Struct.LocationState = result[0];
            break;
        case (3): // 纬度
            // 解析纬度
            // 度
            substring(GPRMC_Struct.str, result, 0, 2);
            GPRMC_Struct.LatitudeDegrees = (uint8_t)atoi(GPRMC_Struct.str);
            // 分
            substring(GPRMC_Struct.str, result, 2, 2);
            GPRMC_Struct.LatitudeMinutes = (uint8_t)atoi(GPRMC_Struct.str);
            /*GPS*/
            if (strlen(result) == 11)
            {
                // 秒
                substring(GPRMC_Struct.str, result, 5, 6);
                GPRMC_Struct.LatitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.000001 * 60 + 0.5);
            }
            else if (strlen(result) == 9)
            {
                // 秒
                substring(GPRMC_Struct.str, result, 5, 4);
                GPRMC_Struct.LatitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.0001 * 60 + 0.5);
            }
            /*RTK*/
            else if (strlen(result) == 13)
            {
                // 秒
                substring(GPRMC_Struct.str, result, 5, 8);
                GPRMC_Struct.LatitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.00000001 * 60 + 0.5);
            }

            break;
        case (4): // N/S
            GPRMC_Struct.LatitudeDir = result[0];
            break;
        case (5): // 经度
            // 解析经度
            // 度
            substring(GPRMC_Struct.str, result, 0, 3);
            GPRMC_Struct.LongitudeDegrees = (uint8_t)atoi(GPRMC_Struct.str);
            // 分
            substring(GPRMC_Struct.str, result, 3, 2);
            GPRMC_Struct.LongitudeMinutes = (uint8_t)atoi(GPRMC_Struct.str);

            /*GPS*/
            if (strlen(result) == 11)
            {
                // 秒
                substring(GPRMC_Struct.str, result, 6, 6);
                GPRMC_Struct.LongitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.000001 * 60 + 0.5);
            }
            else if (strlen(result) == 9)
            {
                // 秒
                substring(GPRMC_Struct.str, result, 6, 4);
                GPRMC_Struct.LongitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.0001 * 60 + 0.5);
            }
            /*RTK*/
            else if (strlen(result) == 14)
            {
                // 秒
                substring(GPRMC_Struct.str, result, 6, 8);
                GPRMC_Struct.LongitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.00000001 * 60 + 0.5);
            }
            break;
        case (6): // E/W
            GPRMC_Struct.LongitudeDir = result[0];
            break;
        case (7): // 地面速度
            //
            GPRMC_Struct.Speed = (float)atof(result) * 1.852;
            break;
        case (8): // 航向角
            GPRMC_Struct.CourseAngle = (float)atof(result);
            break;
        case (9): // 日期
            // 解析日期
            substring(GPRMC_Struct.str, result, 0, 2);
            GPRMC_Struct.Day = (uint8_t)atof(GPRMC_Struct.str);
            substring(GPRMC_Struct.str, result, 2, 2);
            GPRMC_Struct.Month = (uint8_t)atof(GPRMC_Struct.str);
            substring(GPRMC_Struct.str, result, 4, 2);
            GPRMC_Struct.Year = (uint8_t)atof(GPRMC_Struct.str) + 2000;
            break;
        case (12): // 定位模式
            GPRMC_Struct.ModeIndicator = result[0];
            break;
        }
    }
}
