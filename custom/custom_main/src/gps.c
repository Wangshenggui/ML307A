#include"gps.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


 uint8_t GGAString[100];
 uint8_t GGAString1[200];
 uint8_t RMCString[100];
 uint8_t GSVString[100];


double x_PI = 3.14159265358979323846 * 3000.0 / 180.0;
double PI = 3.1415926535897932384626;
double ee = 0.00669342162296594323;
double a = 6378245.0;

//裁剪字符串
static char* substring(char* dst, char* src, int start, int len)
{
    int length = 1000;//×î´ó³¤¶È
    if (start >= length || start < 0)
        return NULL;
    if (len > length)
        len = length - start;
    src += start;
    while (len--)
    {
        *(dst++) = *(src++);
    }
    *(dst++) = '\0';
    return dst;
}

void ParseSLAVE(const char* string, int n)
{
    char result[200];
    unsigned char max_length = 200;
    const char* start = string;

    int i = 0;
    // 定位到第n个逗号
    for (i = 0; i < n; i++) {
        start = strchr(start, ',');
        if (!start) {
            // 如果逗号不够n个，将结果设为空字符串
            result[0] = '\0';
            return;
        }

        // 移动到下一个字符
        start++;
    }

    // 计算逗号后字符串的长度
    const char* end = strchr(start, ',');
    size_t length = end ? (size_t)(end - start) : strlen(start);

    // 截取字符串并复制到结果
    if (length < max_length - 1) {
        strncpy(result, start, length);
        // 手动添加 null 结尾
        result[length] = '\0';
    }
    else {
        // 目标长度不足，截断字符串
        strncpy(result, start, max_length - 1);
        result[max_length - 1] = '\0';
    }

    if (n >= 1 && n <= 8)
    {
        substring(SLAVE_Struct.str, result, 0, strlen(result));
        SLAVE_Struct.Slave_State[n - 1] = atoi(SLAVE_Struct.str);

    }
    else if (n >= 9 && n <= 16)
    {
        substring(SLAVE_Struct.str, result, 0, strlen(result));
        SLAVE_Struct.ReadSpeed1[n - 8 - 1] = atoi(SLAVE_Struct.str);

    }
    else if (n >= 17 && n <= 24)
    {
        substring(SLAVE_Struct.str, result, 0, strlen(result));
        SLAVE_Struct.ReadSpeed2[n - 16 - 1] = atoi(SLAVE_Struct.str);

    }
}

void ParseGPGGA(const char* string, int n)
{
    char result[200];
    unsigned char max_length = 200;
    const char* start = string;

    int xor_sum = 0;//校验值
    int i = 0;
    unsigned char len = 0;//计算字符串长度
    char checksum_str[3];
       
    //计算长度
    while (string[len] != '\0')len++;
    // 计算数据的异或和
    for (i = 1; i < len; i++)
    {
        if (string[i] == '*')
            break;
        xor_sum ^= string[i];
    }
    sprintf(checksum_str, "%02X", xor_sum);
    //u0_printf("xor_sum:%s\n",checksum_str);
    if (strncmp(checksum_str, (const char*)&string[i + 1], 2) == 0) //通过 
    {

     
        // 定位到第n个逗号
        for (int i = 0; i < n; i++)
        {
            start = strchr(start, ',');
            if (!start)
            {
                // 如果逗号不够n个，将结果设为空字符串
                result[0] = '\0';
                return;
            }

            // 移动到下一个字符
            start++;
        }

        // 计算逗号后字符串的长度
        const char* end = strchr(start, ',');
        size_t length = end ? (size_t)(end - start) : strlen(start);

        // 截取字符串并复制到结果
        if (length < max_length - 1)
        {
            strncpy(result, start, length);
            // 手动添加 null 结尾
            result[length] = '\0';
        }
        else
        {
            // 目标长度不足，截断字符串
            strncpy(result, start, max_length - 1);
            result[max_length - 1] = '\0';
        }

        //"$GNGGA,023454.00,2623.01151432,N,10636.51528076,E,1,15,2.5,1211.3547,M,-29.1518,M,,*64"
        switch (n)
        {
        case(1)://时间
            substring(GPGGA_Struct.str, result, 0, 2);
            GPGGA_Struct.Hour = atoi(GPGGA_Struct.str) + 8;
            substring(GPGGA_Struct.str, result, 2, 2);
            GPGGA_Struct.Min = atoi(GPGGA_Struct.str);
            substring(GPGGA_Struct.str, result, 4, 2);
            GPGGA_Struct.Sec = atoi(GPGGA_Struct.str);

            break;
        case(2)://纬度
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Latitude = atof(GPGGA_Struct.str);

            break;
        case(4)://经度
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Longitude = atof(GPGGA_Struct.str);

            break;
        case(6)://定位状态
                substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.PositioningMode = (int)atoi(GPGGA_Struct.str); 
            // GPGGA_Struct.PositioningMode = result[0];
            // sprintf(GPGGA_Struct.PositioningMode1,"%s",result);
            break;
                case(7)://使用的卫星数量
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.sats = (int)atoi(GPGGA_Struct.str);
            break;
        case(8)://水平精度因子
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Radius = atof(GPGGA_Struct.str);
            break;
        case(9)://海拔高度
            substring(GPGGA_Struct.str, result, 0, strlen(result));
            GPGGA_Struct.Height = atof(GPGGA_Struct.str);
            break;



        }
    }
}


