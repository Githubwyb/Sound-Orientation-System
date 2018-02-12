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

void main(void) {
    LOG_DEBUG("hello world");

    /*开启中断*/
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    /*必须放在前面，因为后面初始化用到timer*/
    TIMER_SetConfiguration(TIMER_CONFIGURATION_1MS);

    led_init();
    UART1Config();

    led_state(ON);

    CVREFOpen(CVREF_ENABLE | CVREF_OUTPUT_DISABLE | CVREF_RANGE_HIGH | CVREF_SOURCE_AVDD | CVREF_STEP_13);
    CMP2Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_HIGH_TO_LOW | CMP_POS_INPUT_CVREF | CMP2_NEG_INPUT_C2IN_POS);
//    CMP2Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_LOW_TO_HIGH | CMP_POS_INPUT_C2IN_POS | CMP2_NEG_INPUT_IVREF);
    CMP2ConfigInt(mCMP2ClearIntFlag() | mCMP2SetIntPriority(7) | mCMP2SetIntSubPriority(2) | CMP_INT_ENABLE);

    INTEnableSystemMultiVectoredInt();

    while(1)
    {

    }

    CMP2Close();
    CVREFClose();
    led_state(OFF);

    return;
}


void __ISR(_COMPARATOR_2_VECTOR, ipl7) _MK3RecievedSound(void)
{
    led_set(6, ON);
    int i;
    for (i = 0; i < 3000000; i ++);
    led_set(6, OFF);
    mCMP2ClearIntFlag();
}
