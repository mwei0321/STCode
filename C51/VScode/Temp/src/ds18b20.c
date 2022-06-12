#include "ds18b20.h"

void dsdelay1xus(uint16_t t){
	while(t--){
		_nop_();
		_nop_();
        _nop_();
        _nop_();
	}
}

/***** initialization ds18b20ry *********/
bit DS18B20Init(){
	bit stat = 1;
	
	DS18B20_DQ = 0;
	dsdelay1xus(600);
	DS18B20_DQ = 1;
  	dsdelay1xus(60);
	stat = DS18B20_DQ;
	while(!DS18B20_DQ);
	
	return stat;
}

/******* ds18b20 write cmmand *******/
void DS18B20WritCmd(u8 dat){
	u8 i=0;
	
	for(;i < 8;i++){
		DS18B20_DQ = 0;
		dsdelay1xus(2);
		dat>>=1;
		DS18B20_DQ = CY;
		dsdelay1xus(60);
		DS18B20_DQ = 1;
		dsdelay1xus(10);
	}

}

u8 DS18B20Read(){
	u8 i=0,dat = 0x00;
    
	for(;i < 8;i++){
		DS18B20_DQ = 0;
		dsdelay1xus(2);
		DS18B20_DQ = 1;
		dsdelay1xus(2);
		if(DS18B20_DQ)
			dat |= 1 << i;
		dsdelay1xus(60);
	}
    
	return dat;
}

/***** start ds18b20 temp convert *************/
void DS1B20CT(){
	DS18B20Init();
	dsdelay1xus(5);
	DS18B20WritCmd(0xCC);
	DS18B20WritCmd(0x1F);
	DS18B20Init();
	dsdelay1xus(5);
	DS18B20WritCmd(0xCC);
	DS18B20WritCmd(0x44);
}

/***** start ds18b20 temp Read *************/
void DS1B20ReadCmd(){
	DS18B20Init();
	dsdelay1xus(5);
	DS18B20WritCmd(0xCC);
	DS18B20WritCmd(0xBE);
}

/******** read temp *************/
uint16_t getTemp(){
	u8 LSB,MSB;
	uint16_t temp;
	
	DS1B20CT();
	DS1B20ReadCmd();
	LSB = DS18B20Read();
	MSB = DS18B20Read();
	temp = ((int)MSB << 8) + LSB;
		
	return temp;
} 

