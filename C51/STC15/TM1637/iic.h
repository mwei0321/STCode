#ifndef __IIC_H
#define __IIC_H
#include "INTRINS.h"
#include "STC15Wxx.h"

sbit SLK = P1^1;
sbit DIO = P1^0;

//iic 起始信号
void I2CStart();
//iic 结束信号
void I2CEnd();
//iic 应答信号
bit I2CAck(void);
//iic 写入一个字节
void I2CWriteByte(unsigned char dat);

#endif 
