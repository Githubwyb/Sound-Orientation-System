#include "led.h"
void led_init(void)
{
    ANSELB =0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB14 = 0;
    
    led_state(OFF);
}

void led_set(u8 led, ENUM_LED_LEVEL sw)
{
    switch(led)
    {
        case 0:
            LEDbits.D0 = sw;
            break;
        case 1:
            LEDbits.D1 = sw;
            break;
        case 2:
            LEDbits.D2 = sw;
            break;
        case 3:
            LEDbits.D3 = sw;
            break;
        case 4:
            LEDbits.D4 = sw;
            break;
        case 5:
            LEDbits.D5 = sw;
            break;
        case 6:
            LEDbits.D6 = sw;
            break;
        case 7:
            LEDbits.D7 = sw;
            break; 
        default: 
            break;       
    } 
}

void led_write(u8 s)
{
    LEDbits.D0 = s & 0x01;
    LEDbits.D1 = !!(s & (0x01 <<1));
    LEDbits.D2 = !!(s & (0x01 <<2));   
    LEDbits.D3 = !!(s & (0x01 <<3));
    LEDbits.D4 = !!(s & (0x01 <<4));   
    LEDbits.D5 = !!(s & (0x01 <<5));
    LEDbits.D6 = !!(s & (0x01 <<6));   
    LEDbits.D7 = !!(s & (0x01 <<7));
}


