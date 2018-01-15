#ifndef __TM1637_H
#define __TM1637_H
#include "iic.h"

//给TM1637写入显示字符
void TMxxShow(unsigned char addr,unsigned char dat);
//给TM1637写入命令
void TMxxCmd(unsigned char cmd);

#endif
