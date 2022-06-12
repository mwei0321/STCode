#include "TUBE_KEY.h"

#include "LED.h"
#include "delay.h"



#define SCL_Set (GPIOB->BSRR = 1<<5)
#define SDA_Set (GPIOB->BSRR = 1<<6)
#define SCL_Reset (GPIOB->BSRR = 1<<(16+5))
#define SDA_Reset (GPIOB->BSRR = 1<<(16+6))

#define SCL_Read (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5))
#define SDA_Read (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))

#define IIC_uS 10

u8 KEY_EXTI= 0;


static void TM1650_Init(void);
static void IIC_Init(void);
static void IIC_Start(void);
static void IIC_Stop(void);
static void IIC_Ack(void);
static void IIC_NoAck(void);
static u8 IIC_Wait_Ack(void);
static void IIC_Wr_Byte(u8 Data);
static u8 IIC_Rd_Byte(void);
static void TM1650_Wr_RAM(u8 Address, u8 Data);
static u8 KEYtm_Scan(void);






static void EXTI_KEY_Init(void);




/*
*函数功能：数码管按键初始化
*提    示：tm1650挂载4个按键，外部中断挂载两个按键
*输入参数：无
*输出参数：无
*返回值  ：无
*/
void Tube_Key_Init(void)
{
    /*按键外部中断初始化*/
    EXTI_KEY_Init();

    /*TM1650初始化*/
    TM1650_Init();
}

/*
*函数功能：TM1650初始化
*提    示：tm1650挂载4个按键，外部中断挂载两个按键
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void TM1650_Init(void)
{
    IIC_Init();

    Delay_mS(50);

    TM1650_Wr_RAM(0x48,0x00);//关闭显示
}

/*
*函数功能：数码管显示数字
*提    示：仅4位支持整数
*输入参数：u16 Num:显示的数字
*输出参数：无
*返回值  ：无
*/
const u8 NUM[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//数码管显示0~9对应的值
void Tube_DisNum(u16 Num)
{
    u8 Temp[4];

    /*数据分离*/
    Temp[0] = Num%10000/1000;
    Temp[1] = Num%1000/100;
    Temp[2] = Num%100/10;
    Temp[3] = Num%10;

    /*显示*/
    TM1650_Wr_RAM(0x68, NUM[Temp[0]]);
    TM1650_Wr_RAM(0x6A, NUM[Temp[1]]);
    TM1650_Wr_RAM(0x6C, NUM[Temp[2]]);
    TM1650_Wr_RAM(0x6E, NUM[Temp[3]]);
}


/*
*函数功能：数码管显示控制
*提    示：0表示亮度最大 1最小
*输入参数：u8 State:0关闭, 1开启; u8 Brightness:亮度 8 1 2-7
*输出参数：无
*返回值  ：无
*/
void Tube_CMD(u8 State, u8 Brightness)
{
    if(State)
    {
        TM1650_Wr_RAM(0x48,Brightness*16+1);//开启显示
    }
    else
    {
        TM1650_Wr_RAM(0x48,0x00);//关闭显示
    }
}


/*
*函数功能：挂载在tm1650上按键扫描
*提    示：无
*输入参数：
*输出参数：无
*返回值  ：无
*/
static u8 KEYtm_Scan(void)
{
    u8 Temp = 0;

    /*读取按键值*/
    IIC_Start();
    IIC_Wr_Byte(0x49);
    IIC_Wait_Ack();
    Temp = IIC_Rd_Byte();
    IIC_Ack();
    IIC_Stop();

    /*按键检测*/
    if(Temp == 0x5C)
    {
        return KEY_Mode_Press;
    }
    else if(Temp == 0x5D)
    {
        return KEY_Up_Press;
    }
    else if(Temp == 0x5E)
    {
        return KEY_Down_Press;
    }
    else if(Temp == 0x5F)
    {
        return KEY_Run_Press;
    }
    else
    {
        return 0;
    }
}


/*
*函数功能：按键扫描
*提    示：无
*输入参数：
*输出参数：无
*返回值  ：u8:检测到的按价值
*/
u8 KEY_Read(void)
{
    u8 Temp = 0;

    if(KEY_EXTI != 0)
    {
        Temp = KEY_EXTI;
        KEY_EXTI = 0;
        return Temp;
    }

    Temp = KEYtm_Scan();
    if(Temp != 0)
    {
        return Temp;
    }

    return 0;
}




/*
*函数功能：TM1650通讯IIC初始化
*提    示：tm1650挂载4个按键，外部中断挂载两个按键
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*IO初始化*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*空闲状态*/
    SCL_Set;
    SDA_Set;
}


