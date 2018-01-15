#include "STC15Wxx.h"

typedef unsigned char u8;
typedef unsigned int u16;

u16 cnt;
bit st;

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;
}

void main(){
    EA = 1;
    
    P5M0 = 0xFF;
    
    Timer0Init();
    
    while(1);

}

void T0() interrupt 1 {
    cnt++;
    if(cnt > 6000){
        cnt = 0;
    }
    
    if(cnt > 3000){
        P55 = 0;
    }else{
        P55 = 1;
    }
}
