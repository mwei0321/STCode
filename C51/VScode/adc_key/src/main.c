/**
 * ��� UART��
 *      TX ���ţ�P31
 *      �����ʣ�9600
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


 
u8* Int2String(u16 num,u8 *str)//10���� 
{
    u16 i = 0,j = 0;//ָʾ���str 
    if(num<0)//���numΪ��������num���� 
    {
        num = -num;
        str[i++] = '-';
    } 
    //ת�� 
    do
    {
        str[i++] = num%10+48;//ȡnum���λ �ַ�0~9��ASCII����48~57������˵����0+48=48��ASCII���Ӧ�ַ�'0' 
        num /= 10;//ȥ�����λ    
    }while(num);//num��Ϊ0����ѭ��
    
    str[i] = '\0';
    
    //ȷ����ʼ������λ�� 
    if(str[0]=='-')//����и��ţ����Ų��õ��� 
    {
        j = 1;//�ӵڶ�λ��ʼ���� 
        ++i;//�����и��ţ����Խ����ĶԳ���ҲҪ����1λ 
    }
    //�Գƽ��� 
    for(;j<i/2;j++)
    {
        //�Գƽ������˵�ֵ ��ʵ����ʡ���м��������a+b��ֵ��a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;//����ת�����ֵ 
}