/*
*函数功能：IIC起始
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void IIC_Start(void)
{
    SCL_Set;
    Delay_uS(IIC_uS);
    SDA_Set;
    Delay_uS(IIC_uS);
    SDA_Reset;
    Delay_uS(IIC_uS);
    SCL_Reset;
    Delay_uS(IIC_uS);
}

/*
*函数功能：IIC结束
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void IIC_Stop(void)
{

    SCL_Reset;
    Delay_uS(IIC_uS);
    SDA_Reset;
    Delay_uS(IIC_uS);
    SCL_Set;
    Delay_uS(IIC_uS);
    SDA_Set;
    Delay_uS(IIC_uS);
}


/*
*函数功能：IIC响应
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void IIC_Ack(void)
{
    SCL_Reset;
    Delay_uS(IIC_uS);
    SDA_Reset;
    Delay_uS(IIC_uS);
    SCL_Set;
    Delay_uS(40);
    SCL_Reset;
    Delay_uS(IIC_uS);
}

/*
*函数功能：IIC非响应
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void IIC_NoAck(void)
{
    SCL_Reset;
    Delay_uS(IIC_uS);
    SDA_Set;
    Delay_uS(IIC_uS);
    SCL_Set;
    Delay_uS(40);
    SCL_Reset;
}


/*
*函数功能：IIC等待响应
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
#define Wait_uS 10
static u8 IIC_Wait_Ack(void)
{
    u8 uS = 0;
    SCL_Reset;
    Delay_uS(IIC_uS);
    SDA_Set;
    Delay_uS(IIC_uS);
    SCL_Set;
    Delay_uS(IIC_uS);
    while(SDA_Read == 1)
    {
        Delay_uS(100);
        uS++;
        if(uS >= Wait_uS)
        {
            IIC_Stop();
            Delay_uS(IIC_uS);
            return 1;
        }
    }
    SCL_Reset;
    Delay_uS(IIC_uS);
    return 0;
}

/*
*函数功能：IIC写字节
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void IIC_Wr_Byte(u8 Data)
{
    u8 i = 0;


    for(i = 0; i < 8; i++)
    {
        SCL_Reset;
        Delay_uS(IIC_uS);

        if(Data >> 7)
        {
            SDA_Set;
        }
        else
        {
            SDA_Reset;
        }
        Data <<= 1;
        Delay_uS(IIC_uS);

        SCL_Set;
        Delay_uS(IIC_uS);
    }
    SCL_Reset;
    Delay_uS(IIC_uS);
    SDA_Set;
    Delay_uS(IIC_uS);
}

/*
*函数功能：IIC读字节
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static u8 IIC_Rd_Byte(void)
{
    u8 i = 0;
    u8 Temp = 0;

    SCL_Reset;
    Delay_uS(IIC_uS);
    SDA_Set;
    Delay_uS(IIC_uS);
    for(i = 0; i < 8; i++)
    {
        SCL_Reset;
        Delay_uS(IIC_uS);
        Temp <<= 1;
        Temp += SDA_Read;
        SCL_Set;
        Delay_uS(IIC_uS);
    }
    SCL_Reset;
    Delay_uS(IIC_uS);
    return Temp;
}



/*
*函数功能：写显存
*提    示：
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void TM1650_Wr_RAM(u8 Address, u8 Data)
{
    IIC_Start();
    IIC_Wr_Byte(Address);
    IIC_Wait_Ack();
    IIC_Wr_Byte(Data);
    IIC_Wait_Ack();
    IIC_Stop();
}




























/*
*函数功能：外部中断按键初始化
*提    示：外部中断挂载两个按键,A15 B15
*输入参数：无
*输出参数：无
*返回值  ：无
*/
static void EXTI_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


/*按键IO配置*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;  //STOP
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;  //内部上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//配置B3 B4 B15为普通IO模式
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;//LOCK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;  //内部上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);


/*中断配置*/
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

    /*引脚绑定*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);

    /*中断模式设置*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line15|EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*中断响应配置*/
    //STOP
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    //LOCK
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		    	//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}



/*
*函数功能：外部中断按键响应函数
*提    示：外部中断挂载两个按键,A15 B15
*输入参数：无
*输出参数：无
*返回值  ：无
*/
#define KEY_Stop_Rd GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define KEY_Lock_Rd GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)
/*Stop*/
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        Delay_mS(10);
        if(KEY_Stop_Rd == RESET)
        {
            KEY_EXTI= KEY_Stop_Press;
        }

        EXTI_ClearITPendingBit(EXTI_Line15);  //清除线路挂起位
    }
}
/*Lock*/
void EXTI3_IRQHandler(void)
{

     Delay_mS(10);
    if(KEY_Lock_Rd == RESET)
    {
        KEY_EXTI= KEY_Lock_Press;
    }

    EXTI_ClearITPendingBit(EXTI_Line3);  //清除线路挂起位
}



#ifndef __TUBE_KEY_H
#define __TUBE_KEY_H

#include "stm32f10x_conf.h"


#define KEY_Lock_Press 1
#define KEY_Stop_Press 2
#define KEY_Up_Press 3
#define KEY_Down_Press 4
#define KEY_Mode_Press 5
#define KEY_Run_Press 6



void Tube_Key_Init(void);
void Tube_DisNum(u16 Num);
void Tube_CMD(u8 State, u8 Brightness);
u8 KEY_Read(void);



#endif