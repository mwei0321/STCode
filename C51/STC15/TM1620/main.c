#include <STC15Wxx.h>
#include <INTRINS.H>

sbit CLK=P3^2;
sbit STB=P5^4;
sbit DIN=P3^3;

//共阴极数码管的0 1 2 3 4 5 6 7 8 9 A b C d E F
unsigned char code LedCode[17]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x00};

void delayXus(int x);



void delayXms(int x){
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

void delayXus(int x){
	while(x--){
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

void WriteChar(unsigned char dat){
	unsigned char i;
	STB = 0;
	delayXus(3);
	for(i=0;i<8;i++){
		CLK = 0;
		DIN = (bit)dat&0x01;
		delayXus(5);
		CLK = 1;
		dat >>= 1;
		delayXus(5);
	}
}

void WriteCmd(unsigned char cmd){
	STB = 0;
	WriteChar(cmd);
	STB = 1;
	delayXus(3);
}

void display(unsigned char dat,unsigned char i){
	WriteChar(0xC0&i);
	WriteChar(LedCode[dat]);
}


void InitTM1620(){
	
	WriteCmd(0x02); //设置显示模式为6位8段
//	WriteCmd(0x44);//固定地址
	WriteCmd(0x40);//设置数据命令自增地址
}

void main(){
	unsigned char i;

	
	while(1){
		CLK = 1;
		DIN = 1;
		STB = 1;
		delayXms(2000);
		
		WriteCmd(0x02);
		WriteCmd(0x40);
		WriteChar(0xC0);
		for(i=0;i<12;i++){
			WriteChar(LedCode[i]);
		}
		STB = 1;
		WriteChar(0x8c);
		STB = 1;
		delayXms(2000);
		
	}
}
