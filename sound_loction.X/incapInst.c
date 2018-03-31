
#include <plib.h>

#define FOSC           80E6
#define PB_DIV         8
#define PRESCALE       256
#define MSEC           10E-3
#define T1_TICK       (500 * MSEC * FOSC)/(PB_DIV * PRESCALE)

/////////////////////////////////////////////
/////////////////////////////////////////////
//int main(void)
//{
//unsigned int CaptureTime;
//
//    //Clear interrupt flag
//    mIC1ClearIntFlag();
//
//    // Setup Timer 3
//    OpenTimer3(T3_ON | T1_PS_1_256, T1_TICK);
//
//    // Enable Input Capture Module 1
//    // - Capture Every edge
//    // - Enable capture interrupts
//    // - Use Timer 3 source
//    // - Capture rising edge first
//    OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_RISE | IC_ON );
//
//    // Wait for Capture events
//    while( !mIC1CaptureReady() ) ;
//
//    //Now Read the captured timer value
//    while( mIC1CaptureReady() )
//    {
//        CaptureTime = mIC1ReadCapture();
//        //process data
//        // ...
//    }
//
//    CloseCapture1();
//    CloseTimer3();
//
//    while(1)
//    {}
//
//}


/*
    初始化两个定时器
*/
void incap_timer_init(void)
{

    ConfigIntTimer23(T23_INT_ON|T23_INT_PRIOR_5|T23_INT_SUB_PRIOR_0);
    OpenTimer23(T23_ON|T23_IDLE_CON|T23_GATE_ON|T23_PS_1_1|T23_32BIT_MODE_ON|T23_SOURCE_INT, 16000000);    
}

void incap_timer_reset(void)
{

    ConfigIntTimer23(T23_INT_ON|T23_INT_PRIOR_5|T23_INT_SUB_PRIOR_0);
    OpenTimer23(T23_ON|T23_IDLE_CON|T23_GATE_ON|T23_PS_1_1|T23_32BIT_MODE_ON|T23_SOURCE_INT, 16000000);    
}



/*
初始化定时器
初始化三路输入捕获
*/
void ipc_inst_init(void)
{
    incap_timer_init();

    OpenTimer3(T3_ON | T1_PS_1_256, T1_TICK);

    OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_RISE | IC_ON );


}


// TODU: 调整中断入口和标志位
void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl5) _incap1Handler(void)
{
    data.incapFlag |= 0x01 << 0;
    INTClearFlag(INT_IC1);//中断标志清零
    //EnableIntT1;
}

void __ISR(_INPUT_CAPTURE_2_VECTOR, ipl5) _incap2Handler(void)
{
    data.incapFlag |= 0x01 << 1;
    INTClearFlag(INT_IC2);//中断标志清零
    //EnableIntT1;
}

void __ISR(_INPUT_CAPTURE_3_VECTOR, ipl5) _incap3Handler(void)
{
    data.incapFlag |= 0x01 << 2;
    INTClearFlag(INT_IC3);//中断标志清零
    //EnableIntT1;
}



