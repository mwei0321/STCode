/**
 * 软件 UART：
 *      TX 引脚：P31
 *      波特率：9600
*/

#include "config.h"
#include "soft_uart.h"
#include "delay.h"

u8* Int2String(int num,u8 *str);


void adcinit() {
    
    ADC_InitTypeDef adcParam;

    adcParam.ADC_Px = ADC_P12;
    adcParam.ADC_Speed = ADC_90T;
    adcParam.ADC_Power = ENABLE;
    adcParam.ADC_AdjResult = ADC_RES_H2L8;
    adcParam.ADC_Polity = PolityHigh;
    adcParam.ADC_Interrupt = ENABLE;

    ADC_Inilize(&adcParam);
}

void main()
{
    bit keydown;
    u8 key;
    u16 adcrl;
    u8 h8,l8;
    u8 adcres[16] = {0};
    adcinit();

    while (1)
    {

        adcrl = Get_ADC10bitResult(2);

        if(keydown == 0) {
            if(adcrl > 600 && adcrl < 630) {
                key = 1;
                keydown = 1;
            }else if(adcrl > 370 && adcrl < 400) { 
                key = 2;
                keydown = 1;
            }else if(adcrl > 850 && adcrl < 880) { 
                key = 3;
                keydown = 1;
            }
        }
        
        if (keydown && key > 0) {
            key = 0;
            keydown = 0;
            LOG("push down key");
            LOG(key+"0");
            LOG("\n");
        }

        // Int2String(adcrl,adcres);

        // LOG(Int2String);
        // LOG("\t");

        // LOG("Hello, STC15F104W\n");
    }
}


 
u8* Int2String(u16 num,u8 *str)//10进制 
{
    u16 i = 0,j = 0;//指示填充str 
    if(num<0)//如果num为负数，将num变正 
    {
        num = -num;
        str[i++] = '-';
    } 
    //转换 
    do
    {
        str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
        num /= 10;//去掉最低位    
    }while(num);//num不为0继续循环
    
    str[i] = '\0';
    
    //确定开始调整的位置 
    if(str[0]=='-')//如果有负号，负号不用调整 
    {
        j = 1;//从第二位开始调整 
        ++i;//由于有负号，所以交换的对称轴也要后移1位 
    }
    //对称交换 
    for(;j<i/2;j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;//返回转换后的值 
}