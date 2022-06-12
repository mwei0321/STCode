#include "tm1620.h"

// 定义数据段位显示,共阴极数码管的0 1 2 3 4 5 6 7 8 9 A b C d E F
uint8_t code LedCode[18] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x00,0x70};
uint8_t xdata LedShowBuffer[12] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// @12MHZ
void TMDelayUs(uint16_t us){
  while (--us)
  {
    _nop_();
    _nop_();
    _nop_();
    _nop_();
  }
}

/*********************************
 * 函数: TM1620WriteData(uint8_t dat) 芯片写一位数据
 * 参数: uint8_t dat 一个字节
 * 返回: void
 * 日期: 2020-04-05
 * 作者: MaWei (http://mawei.live)
 * 备注: none
 * ******************************/
void TM1620WriteData(uint8_t dat){
  unsigned char i;

  TM1620_STB = 0; // 给有效的片选信号（片选信号是低电平有效）
	TMDelayUs(10);
  // 开始传送8位数据，每循环一次传送一位数据,低位在前
  for(i=0;i<8;i++){
    dat >>= 1;
		TM1620_DIN = CY;
		TM1620_CLK = 0;
    TMDelayUs(5);
    TM1620_CLK = 1;
    TMDelayUs(5);
  }
}

/*********************************
 * 函数: TM1620WriteCmd(uint8_t cmd) 芯片写一个指令
 * 参数: uint8_t cmd 一个字节
 * 返回: void
 * 日期: 2020-04-05
 * 作者: MaWei (http://mawei.live)
 * 备注: none
 * ******************************/
void TM1620WriteCmd(uint8_t cmd){
  // 调用写数据
  TM1620WriteData(cmd);
  // 延时
  TMDelayUs(5);
  // 拉高片选,完成写命令
  TM1620_STB = 1; 
}

/*********************************
 * 函数: TM1620ShowByBuffer(uint8_t cmd) 显示缓存里的数据
 * 参数: none
 * 返回: void
 * 日期: 2020-04-05
 * 作者: MaWei (http://mawei.live)
 * 备注: none
 * ******************************/
void TM1620ShowByBuffer(){
	uint8_t i;
  // 写入地址
  TM1620WriteCmd(0xC0);
  // 写入数据
  for (i = 0; i < 12; i++){
    TM1620WriteData(0x00);
  }
  // 结束写入
  TMDelayUs(5);
  TM1620End();
}

/*********************************
 * 函数: TM1620ShowByAddr(uint8_t cmd) 在指定地址里写入数据
 * 参数: uint8_t cmd 一个字节
 * 返回: void
 * 日期: 2020-04-05
 * 作者: MaWei (http://mawei.live)
 * 备注: none
 * ******************************/
void TM1620ShowByAddr(uint8_t addr,uint8_t dat){
  // 写入地址
  TM1620WriteData(addr);
  // 写入数据
  TM1620WriteData(dat);
  // 结束写入
  TMDelayUs(200);
  TM1620End();
}

/*********************************
 * 函数: TM1620End(uint8_t cmd) 在指定地址里写入数据
 * 参数: uint8_t cmd 一个字节
 * 返回: void
 * 日期: 2020-04-05
 * 作者: MaWei (http://mawei.live)
 * 备注: none
 * ******************************/
void TM1620End(){
  TM1620_STB = 1;
  TM1620_CLK = 1;
  TM1620_DIN = 1;
}

/*********************************
 * 函数: TM1620Init() 芯片写一个指令
 * 参数: uint8_t cmd 一个字节
 * 返回: void
 * 日期: 2020-04-05
 * 作者: MaWei (http://mawei.live)
 * 备注: none
 * ******************************/
void TM1620Init(){
  // 初始化IO
  TM1620_STB = 1;
  TM1620_CLK = 1;
  TM1620_DIN = 1;
	delay_ms(10);
//	TM1620WriteCmd(0x00);
  // 初始化显示模式,6位8段(M6BIT8:0x02),5位9段(M5BIT9:0x01),4位10段(M4BIT10:0x00)
  TM1620WriteCmd(0x02);

  // 初始化数据命令模式, AddAuto:0x40(地址自动增加模式), AddFixed:0x44(固定地址模式)
  TM1620WriteCmd(0x44);

  // 写入默认数据
  // TM1620ShowByBuffer();
	// TM1620ShowByAddr(0x0C,0x3F);

  // 打开显示
  TM1620WriteCmd(0x88);
  // 结束写入
  TM1620End();
}


void TM1620ShowFour() {
  TM1620ShowByAddr(0xc0,0x3F);
  TM1620ShowByAddr(0xc2,0x06);
  TM1620ShowByAddr(0xc4,0x5B);
  TM1620ShowByAddr(0xc6,0x4F);
}

void TM1620Test() {
  // uint8_t i = 0;
  // TM1620WriteCmd(0x02);
  // TM1620WriteCmd(0x44);
  TM1620ShowByAddr(0xc0,0x3F);
  TM1620ShowByAddr(0xc2,0x06|0x80);
  TM1620ShowByAddr(0xc4,0x5B);
  TM1620ShowByAddr(0xc6,0x4F);
  // TM1620WriteCmd(0x8c);
  // 0x3F, 0x06, 0x5B, 0x4F,
}