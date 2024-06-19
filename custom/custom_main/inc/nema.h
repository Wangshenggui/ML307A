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

typedef struct
{
    char Buff[200];

    char str[100];
    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;

    uint16_t Year;
    uint8_t Month;
    uint8_t Day;

    uint16_t LatitudeDegrees;
    uint8_t LatitudeMinutes;
    uint8_t LatitudeSeconds;

    uint16_t LongitudeDegrees;
    uint8_t LongitudeMinutes;
    uint8_t LongitudeSeconds;

    float Speed;
    float CourseAngle;

    uint8_t ModeIndicator;
    uint8_t LocationState;

    uint8_t LatitudeDir;
    uint8_t LongitudeDir;
} GPRMC_Structure;
extern GPRMC_Structure GPRMC_Struct;

int extractFirstxxx(const char *data, char *result, size_t result_size,char *gga_start);
void ParseGPGGA(const char *string, int n);
void ParseGPRMC(const char *string, int n);


#endif
