#include <xc.h>
#include <stdint.h>
#include <plib.h>
#include "log.h"
#include "data.h"
#include "cmp_extra.h"
#include "led.h"

void cmp_init()
{

    TRISBbits.TRISB1 = 1;//
    TRISBbits.TRISB3 = 1;//
    TRISBbits.TRISB14 = 1;//
    
    CVREFOpen(CVREF_ENABLE | CVREF_OUTPUT_DISABLE | CVREF_RANGE_HIGH | CVREF_SOURCE_AVDD | CVREF_STEP_1 );

    CMP1Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_CHANGE | CMP_POS_INPUT_CVREF | CMP1_NEG_INPUT_C1IN_POS); //C1INC -> mk1
    CMP1ConfigInt(mCMP1ClearIntFlag() | mCMP1SetIntPriority(5) | mCMP1SetIntSubPriority(3) | CMP_INT_ENABLE);

    CMP2Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_CHANGE | CMP_POS_INPUT_CVREF | CMP2_NEG_INPUT_C2IN_POS);//C2INC -> mk2
    CMP2ConfigInt(mCMP2ClearIntFlag() | mCMP2SetIntPriority(5) | mCMP2SetIntSubPriority(3) | CMP_INT_ENABLE);

    CMP3Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_CHANGE | CMP_POS_INPUT_CVREF | CMP_POS_INPUT_C3IN_POS);//C3INB ->mk0
    CMP3ConfigInt(mCMP3ClearIntFlag() | mCMP3SetIntPriority(5) | mCMP3SetIntSubPriority(3) | CMP_INT_ENABLE);
}

void __ISR(_COMPARATOR_3_VECTOR, ipl5) _MK0RecievedSound(void)
{
    TICK_PAUSE(MK0);
}

void __ISR(_COMPARATOR_1_VECTOR, ipl5) _MK1RecievedSound(void)
{
    TICK_PAUSE(MK1);
}

void __ISR(_COMPARATOR_2_VECTOR, ipl5) _MK2RecievedSound(void)
{
    TICK_PAUSE(MK2);
}
