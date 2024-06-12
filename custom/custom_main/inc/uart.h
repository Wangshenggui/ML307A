#ifndef __UART_H__
#define __UART_H__

#include "cm_uart.h"
#include "stdarg.h"
#include "stddef.h"
#include "cm_os.h"
#include "cm_iomux.h"
#include "gps.h"
#include "tcp.h"

#define	EOF	(-1)
uint8_t txBuff0[600];
uint8_t txBuff1[600];
uint8_t rxBuff0[600];
uint8_t rxBuff1[800];
uint8_t rxBuff2[800];
extern uint8_t corsString[100];
    extern  char inf1[100];
    extern  char inf2[100];
    extern  char inf3[100];
typedef struct
{
uint8_t  Domain;  
uint8_t  Port;
uint8_t Mount;
char accpass[200];
}CORS_Structure;
CORS_Structure CORS_Struct;


typedef struct
{
uint8_t  Domain;  
uint8_t  Port;
uint8_t Mount;
char accpass[100];
}MOTOR_Structure;
MOTOR_Structure MOTOR_Struct;






void open_uart(void);
void u0_printf(char *str, ...);
void u1_printf(char *str, ...);
void u0_callback(void *param, uint32_t type);
void u1_callback(void *param, uint32_t type);
void uart_open(cm_uart_dev_e dev, int baudrate, void *callback);
int u0_uart_read(char *data);
int u1_uart_read(char *data);
void u1_read(char *param);

void ParseCORS(const char* string, int n);
void separateCORS(char* inputString, const char* delimiter, char* inf1, char* inf2, char* inf3);
#endif




