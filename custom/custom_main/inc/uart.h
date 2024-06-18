#ifndef _UART_H
#define _UART_H

#include "cm_uart.h"
#include "cm_os.h"
#include "cm_iomux.h"
#include "custom_main.h"


extern osSemaphoreId_t uart0_read_sem;
extern osSemaphoreId_t uart1_read_sem;

void uart0_init(void);
void uart1_init(void);
void u0_printf(char* str, ...);
void u1_printf(char* str, ...);
void u0_read(char *param);
void u1_read(char *param);
void u0_callback(void *param, uint32_t type);
void u1_callback(void *param, uint32_t type);


#endif
