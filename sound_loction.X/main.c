/*
 * File:   main.c
 * Author: Shenghui_Wu
 *
 * Created on 2017年12月23日, 下午9:13
 */
#include <xc.h>
#include <stdint.h>

#include "bit_operation.h"
#include "led_drive.h"

// PIC32MX250F128B Configuration Bit Settings
// 'C' source line config statements

// DEVCFG3
// USERID = No Setting
// DEVCFG1
#pragma config FNOSC = PRI              // Oscillator Selection Bits (Primary Osc (XT,HS,EC))
#pragma config POSCMOD = EC             // Primary Oscillator Configuration (External clock mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config ICESEL = ICS_PGx3        // ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)

void init();
void start_flash();



void init()
{
    ANSELB = 0;
    TRISB = 0;
    PORTB = 0;
    
    return ;
}

void start_flash()
{
    int i;
    uint8_t led_display_vector = 1;
    while(1)
    {
        led_drive(led_display_vector);
        led_display_vector = led_display_vector << 1;
        
        if (!led_display_vector)
        {
            led_display_vector = 1;
        }
        
        i = 0;
        while( i < 100000)
        {
            i++;
        }
    }
}

void main(void) {
    init();

    start_flash();

    //    T1CONbits.ON = 0;
    //    T1CONbits.TCS = 0;
    //    T1CONbits.TCKPS = 0;
    //    TMR1 = 0x0;
    //    PR1 = 0xFFFF;
    //    T1CONSET = 1; 
    //    IFS0bits.T1IF = 0;
    //    IPC1bits.T1

    return;
}
