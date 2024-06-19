#ifndef _MCU_UART_H
#define _MCU_UART_H

#include "cm_uart.h"
#include "cm_os.h"
#include "cm_iomux.h"
#include "custom_main.h"

extern uint8_t CORSString[100];

typedef struct
{
    uint8_t Domain;
    uint8_t Port;
    uint8_t Mount;
    
    char Corsipstr[20];//ip/域名
    int CORSport;//端口
    char CORSMount[20];//挂载点
    char AccPassCiphertext[200];
} CORS_Structure;
extern CORS_Structure CORS_Struct;

void ParsingCORS(const char *string);
void ParseCORS(const char *string, int n);

#endif
