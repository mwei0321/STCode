/**
 * 软件 UART�?
 *      TX 引脚：P31
 *      波特率：9600
*/

#include "config.h"
// #include "soft_uart.h"
#include "delay.h"
#include "tm1620.h"
#include "ds18b20.h"

void main()
{
    uint16_t temp = 0;
    uint8_t l1,l2,l3,l4;
    uint8_t hide = 20;
    uint8_t pointflash = 0;
    // uint8_t show = 5;
    // EA = 1;
    delay_ms(100);
    TM1620Init();
    delay_ms(100);
    TM1620Test();


    while (1)
    {
        // LOG("Hello, STC15F104W\n");
        
        temp = getTemp();
        temp = temp * 0.0625 * 100 + 0.5;

        l1 = (temp % 10000 / 1000);
        l2 = (temp % 1000 / 100);
        l3 = (temp % 100 / 10);
        l4 = (temp % 10);

        if 
        TM1620ShowByAddr(0xc0,LedCode[l1]);
        TM1620ShowByAddr(0xc2,LedCode[l2]|0x80);
        TM1620ShowByAddr(0xc4,LedCode[l3]);
        TM1620ShowByAddr(0xc6,LedCode[l4]);

        // while (hide--)
        // {
            delay_ms(200);
        // }
        // hide = 50;
    }
}
