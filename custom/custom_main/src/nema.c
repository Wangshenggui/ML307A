#include "nema.h"
#include "string.h"


uint8_t GGAString[100];


void separateString(char *inputString, const char *delimiter, char *info1, char *info2, char *info3)
{
    char *token = strtok(inputString, delimiter);

    // 使用 strncpy 将每个信息复制到相应的指针指向的缓冲区
    if (token != NULL)
    {
        strncpy(info1, token, strlen(token));
        info1[strlen(token)] = '\0';
    }
    else
    {
        info1[0] = '\0';
    }

    token = strtok(NULL, delimiter);

    if (token != NULL)
    {
        strncpy(info2, token, strlen(token));
        info2[strlen(token)] = '\0';
    }
    else
    {
        info2[0] = '\0';
    }

    token = strtok(NULL, delimiter);

    if (token != NULL)
    {
        strncpy(info3, token, strlen(token));
        info3[strlen(token)] = '\0';
    }
    else
    {
        info3[0] = '\0';
    }
}


