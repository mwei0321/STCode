#include "STC15Wxx.h"
#include "INTRINS.h"

sbit Trig = P1^1;
sbit Echo = P1^0;

bit busy;
unsigned char IntCnt=0;
unsigned int distance=0;

//延时1MS
void DelayXms(int x){		//@12.000MHz
	unsigned char i, j;
	
	while(x--){
		i = 12;
		j = 169;
		do{
			while (--+j);
		} while (--i);
	}
}

//延时1us
void Delay1us(){		//@12.000MHz
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

//4800bps@12.000MHz
void UartInit(void){
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

//100微秒@12.000MHz
void Timer0Init(void){
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x00;		//设置定时初值
	TF0 = 0;		//清除TF0标志
//	EX0 = 1; //开启外部中断
//	IT0 = 1; //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
	ET0 = 1; //开启定时器中断
}


/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char dat){
	//写数据到UART数据寄存器
	SBUF = dat; 
	//TI是发送成功标志，等待前面的数据发送完成
	while(!TI);
	TI = 0;
}
//发送字符
void SendString(char *s){
	while (*s){                  //检测字符串结束标志
		SendData(*s++);         //发送当前字符
	}
}

//获取距离
void getDistance(){
	unsigned char i,time,d20us = 55;
	unsigned char str[] = "D:-.--M";
	
	//初始化定时器值
	TH0 = TL0 = 0;
	//拉高超声波模块触发IO
	Trig = 1;
	//延时20us
	while(d20us--);
	//拉低超声波模块触发IO
	Trig = 0;
	//等待超声波模块输出IO拉高
	while(!Echo);
	//开启定时器计时
	TR0 = 1;
	//等待超声波模块输出IO拉低
	while(Echo);
	//关闭定时器计时
 	TR0 = 0;
	//距离 uS/58=厘米 最大400cm， 最大时间 = 400cm*58=23200, 定时器 65536-23200/(1/12)=278400
	 	//判断是否超出模块最大测距 4m
 	if(IntCnt < 5){
	 	//计算出时间
		time = (TH0 * 256 + TL0 + IntCnt * 65536) * (1 / 12.000) + 0.5;
		//计算出距离 340m/s 或 us/58cm
		distance = time / 58 + 0.5;
	}else {
		distance = 0;
	}
	str[2] = (distance % 1000 / 100) + '0';
	str[4] = (distance % 100 / 10) + '0';
	str[5] = (distance % 10) + '0';
	//从串口发出，也可以用显示器显示，自己接显示驱动就可以了
	for(i=0;i<7;i++){
//			SendData(str[i]);
	}
//	SendData('\n');
	SendData(IntCnt);
	SendData(0xFF);
//	SendData('\n');
	SendData(TH0);
	SendData(TL0);
	SendData(0xFF);
	//复位定时器
	IntCnt = 0;
	TH0 = TL0 = 0;
}

//中断
void ET0_INT0(void) interrupt 1{
	IntCnt++;
}

void main(){
	
	//开启总中断
	EA = 1;
	//初始化串口
	UartInit();
	//初始化定时器
	Timer0Init();
	
	DelayXms(1000);
	SendString("MaWei");
	
	
	while(1){
		getDistance();
		DelayXms(2000);
	}
	
}