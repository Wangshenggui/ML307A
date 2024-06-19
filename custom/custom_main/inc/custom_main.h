#ifndef _CUSTOM_MAIN_H
#define _CUSTOM_MAIN_H

#include <stdint.h>
#include "cm_os.h"



void Delay(uint32_t ms);
osThreadId_t osThreadCreat(const char* name, osThreadFunc_t func, osPriority_t priority, uint32_t stacksize);



#endif

