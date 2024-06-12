#ifndef __TCP_H__
#define __TCP_H__
#include "custom_main.h"
//#include <cstdint.h>

#include "uart.h"
#include "netdb.h"
#include "cm_uart.h"
#include "stdio.h"

#include "gps.h"
//////////////////////////////////////////////////////////////////////////////
#include "cm_pm.h"


    

    extern  char info1[100];
    extern  char info2[100];
    extern  char info3[100];
    
extern char Corsipstr[20];
extern  int CORSport;
extern  char CORScount[20];
extern  char CORSaccpass[100];


void Uploading(char GGAString2[100], char RMCString2[100]);
void print_ipv4_address(const char *hostname);
void print_ipv4_address1(const char *hostname);
void UpBaidu(void);
void GetCors(void);
////////////////////////////////////////////////////////////////////////////////
void Waitnet();
void Waitnet1();
void swit(void);






#endif