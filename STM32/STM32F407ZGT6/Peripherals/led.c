#include "led.h"
#include "delay.h"

//LED闪光初始化函数定义
void LedInit(){
    //声明结构体变量
    GPIO_InitTypeDef GPIOF_InitStruct;
    //使能GPIOF时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
    //GPIO初始化
    GPIOF_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //开始IO口
    GPIOF_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //输出模式
    GPIOF_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIOF_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //时钟速度
    GPIOF_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽
	GPIO_Init(GPIOF,&GPIOF_InitStruct);
}

//int main(){

//    //初始化延时
//    delay_init(168);
//    //初始化Led
//    LedInit();
//    
//    while(1){
//        GPIO_SetBits(GPIOF,GPIO_Pin_9);
//        GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//        delay_ms(500);
//        GPIO_SetBits(GPIOF,GPIO_Pin_10);
//        GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//        delay_ms(500);
//    }
//}
