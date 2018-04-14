#include <plib.h>
#include "pwmSim.h"

volatile ST_PWM_INST pwm_map[MAX_PWM_INST] = {0};
int pwm_cnt = 0;

void pwm_run(void)
{
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_2 | T4_INT_SUB_PRIOR_0);
    OpenTimer4(T4_ON | T4_PS_1_1 | T4_SOURCE_INT,10000);  //1000000: 2/32 ms
}

void pwm_stop(void)
{
    ConfigIntTimer4(T4_INT_OFF);
    CloseTimer4();
}

int pwm_request (int rate, pwm_cmp_handler handler_l, pwm_cmp_handler handler_h)
{
    int i = 0;
    for( ; i < MAX_PWM_INST; i++)
    {
        if( false == pwm_map[i].en)
        {
            pwm_map[i].en = true;
            pwm_map[i].cmpCnt = rate;
            pwm_map[i].lstState = PWM_CMP_L;
            pwm_map[i].handler_l = handler_l;
            pwm_map[i].handler_h = handler_h;
            return i;
        }
    }

    return PWM_ERROR_INDEX;
}

int temp = 0;
bool pwm_reConfig(int pwmIndex, int newRate)
{
    int i = 0;
    for( ; i < MAX_PWM_INST; i++)
    {
        if(pwmIndex == i)
        {
            if(true == pwm_map[i].en)
            {
                temp = newRate;
                pwm_map[i].cmpCnt = newRate;
                return true;
            }
        }
    }
    return false;
}


bool pwm_cancle(int pwmIndex)
{
    int i = 0;
    for( ; i < MAX_PWM_INST; i++)
    {
        if(pwmIndex == i)
        {
            pwm_map[i].en == false;
        }
    }
    return true;
}


/*
* low [0,pwm_map[i].cmpCnt)
* high [pwm_map[i].cmpCnt, PWM_PERIOD_CNT)
*/
void __ISR(_TIMER_4_VECTOR, ipl2) pwm_runTick_handler(void)
{
    int i = 0;
    pwm_cnt = (++pwm_cnt)%PWM_PERIOD_CNT;
    
    for( ; i < MAX_PWM_INST; i++)
    {
        if(true == pwm_map[i].en)
        {
            if(pwm_cnt == pwm_map[i].cmpCnt)
            {
                pwm_map[i].handler_h();
                pwm_map[i].lstState = PWM_CMP_H;
            }
            else if(pwm_cnt == 0)
            {
                pwm_map[i].handler_l();
                pwm_map[i].lstState = PWM_CMP_L;
            }

        }
    }

    INTClearFlag(INT_T4);//中断标志清零
}

