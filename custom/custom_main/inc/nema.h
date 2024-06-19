#ifndef _NEMA_H
#define _NEMA_H

#include "custom_main.h"
#include "stddef.h"

extern uint8_t GGAString[100];

int extractFirstGGA(const char *data, char *result, size_t result_size,char *gga_start);


#endif
