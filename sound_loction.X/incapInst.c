
#include <plib.h>
#include <stdbool.h>
#include "data.h"
#include "log.h"
#define FOSC           80E6
#define PB_DIV         8
#define PRESCALE       256
#define MSEC           10E-3
#define T1_TICK       (500 * MSEC * FOSC)/(PB_DIV * PRESCALE)

/////////////////////////////////////////////
/////////////////////////////////////////////
/*
    初始化32位定时器
*/
void incap_timer_init(void)
{

    ConfigIntTimer23(T23_INT_ON|T23_INT_PRIOR_5|T23_INT_SUB_PRIOR_0);
    OpenTimer23(T23_ON|T23_IDLE_CON|T23_PS_1_1|T23_32BIT_MODE_ON|T23_SOURCE_INT, 4294967295);    
}

void incap_timer_reset(void)
{
    TMR2 = 0;
    TMR2 = 0;
}


/*
初始化定时器
初始化三路输入捕获
mk1 -> PB14 ->IC2
mk2 -> PB1  ->IC3
mk3 -> PB3  ->IC4
*/
void ipc_inst_init(void)
{
    incap_timer_init();
    
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB14 = 1;

    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB3 = 0;
    ANSELBbits.ANSB14 = 0;

    PPSInput(4, IC2,  RPB14);
    PPSInput(2, IC3,  RPB1);
    PPSInput(1, IC4,  RPB3);


    mIC2ClearIntFlag();
    mIC3ClearIntFlag();
    mIC4ClearIntFlag();

    OpenCapture2(IC_ON| IC_SP_EVERY_EDGE | IC_CAP_32BIT| IC_INT_4CAPTURE | IC_FEDGE_FALL);
    OpenCapture3(IC_ON| IC_SP_EVERY_EDGE | IC_CAP_32BIT| IC_INT_4CAPTURE | IC_FEDGE_FALL);
    OpenCapture4(IC_ON| IC_SP_EVERY_EDGE | IC_CAP_32BIT| IC_INT_4CAPTURE | IC_FEDGE_FALL);
    
}

/*
*buffer must >= 4
*return buffer len
*/
int incap_dumpData(ENUM_MK mk, uint32_t *buffer)
{
    int i = 0;
    uint32_t temp = 0;
    switch(mk)
    {
        case MK0:
            while( mIC2CaptureReady())
            {
                temp = (uint32_t)mIC2ReadCapture();
                if(i<4)
                {
                    *(buffer + i) = temp;
                }
                i++;
            }
            break;
        case MK1:
            while( mIC3CaptureReady())
            {
                temp = (uint32_t)mIC3ReadCapture();
                if(i<4)
                {
                    *(buffer + i) = temp;
                }
                i++;
            }
            break;
        case MK2:
            while( mIC4CaptureReady())
            {
                temp = (uint32_t)mIC4ReadCapture();
                if(i<4)
                {
                    *(buffer + i) = temp;
                }
                i++;
            }
            break;
        default:
            break;
    }
    return i;
}


// TODU: 调整中断入口和标志位
void __ISR(_INPUT_CAPTURE_2_VECTOR, ipl5) _incap2Handler(void)
{
    LOG_DEBUG("here");
    mIC2ClearIntFlag();
}

void __ISR(_INPUT_CAPTURE_3_VECTOR, ipl5) _incap3Handler(void)
{
    LOG_DEBUG("here");
    mIC3ClearIntFlag();
}

void __ISR(_INPUT_CAPTURE_4_VECTOR, ipl5) _incap4Handler(void)
{
    LOG_DEBUG("here");
    mIC4ClearIntFlag();
}

/*32位定时器中断入口*/
void __ISR(_TIMER_3_VECTOR, ipl5) _Timer3Handler(void)
{
    LOG_DEBUG("timer-32bit timer out!");
    INTClearFlag(INT_T3);//中断标志清零
}


