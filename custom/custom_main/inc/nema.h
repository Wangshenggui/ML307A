#ifndef _NEMA_H
#define _NEMA_H

#include "custom_main.h"
#include "stddef.h"

extern uint8_t GGAString[100];

typedef struct
{
    char str[100];

    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;
    char PositioningMode1[10];
    int PositioningMode;

    double Latitude;
    double Longitude;

    double Height;
    double Radius;
    int sats;
} GPGGA_Structure;
extern GPGGA_Structure GPGGA_Struct;

int extractFirstxxx(const char *data, char *result, size_t result_size,char *gga_start);
void ParseGPGGA(const char *string, int n);


#endif