void ParseGPRMC(const char* string, int n)
{
    char result[200];
    unsigned char max_length = 200;
    const char* start = string;

    int xor_sum = 0;//校验值
    int i = 0;
    unsigned char len = 0;//计算字符串长度
    char checksum_str[3];

    //计算长度
    while (string[len] != '\0')len++;
    // 计算数据的异或和
    for (i = 1; i < len; i++)
    {
        if (string[i] == '*')
            break;
        xor_sum ^= string[i];
    }
    sprintf(checksum_str, "%02X", xor_sum);
    if (strncmp(checksum_str, (const char*)&string[i + 1], 2) == 0) //通过 
    {



        // 定位到第n个逗号
        for (int i = 0; i < n; i++) {
            start = strchr(start, ',');
            if (!start) {
                // 如果逗号不够n个，将结果设为空字符串
                result[0] = '\0';
                return;
            }

            // 移动到下一个字符
            start++;
        }

        // 计算逗号后字符串的长度
        const char* end = strchr(start, ',');
        size_t length = end ? (size_t)(end - start) : strlen(start);

        // 截取字符串并复制到结果
        if (length < max_length - 1) {
            strncpy(result, start, length);
            // 手动添加 null 结尾
            result[length] = '\0';
        }
        else {
            // 目标长度不足，截断字符串
            strncpy(result, start, max_length - 1);
            result[max_length - 1] = '\0';
        }

        switch (n)
        {
        case(1)://时间
            //解析时间
            substring(GPRMC_Struct.str, result, 0, 2);
            GPRMC_Struct.Hour = (uint8_t)atof(GPRMC_Struct.str) + 8;
            substring(GPRMC_Struct.str, result, 2, 2);
            GPRMC_Struct.Min = (uint8_t)atof(GPRMC_Struct.str);
            substring(GPRMC_Struct.str, result, 4, 2);
            GPRMC_Struct.Sec = (uint8_t)atof(GPRMC_Struct.str);
            break;
        case(2)://定位状态
            GPRMC_Struct.LocationState = result[0];
            break;
        case(3)://纬度
            //解析纬度
            //度
            substring(GPRMC_Struct.str, result, 0, 2);
            GPRMC_Struct.LatitudeDegrees = (uint8_t)atoi(GPRMC_Struct.str);
            //分
            substring(GPRMC_Struct.str, result, 2, 2);
            GPRMC_Struct.LatitudeMinutes = (uint8_t)atoi(GPRMC_Struct.str);
            /*GPS*/
            if (strlen(result) == 11)
            {
                //秒
                substring(GPRMC_Struct.str, result, 5, 6);
                GPRMC_Struct.LatitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.000001 * 60 + 0.5);
            }
            else if (strlen(result) == 9)
            {
                //秒
                substring(GPRMC_Struct.str, result, 5, 4);
                GPRMC_Struct.LatitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.0001 * 60 + 0.5);
            }
            /*RTK*/
            else if (strlen(result) == 13)
            {
                //秒
                substring(GPRMC_Struct.str, result, 5, 8);
                GPRMC_Struct.LatitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.00000001 * 60 + 0.5);
            }



            break;
        case(4)://N/S
            GPRMC_Struct.LatitudeDir = result[0];
            break;
        case(5)://经度
            //解析经度
            //度
            substring(GPRMC_Struct.str, result, 0, 3);
            GPRMC_Struct.LongitudeDegrees = (uint8_t)atoi(GPRMC_Struct.str);
            //分
            substring(GPRMC_Struct.str, result, 3, 2);
            GPRMC_Struct.LongitudeMinutes = (uint8_t)atoi(GPRMC_Struct.str);

            /*GPS*/
            if (strlen(result) == 11)
            {
                //秒
                substring(GPRMC_Struct.str, result, 6, 6);
                GPRMC_Struct.LongitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.000001 * 60 + 0.5);
            }
            else if (strlen(result) == 9)
            {
                //秒
                substring(GPRMC_Struct.str, result, 6, 4);
                GPRMC_Struct.LongitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.0001 * 60 + 0.5);
            }
            /*RTK*/
            else if (strlen(result) == 14)
            {
                //秒
                substring(GPRMC_Struct.str, result, 6, 8);
                GPRMC_Struct.LongitudeSeconds = (uint8_t)(atof(GPRMC_Struct.str) * 0.00000001 * 60 + 0.5);
            }
            break;
        case(6)://E/W
            GPRMC_Struct.LongitudeDir = result[0];
            break;
        case(7)://地面速度
        //              
            GPRMC_Struct.Speed = (float)atof(result)*1.852;
            break;
        case(8)://航向角
            GPRMC_Struct.CourseAngle = (float)atof(result);
            break;
        case(9)://日期
            //解析日期
            substring(GPRMC_Struct.str, result, 0, 2);
            GPRMC_Struct.Day = (uint8_t)atof(GPRMC_Struct.str);
            substring(GPRMC_Struct.str, result, 2, 2);
            GPRMC_Struct.Month = (uint8_t)atof(GPRMC_Struct.str);
            substring(GPRMC_Struct.str, result, 4, 2);
            GPRMC_Struct.Year = (uint8_t)atof(GPRMC_Struct.str) + 2000;

           // u1_printf("日期：%d-%d-%d\r\n", GPRMC_Struct.Year, GPRMC_Struct.Month, GPRMC_Struct.Day);
            break;
        case(12)://定位模式
            GPRMC_Struct.ModeIndicator = result[0];
            break;
        }
    }
}






