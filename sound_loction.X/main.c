#include <xc.h>
#include <stdint.h>
#include <plib.h>

#include "protocol.h"
#include "timer1.h"
#include "led.h"
#include "log.h"
#include "data.h"
#include "process.h"
#include "cmp_extra.h"

#define SYS_FREQ F_SYS_CLK
/*
*   sys config
*   fSYS    = 16.00MHz/FPLLIDIV*FPLLMUL/FPLLODIV = 64MHz
*   fPBCK   = fSYS/FPBDIV = 16MHz
*/

/************************************************************************************************************/
#pragma config ICESEL = ICS_PGx3        // ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)

// DEVCFG2
#pragma config FPLLIDIV = DIV_6         // PLL Input Divider (2x Divider)
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

void testHandler(void)
{
    int i=0;
    INTEnable( INT_AD1, INT_DISABLED );
    LOG_DEBUG("max data is %d, cnt %d", data.valueTemp, data.cntTemp);
    for(;i<15;i++)
    {
        LOG_DEBUG("buff%d = %d",i,data.temp2[i]);
    }
    INTEnable( INT_AD1, INT_ENABLED );
    data.cntTemp = 0;
    data.valueTemp = 0;
}

void main(void) 
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    /*开启中断*/
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    
    /*必须放在前面，因为后面初始化用到timer*/
    TIMER_SetConfiguration(TIMER_CONFIGURATION_1MS);
    led_init();
    led_write(0xff);
    
    led_state_init();
    uart1_init();
    //adc2_init();
    //cmp_init();
    
    TIMER_RequestTick(testHandler, 1000);
    TIMER_Start(testHandler);
    LOG_DEBUG("hello world");

    //主流程
    process_run();

    CMP2Close();
    CVREFClose();
    return;
}
