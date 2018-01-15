#include "ds18b20.h"
#include "tm1637.h"


/********************定义数据*************************/
//unsigned char code CODE[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e}; //共阳显示数据0-F

//共阴极数码管的0 1 2 3 4 5 6 7 8 9 A b C d E F
unsigned char code LedCode[17]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x00};

unsigned int temp = 0,cnttime=0;
unsigned char TempVal[] = {0x00,0x00,0x00,0x00};
unsigned char flashIndex = 1;
bit isRefresh = 1;

void DelayXms(unsigned int x);
void Timer0Init();
void UartInit();
void SendData(u8 data_buf);
void show();

void main(){

    EA = 1;
    
    DelayXms(500);
    Timer0Init();
//    UartInit();
    DelayXms(50);
    TMxxCmd(0x44);
    DelayXms(50);
    
//    TMxxShow(0xC0,CODE[3]);
//    TMxxShow(0xC1,CODE[8]);
//    TMxxShow(0xC2,CODE[5]);
//    TMxxShow(0xC3,CODE[0]);
//    TMxxCmd(0x89);
    
    while(1){
        if(isRefresh){
            isRefresh = 0;
            show();
        }
    }
}

void show(){
    u8 i=0,tmp;
    
    temp = getTemp();
    temp = temp * 0.0625 * 100 + 0.5;
    TempVal[0] = (temp % 10000 / 1000);
    TempVal[1] = (temp % 1000 / 100);
    TempVal[2] = (temp % 100 / 10);
    TempVal[3] = temp%10;
    
//    TempVal[0] = 1;
//    TempVal[1] = 2;
//    TempVal[2] = 3;
//    TempVal[3] = 0x0C;
    
    TMxxShow(0xC0,LedCode[0xff]);
    for(i=0;i<5;i++){
        tmp = LedCode[TempVal[i]];
        if(i==1) tmp |= 0x80;
        TMxxShow((0xC0+i),tmp);
    }
    TMxxCmd(0x89);
}

void nowLedCode(u8 type){
//    u8 sec,miute,hour,day,month,year;
    
    switch(type){
        case 0: //显示时间时、分
//            miute = DS3231_Read_Single_Byte(0x01);
//            hour = DS3231_Read_Single_Byte(0x02);
//            TempVal[0] = hour >> 4;
//            TempVal[1] = hour&0x0F;
//            TempVal[2] = miute >> 4;
//            TempVal[3] = miute&0x0F;
//            if((hour > 0) && (hour < 7)){
//                wirelessRoute = 0;
//            }else
//                wirelessRoute = 1;
            break;
        case 1: //显示温度
            temp = getTemp();
            temp = temp * 0.0625 * 100 + 0.5;
            TempVal[0] = (temp % 10000 / 1000);
            TempVal[1] = (temp % 1000 / 100);
            TempVal[2] = (temp % 100 / 10);
            TempVal[3] = 0x0C;
            break;
    }
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;   //使能定时器0中断
}

void T0() interrupt 1{
    cnttime++;
    if(cnttime > 999){
        cnttime = 0;
        isRefresh = 1;
    }
}

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

//发送一个字符
void SendData(u8 data_buf){
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}

void DelayXms(unsigned int x)		//@12.000MHz
{
	unsigned char i, j;
    
    while(x--){
        i = 12;
        j = 169;
        do
        {
            while (--j);
        } while (--i);
    }
}
