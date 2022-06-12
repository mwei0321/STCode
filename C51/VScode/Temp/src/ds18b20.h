#ifndef __DS18B20_H
#define __DS18B20_H

#include "config.h"

sbit DS18B20_DQ = P3^3;

u16 getTemp();

#endif