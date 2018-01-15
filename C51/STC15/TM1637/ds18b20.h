#ifndef __DS18B20_H
#define __DS18B20_H
#include "STC15Wxx.h"
#include "INTRINS.h"

/********reset var type*********/
#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

/*****set pin********/
sbit DS18B20_DQ = P1^2;

u16 getTemp();

#endif