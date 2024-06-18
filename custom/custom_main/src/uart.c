#include "uart.h"
#include <stdarg.h>


osSemaphoreId_t uart0_read_sem = NULL;
osSemaphoreId_t uart1_read_sem = NULL;

uint8_t txBuff0[1024];
uint8_t txBuff1[1024];
void u0_printf(char* str, ...)
{
    va_list args;
    int len;

    va_start(args, str);
    len = vsnprintf((char*)txBuff0, 1024, str, args);
    va_end(args);
    cm_uart_write(CM_UART_DEV_0, txBuff0, len, 5000);
}
void u1_printf(char* str, ...)
{
    va_list args;
    int len;

    va_start(args, str);
    len = vsnprintf((char*)txBuff1, 1024, str, args);
    va_end(args);
    cm_uart_write(CM_UART_DEV_1, txBuff1, len, 5000);
}

void uart0_init(void)
{
    uart0_read_sem = osSemaphoreNew(1, 0, NULL);

    cm_uart_cfg_t config =
        {
            CM_UART_BYTE_SIZE_8,
            CM_UART_PARITY_NONE,
            CM_UART_STOP_BIT_ONE,
            CM_UART_FLOW_CTRL_NONE,
            CM_UART_BAUDRATE_115200};
    cm_uart_event_t event =
        {
            CM_UART_EVENT_TYPE_RX_ARRIVED | CM_UART_EVENT_TYPE_RX_OVERFLOW,
            "uart0",
            u0_callback};

    cm_iomux_set_pin_func(CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1);
    cm_iomux_set_pin_func(CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1);

    cm_uart_register_event(CM_UART_DEV_0, &event);

    cm_uart_open(CM_UART_DEV_0, &config);

    /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
    cm_iomux_set_pin_cmd(CM_IOMUX_PIN_17, CM_IOMUX_PINCMD1_LPMEDEG, CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
}
void uart1_init(void)
{
    uart1_read_sem = osSemaphoreNew(1, 0, NULL);

    cm_uart_cfg_t config =
        {
            CM_UART_BYTE_SIZE_8,
            CM_UART_PARITY_NONE,
            CM_UART_STOP_BIT_ONE,
            CM_UART_FLOW_CTRL_NONE,
            CM_UART_BAUDRATE_115200};
    cm_uart_event_t event =
        {
            CM_UART_EVENT_TYPE_RX_ARRIVED | CM_UART_EVENT_TYPE_RX_OVERFLOW,
            "uart1",
            u1_callback};

    cm_iomux_set_pin_func(CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1);
    cm_iomux_set_pin_func(CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1);

    cm_uart_register_event(CM_UART_DEV_1, &event);

    cm_uart_open(CM_UART_DEV_1, &config);
}

void u0_callback(void *param, uint32_t type)
{
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        /* 接收到新的数据  */
        osSemaphoreRelease(uart0_read_sem);
    }
    else if (CM_UART_EVENT_TYPE_RX_OVERFLOW & type)
    {
        /* 接收FIFO缓存溢出 */
    }
}
void u1_callback(void *param, uint32_t type)
{
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        /* 接收到新的数据  */
        osSemaphoreRelease(uart1_read_sem);
    }
    else if (CM_UART_EVENT_TYPE_RX_OVERFLOW & type)
    {
        /* 接收FIFO缓存溢出 */
    }
}