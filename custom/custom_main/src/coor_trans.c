#include "coor_trans.h"
#include <stdarg.h>
#include <stdio.h>
#include "nema.h"
#include "string.h"
#include <mcu_uart.h>

// 定义位置坐标，用于坐标转换
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

// wgs84转换gcj02
POSITION wgs84togcj02(double wgs_lon, double wgs_lat)
{
    // 判断点是否在中国境内
    if (outof_China(wgs_lon, wgs_lat))
    {
        gcj_pos.longitude = wgs_lon;
        gcj_pos.latitude = wgs_lat;

        return gcj_pos;
    }
    else
    {
        // 如果点在中国境内，则进行WGS-84到GCJ-02的转换
        double dlat = translate_lat(wgs_lon - 105.0, wgs_lat - 35.0);
        double dlon = translate_lon(wgs_lon - 105.0, wgs_lat - 35.0);
        // 将纬度转换为弧度
        double radlat = wgs_lat / 180.0 * PI;
        // 计算magic值，用于椭球参数调整
        double magic = sin(radlat);
        magic = 1 - ee * magic * magic;
        double squrtmagic = sqrt(magic);
        // 计算经度和纬度的偏移量
        dlon = (dlon * 180.0) / (a / squrtmagic * cos(radlat) * PI);
        dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * squrtmagic) * PI);
        // 应用偏移量得到新的GCJ-02坐标
        gcj_pos.longitude = wgs_lon + dlon;
        gcj_pos.latitude = wgs_lat + dlat;

        return gcj_pos;
    }
}

// gcj02转换bd09
POSITION gcj02tobd09(double gcj_lon, double gcj_lat)
{
    // 计算z值，z是将经纬度转换为极坐标后加上一个小的扰动值
    double z = sqrt(gcj_lon * gcj_lon + gcj_lat * gcj_lat) + 0.00002 * sin(gcj_lat * x_PI);
    // 计算theta值，theta是将极坐标的角度值加上一个小的扰动值
    double theta = atan2(gcj_lat, gcj_lon) + 0.000003 * cos(gcj_lon * x_PI);
    // 计算新的经度值，并加上百度坐标系的偏移量
    bd_pos.longitude = z * cos(theta) + 0.0065;
    // 计算新的纬度值，并加上百度坐标系的偏移量
    bd_pos.latitude = z * sin(theta) + 0.006;
    // 返回转换后的百度坐标
    return bd_pos;
}

// 判断坐标是否在中国境内(矩形)
bool outof_China(double lon, double lat)
{
    return (lon < 72.004 || lon > 137.8374 || lat < 0.8293 || lat > 55.8271 || false);
}

// 计算纬度的转换偏移量
double translate_lat(double lon, double lat)
{
    // 初始化返回值
    double ret = -100 + 2.0 * lon + 3.0 * lat + 0.2 * lat * lat + 0.1 * lon * lat + 0.2 * sqrt((fabs(lon)));
    // 第一次调整
    ret += (20.0 * sin(6.0 * lon * PI) + 20 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    // 第二次调整
    ret += (20.0 * sin(lat * PI) + 40.0 * sin(lat / 3.0 * PI)) * 2.0 / 3.0;
    // 第三次调整
    ret += (160.0 * sin(lat / 12.0 * PI) + 320.0 * sin(lat / 30.0 * PI)) * 2.0 / 3.0;
    // 返回计算得到的纬度转换偏移量
    return ret;
}

// 计算经度的转换偏移量
double translate_lon(double lon, double lat)
{
    // 初始化返回值
    double ret = 300.0 + lon + 2.0 * lat + 0.1 * lon * lon + 0.1 * lon * lat + 0.1 * sqrt(fabs(lon));
    // 第一次调整
    ret += (20.0 * sin(6.0 * lon * PI) + 20.0 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    // 第二次调整
    ret += (20.0 * sin(lon * PI) + 40.0 * sin(lon / 3.0 * PI)) * 2.0 / 3.0;
    // 第三次调整
    ret += (150 * sin(lon / 12.0 * PI) + 300.0 * sin(lon / 30.0 * PI)) * 2.0 / 3.0;
    // 返回计算得到的经度转换偏移量
    return ret;
}
