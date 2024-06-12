/*********************************************************
*  @file    cm_main.h
*  @brief   ML302 OpenCPU main header file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/08
********************************************************/
#ifndef __CM_MAIN_H__
#define __CM_MAIN_H__

#include "uart.h"
#include "cm_iomux.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "cm_os.h"
#include "cm_mem.h"
#include "cm_common.h"
#include "cm_sys.h"

#include "sockets.h"
#include "cm_modem.h"
#include <inet.h>
#include <netdb.h>

#include "stdint.h"

#include "stdbool.h"

#include <time.h>
#include <tcp.h>
#include <gps.h>




void Delay(uint32_t ms);
osThreadId_t osThreadCreat(const char *name, osThreadFunc_t func, osPriority_t priority, uint32_t stacksize);
int cm_opencpu_entry(char *param);





#endif
