#include "tm1637.h"

//给TM1637写入命令
void TMxxCmd(unsigned char cmd){
    I2CStart();
    I2CWriteByte(cmd);
    I2CEnd();
}

//给TM1637写入显示字符
void TMxxShow(unsigned char addr,unsigned char dat){
    I2CStart();
    I2CWriteByte(addr);
    I2CAck();
    I2CWriteByte(dat);
    I2CAck();
    I2CEnd();
}
