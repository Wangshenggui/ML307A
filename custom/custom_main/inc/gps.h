#ifndef __GPS_H__
#define __GPS_H__
#include "custom_main.h"

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned int
#endif

extern uint8_t GGAString[100];
extern uint8_t GGAString1[200];
extern uint8_t RMCString[100];
extern uint8_t GSVString[100];

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
GPRMC_Structure GPRMC_Struct;

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
GPGGA_Structure GPGGA_Struct;

typedef struct
{
    char str[100];                      //´æ´¢·Ö¸îµÄ×Ö·û´®

    unsigned int Slave_State[8];
    unsigned int ReadSpeed1[8];
    unsigned int ReadSpeed2[8];
}SLAVE_Structure;
SLAVE_Structure SLAVE_Struct;

// char tempString[200];

// char SendStr[1000] = { 0 };

typedef struct _POSITION
{
    double longitude;
    double latitude;
} POSITION;

POSITION wgs84togcj02(double wgs_lon, double wgs_lat);
POSITION gcj02tobd09(double gcj_lon, double gcj_lat);
double dms_to_degrees(double dms);
long long format_time_to_timestamp(uint16_t Y, uint16_t M, uint16_t D, uint16_t h, uint16_t m, uint16_t s);

extern double x_PI;
extern double PI;
extern double ee;
extern double a;

POSITION bd_pos;
POSITION gcj_pos;

void ParseSLAVE(const char* string, int n);
void ParseGPGGA(const char *string, int n);
void ParseGPRMC(const char *string, int n);
void separateString(char *inputString, const char *delimiter, char *info1, char *info2, char *info3);
void processInfo(uint8_t *info1, uint8_t *info2, uint8_t *info3, uint8_t *rmcBuffer, uint8_t *ggaBuffer, uint8_t *gsvBuffer, size_t bufferLength);

#endif
