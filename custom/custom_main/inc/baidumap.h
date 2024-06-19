#ifndef _BAIDUMAP_H
#define _BAIDUMAP_H

#include "cm_uart.h"
#include "cm_os.h"
#include "cm_iomux.h"
#include "custom_main.h"

void UpBaiduMap(void);
void print_ipv4_addressBauduMap(const char* hostname);
void CreateBaiduMapSocket(void);


#endif
