#include "iic.h"

//iic 延时
void I2CDelayXus(unsigned int x)		//@12.000MHz
{
    while(x--){
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

//iic 起始信号
void I2CStart(){
	SLK = 1;
    DIO = 1;
    I2CDelayXus(5);
    DIO = 0;
	SLK = 0;
}

//iic 结束信号
void I2CEnd(){
    SLK = 0;
    I2CDelayXus(5);
    DIO = 0;
    I2CDelayXus(5);
    SLK = 1;
    I2CDelayXus(5);
    DIO = 1;    
}

//iic 应答信号
bit I2CAck(){
    bit ack;
    
    SLK = 0;
    I2CDelayXus(5);
    ack = DIO;
    SLK = 1;
    I2CDelayXus(5);
    SLK = 0;
    return ack;
}

//iic 写入一个字节
void I2CWriteByte(unsigned char dat){
    unsigned char i;
    
    for(i=0;i<8;i++){
        SLK = 0;
        dat >>= 1;
        DIO = CY;
        I2CDelayXus(5);
        SLK = 1;
        I2CDelayXus(5);
    }
}

