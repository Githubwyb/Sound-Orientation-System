#include <xc.h>
#include <stdint.h>
#include <plib.h>

#include "cmp_extra.h"
#include "led.h"
#include "protocol.h"

void refreshRecord();
void outputError();
void calculateLED();
void recordEvent(int MK);

int eventNum = 0;
int eventSeq[3] = {MK_NONE, MK_NONE, MK_NONE};

void cmp_init()
{
    CMP1Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_LOW_TO_HIGH | CMP_POS_INPUT_C1IN_POS | CMP1_NEG_INPUT_IVREF);
    CMP1ConfigInt(mCMP1ClearIntFlag() | mCMP1SetIntPriority(7) | mCMP1SetIntSubPriority(3) | CMP_INT_ENABLE);

    CMP2Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_LOW_TO_HIGH | CMP_POS_INPUT_C2IN_POS | CMP2_NEG_INPUT_IVREF);
    CMP2ConfigInt(mCMP2ClearIntFlag() | mCMP2SetIntPriority(7) | mCMP2SetIntSubPriority(3) | CMP_INT_ENABLE);

    CMP3Open(CMP_ENABLE | CMP_OUTPUT_NONINVERT | CMP_OUTPUT_DISABLE | CMP_RUN_IN_IDLE | CMP_EVENT_LOW_TO_HIGH | CMP_POS_INPUT_C3IN_POS | CMP3_NEG_INPUT_IVREF);
    CMP3ConfigInt(mCMP3ClearIntFlag() | mCMP3SetIntPriority(7) | mCMP3SetIntSubPriority(3) | CMP_INT_ENABLE);
}

void refreshRecord()
{
    eventNum = 0;
    eventSeq[0] = MK_NONE;
    eventSeq[1] = MK_NONE;
    eventSeq[2] = MK_NONE;
    
    mCMP1IntEnable(1);
    mCMP2IntEnable(1);
    mCMP3IntEnable(1);
}

void outputLED(u8 x)
{
    led_set(x, ON);
    int i;
    for (i = 0; i < 5000000; i ++);
    led_set(x, OFF);
}

void outputError()
{
//    led_state(OFF);
//    int i;
//    for (i = 0; i < 10000; i ++);
//    led_state(ON);
}

void calculateLED()
{
    if(eventSeq[0] == MK1)
    {
        if(eventSeq[1] == MK2 && eventSeq[2] == MK3)
        {
            outputLED(2);
        }
        else if(eventSeq[1] == MK3 && eventSeq[2] == MK2)
        {
            outputLED(0);
        }
        else
        {
            outputError();
        }
    }
    else if(eventSeq[0] == MK2)
    {
        outputLED(3);
    }
    else if(eventSeq[0] == MK3)
    {
        outputLED(6);
    }
    else
    {
        outputError();
    }
}

void recordEvent(int MK)
{
    switch(eventNum)
    {
        case 0:
            eventSeq[0] = MK;
            eventNum++;
            break;
        case 1:
            eventSeq[1] = MK;
            eventNum++;
            break;
        case 2:
            eventSeq[2] = MK;
            calculateLED();
            refreshRecord();
            break;
        default:
            refreshRecord();
            break;
    }
}

void __ISR(_COMPARATOR_3_VECTOR, ipl7) _MK1RecievedSound(void)
{
    mCMP3IntEnable(0);
    recordEvent(MK1);
    mCMP3ClearIntFlag();
}

void __ISR(_COMPARATOR_2_VECTOR, ipl7) _MK2RecievedSound(void)
{
    mCMP2IntEnable(0);
    recordEvent(MK2);
    mCMP2ClearIntFlag();
}

void __ISR(_COMPARATOR_1_VECTOR, ipl7) _MK3RecievedSound(void)
{
    mCMP1IntEnable(0);
    recordEvent(MK3);
    mCMP1ClearIntFlag();
}
