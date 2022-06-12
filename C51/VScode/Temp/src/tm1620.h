#ifndef __TM1620_H
#define __TM1620_H

#include "delay.h"
#include "config.h"

/********定义通信IO*******************/
// STC15F104W
// sbit TM1620_STB = P3^4;
// sbit TM1620_CLK = P3^1;
// sbit TM1620_DIN = P3^3;
// STC15W204S
// sbit TM1620_STB = P5^4;
// sbit TM1620_CLK = P3^1;
// sbit TM1620_DIN = P3^3;
// STC15W204S
sbit TM1620_STB = P3^2;
sbit TM1620_CLK = P5^4;
sbit TM1620_DIN = P5^5;

/******* 函数声明 ***********/

// TM1620初始化
extern void TM1620Init();
// 在指定的地址写入数据
extern void TM1620ShowByAddr(uint8_t addr,uint8_t dat);
// 显示缓存里的数据
extern void TM1620ShowByBuffer();
// 结束传输
extern void TM1620End();
// 测试
extern void TM1620Test();

// 四位数据管
extern uint8_t xdata LedShowBuffer[12];
extern uint8_t code LedCode[18];

#endif