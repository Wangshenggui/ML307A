#ifndef _COOR_TRANS_H
#define _COOR_TRANS_H

#include "cm_uart.h"
#include "cm_os.h"
#include "cm_iomux.h"
#include "custom_main.h"
#include <stdbool.h>
#include "math.h"


#define PI 3.1415926535897932384626
#define x_PI 3.1415926535897932384626 * 3000.0 / 180.0
#define ee 0.00669342162296594323
#define a 6378245.0

typedef struct _POSITION
{
    double longitude;
    double latitude;
} POSITION;

double dms_to_degrees(double dms);
POSITION wgs84togcj02(double wgs_lon, double wgs_lat);
POSITION gcj02tobd09(double gcj_lon, double gcj_lat);
bool outof_China(double lon, double lat);
double translate_lat(double lon, double lat);
double translate_lon(double lon, double lat);



#endif
