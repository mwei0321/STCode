#include "key.h"

void KeyInit(){
    
    GPIO_InitTypeDef GPIOEA_InitStruct;
    
    //使能GPIOE,GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);
    
    GPIOEA_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
    GPIOEA_InitStruct.GPIO_Mode = GPIO_Mode_IN; //输出模式
    GPIOEA_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIOEA_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //时钟速度
    GPIO_Init(GPIOE,&GPIOEA_InitStruct);
    
    GPIOEA_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIOEA_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
    GPIO_Init(GPIOA,&GPIOEA_InitStruct);
}

u8 keyScan(u8 mode){
    static u8 key_up = 1;
    if(mode)key_up=1; //支持连按
    if(WK_UP || (KEY0==0) || (KEY1==0) || (KEY2==0)){
        delay_ms(10);
        key_up=0;
        if(WK_UP){
            return 4;
        }else if(!KEY0){
            return 1;
        }else if(!KEY1){
            return 2;
        }else if(!KEY2){
            return 3;
        }
    }else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1;
    
    return 0;
}

int main(){
    
    while(1){
    
    }
}
