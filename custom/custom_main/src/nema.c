#include "nema.h"
#include "string.h"

uint8_t GGAString[100];

#define MAX_LINE_LENGTH 256

int extractFirstGGA(const char *data, char *result, size_t result_size,char *gga_start)
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

