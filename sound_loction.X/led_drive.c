/**
 * LED驱动，输入一个8位的uint8_t，控制每一位对应的灯亮灭
 * @param display_vector
 */

#include <xc.h>
#include <stdint.h>

#include "bit_operation.h"
#include "led_drive.h"

void led_drive(uint8_t display_vector)
{
    if(bittest(display_vector, 0))
    {
        LED0_ON;
    }
    else
    {
        LED0_OFF;
    }

    if(bittest(display_vector, 1))
    {
        LED1_ON;
    }
    else
    {
        LED1_OFF;
    }

    if(bittest(display_vector, 2))
    {
        LED2_ON;
    }
    else
    {
        LED2_OFF;
    }
    
    if(bittest(display_vector, 3))
    {
        LED3_ON;
    }
    else
    {
        LED3_OFF;
    }
    
    if(bittest(display_vector, 4))
    {
        LED4_ON;
    }
    else
    {
        LED4_OFF;
    }
    
    if(bittest(display_vector, 5))
    {
        LED5_ON;
    }
    else
    {
        LED5_OFF;
    }
    
    if(bittest(display_vector, 6))
    {
        LED6_ON;
    }
    else
    {
        LED6_OFF;
    }
    
    if(bittest(display_vector, 7))
    {
        LED7_ON;
    }
    else
    {
        LED7_OFF;
    }
}