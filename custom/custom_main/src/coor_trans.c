#include "coor_trans.h"
#include <stdarg.h>
#include <stdio.h>
#include "nema.h"
#include "string.h"
#include <mcu_uart.h>

POSITION bd_pos;
POSITION gcj_pos;

// 将度分秒格式的经纬度转换为十进制度数
double dms_to_degrees(double dms)      // 2623.0546
{                                      // degrees =26
    double degrees = (int)(dms / 100); // 取整数部分作为度数

    // 2623.0546-26*100=23.0546
    double minutes = dms - degrees * 100; // 取小数部分作为分数

    double decimal_degrees = degrees + minutes / 60.0; // 转换为十进制度数
    return decimal_degrees;
}

POSITION wgs84togcj02(double wgs_lon, double wgs_lat)
{
    if (outof_China(wgs_lon, wgs_lat))
    {
        gcj_pos.longitude = wgs_lon;
        gcj_pos.latitude = wgs_lat;
        return gcj_pos;
    }
    else
    {
        double dlat = translate_lat(wgs_lon - 105.0, wgs_lat - 35.0);
        double dlon = translate_lon(wgs_lon - 105.0, wgs_lat - 35.0);
        double radlat = wgs_lat / 180.0 * PI;
        double magic = sin(radlat);
        magic = 1 - ee * magic * magic;
        double squrtmagic = sqrt(magic);
        dlon = (dlon * 180.0) / (a / squrtmagic * cos(radlat) * PI);
        dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * squrtmagic) * PI);
        gcj_pos.longitude = wgs_lon + dlon;
        gcj_pos.latitude = wgs_lat + dlat;
        return gcj_pos;
    }
}


POSITION gcj02tobd09(double gcj_lon, double gcj_lat)
{
    double z = sqrt(gcj_lon * gcj_lon + gcj_lat * gcj_lat) + 0.00002 * sin(gcj_lat * x_PI);
    double theta = atan2(gcj_lat, gcj_lon) + 0.000003 * cos(gcj_lon * x_PI);
    bd_pos.longitude = z * cos(theta) + 0.0065;
    bd_pos.latitude = z * sin(theta) + 0.006;
    return bd_pos;
}


bool outof_China(double lon, double lat)
{
    return(lon < 72.004 || lon>137.8374 || lat < 0.8293 || lat >55.8271 || false);
}

double translate_lat(double lon, double lat)
{
    double ret = -100 + 2.0 * lon + 3.0 * lat + 0.2 * lat * lat + 0.1 * lon * lat + 0.2 * sqrt((fabs(lon)));
    ret += (20.0 * sin(6.0 * lon * PI) + 20 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lat * PI) + 40.0 * sin(lat / 3.0 * PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(lat / 12.0 * PI) + 320.0 * sin(lat / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
}

double translate_lon(double lon, double lat)
{
    double ret = 300.0 + lon + 2.0 * lat + 0.1 * lon * lon + 0.1 * lon * lat + 0.1 * sqrt(fabs(lon));
    ret += (20.0 * sin(6.0 * lon * PI) + 20.0 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lon * PI) + 40.0 * sin(lon / 3.0 * PI)) * 2.0 / 3.0;
    ret += (150 * sin(lon / 12.0 * PI) + 300.0 * sin(lon / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
}