#include <stdbool.h>
bool outof_China(double lon, double lat)
{
    return(lon < 72.004 || lon>137.8374 || lat < 0.8293 || lat >55.8271 || false);
}

double translate_lon(double lon, double lat)
{
    double ret = 300.0 + lon + 2.0 * lat + 0.1 * lon * lon + 0.1 * lon * lat + 0.1 * sqrt(fabs(lon));
    ret += (20.0 * sin(6.0 * lon * PI) + 20.0 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lon * PI) + 40.0 * sin(lon / 3.0 * PI)) * 2.0 / 3.0;
    ret += (150 * sin(lon / 12.0 * PI) + 300.0 * sin(lon / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
}
double translate_lat(double lon, double lat)
{
    double ret = -100 + 2.0 * lon + 3.0 * lat + 0.2 * lat * lat + 0.1 * lon * lat + 0.2 * sqrt((fabs(lon)));
    ret += (20.0 * sin(6.0 * lon * PI) + 20 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lat * PI) + 40.0 * sin(lat / 3.0 * PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(lat / 12.0 * PI) + 320.0 * sin(lat / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
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


// 将度分秒格式的经纬度转换为十进制度数
double dms_to_degrees(double dms)  //2623.0546
{             //degrees =26
    double degrees = (int)(dms / 100);  // 取整数部分作为度数

                    //2623.0546-26*100=23.0546
    double minutes = dms - degrees * 100; // 取小数部分作为分数


    double decimal_degrees = degrees + minutes / 60.0; // 转换为十进制度数
    return decimal_degrees;
}

long long format_time_to_timestamp(uint16_t Y, uint16_t M, uint16_t D, uint16_t h, uint16_t m, uint16_t s)
{
    struct tm timeinfo = { 0 };
    time_t timestamp;

    timeinfo.tm_year = Y - 1900;
    timeinfo.tm_mon = M - 1;
    timeinfo.tm_mday = D;
    timeinfo.tm_hour = h - 8;
    timeinfo.tm_min = m;
    timeinfo.tm_sec = s;
    // 转换为时间戳
    timestamp = mktime(&timeinfo);
    
    return timestamp;
}





void separateString(char* inputString, const char* delimiter, char* info1, char* info2, char* info3) {
    char* token = strtok(inputString, delimiter);

    // 使用 strncpy 将每个信息复制到相应的指针指向的缓冲区
    if (token != NULL) {
        strncpy(info1, token, strlen(token));
        info1[strlen(token)] = '\0';
    }
    else
    {
        info1[0] = '\0';
    }

    token = strtok(NULL, delimiter);

    if (token != NULL) {
        strncpy(info2, token, strlen(token));
        info2[strlen(token)] = '\0';
    }
    else
    {
        info2[0] = '\0';
    }

    token = strtok(NULL, delimiter);

    if (token != NULL) {
        strncpy(info3, token, strlen(token));
        info3[strlen(token)] = '\0';
    }
    else
    {
        info3[0] = '\0';
    }
}





void processInfo(uint8_t* info1, uint8_t* info2, uint8_t* info3, uint8_t* rmcBuffer, uint8_t* ggaBuffer, uint8_t* gsvBuffer, size_t bufferLength)
{
    // Initialize buffers with null characters
    memset(rmcBuffer, 0, bufferLength);
    memset(ggaBuffer, 0, bufferLength);
    memset(gsvBuffer, 0, bufferLength);

    // Process info1
    if (info1[3] == 'R' && info1[4] == 'M' && info1[5] == 'C')
    {
        memcpy(rmcBuffer, info1, bufferLength - 1);
    }
    else if (info1[3] == 'G' && info1[4] == 'G' && info1[5] == 'A')
    {
        memcpy(ggaBuffer, info1, bufferLength - 1);
    }
    else if (info1[3] == 'G' && info1[4] == 'S' && info1[5] == 'V')
    {
        memcpy(gsvBuffer, info1, bufferLength - 1);
    }

    // Process info2
    if (info2[3] == 'R' && info2[4] == 'M' && info2[5] == 'C')
    {
        memcpy(rmcBuffer, info2, bufferLength - 1);
    }
    else if (info2[3] == 'G' && info2[4] == 'G' && info2[5] == 'A')
    {
        memcpy(ggaBuffer, info2, bufferLength - 1);
    }
    else if (info2[3] == 'G' && info2[4] == 'S' && info2[5] == 'V')
    {
        memcpy(gsvBuffer, info2, bufferLength - 1);
    }

    // Process info3
    if (info3[3] == 'R' && info3[4] == 'M' && info3[5] == 'C')
    {
        memcpy(rmcBuffer, info3, bufferLength - 1);
    }
    else if (info3[3] == 'G' && info3[4] == 'G' && info3[5] == 'A')
    {
        memcpy(ggaBuffer, info3, bufferLength - 1);
    }
    else if (info3[3] == 'G' && info3[4] == 'S' && info3[5] == 'V')
    {
        memcpy(gsvBuffer, info3, bufferLength - 1);
    }

}




