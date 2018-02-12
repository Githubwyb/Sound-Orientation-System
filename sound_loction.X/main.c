#include <xc.h>
#include <stdint.h>
#include <plib.h>

#include "protocol.h"
#include "timer1.h"
#include "led.h"
#include "log.h"

//11.0592MHz = 115200*96
/*
*   sys config
*   fSYS    = 22.1184/FPLLIDIV*FPLLMUL/FPLLODIV = 44.236800MHz
*   fPBCK   = fSYS/FPBDIV = 11.0592MHz
*/
/************************************************************************************************************/
#pragma config ICESEL = ICS_PGx3        // ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)

// DEVCFG2
#pragma config FPLLIDIV = DIV_12         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (16x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_4           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)
/************************************************************************************************************/
    unsigned int channel4;    // conversion result as read from result buffer
    unsigned int channel5;    // conversion result as read from result buffer
    unsigned int offset;    // buffer offset to point to the base of the idle buffer
void timer_test_handler()
{
    LOG_DEBUG("chan4:%u, chan5:%u", channel4, channel5);
}

void main(void) {
    /*开启中断*/
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    /*必须放在前面，因为后面初始化用到timer*/
    TIMER_SetConfiguration(TIMER_CONFIGURATION_1MS);
    
    led_init();
    uart1_init();
    adc_init();
    
    //led_state(ON);
    //print("hello world~\r\n");
    LOG_DEBUG("hello world");
    
    
    
    unsigned int i = 1, z, pwm = 125;  

    PPSOutput(4, RPB14, OC3);  // Set OC4 to RPA3 with peripheral pin select  

    //ConfigIntOC3(OC_INT_OFF|OC_INT_SUB_PRI_0|OC_INT_PRIOR_0);
    /* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/  
    OpenOC3(OC_ON | OC_IDLE_CON| OC_TIMER_MODE16| OC_TIMER2_SRC| OC_HIGH_LOW, 0, 0);  
    /* Open Timer2 with Period register value */  
    OpenTimer2(T2_ON | T2_PS_1_256 | T2_SOURCE_INT, 0x100);  
  
    // Now blink LEDs ON/OFF forever.  
    while (1)  
    {  
        // Insert some delay  
        for (z = 0; z < 1024 * 256; z++)  ;  
        SetDCOC3PWM(pwm++); // Write new duty cycle  
        if (pwm > 255)  
            pwm = 0;  
    } 
     
    return;
}
