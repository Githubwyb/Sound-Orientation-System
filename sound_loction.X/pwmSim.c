#include <plib.h>
#include "pwmSim.h"
#include "led.h"

uint16_t pwm_led_cmpCnt[PWM_LED_NUM] = {0};
int pwm_cnt = 0;

void pwm_led_run(void)
{
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_2 | T4_INT_SUB_PRIOR_0);
    OpenTimer4(T4_ON | T4_PS_1_1 | T4_SOURCE_INT,10000);  //1000000: 2/32 ms
}

void pwm_led_stop(void)
{
    ConfigIntTimer4(T4_INT_OFF);
    CloseTimer4();
}

//float light : 0~1
void pwm_led_reConfig(int ledIndex, float light)
{
    int i = 0;
    if(light < 0) return;
    pwm_led_cmpCnt[ledIndex] = (uint16_t)(light*(float)(PWM_PERIOD_CNT-1));
}

void pwm_led_reConfig_batch(float *light)
{
    int i = 0;
    for( ; i < PWM_LED_NUM; i++)
    {
        pwm_led_reConfig(i, light[i]);
    }
}

/*
* low [0,pwm_map[i].cmpCnt)
* high [pwm_map[i].cmpCnt, PWM_PERIOD_CNT)
*/
void __ISR(_TIMER_4_VECTOR, ipl2) pwm_runTick_handler(void)
{
    int i = 0;
    
    pwm_cnt = (++pwm_cnt)%PWM_PERIOD_CNT;
    
    for( ; i < PWM_LED_NUM; i++)
    {
        if(pwm_cnt == pwm_led_cmpCnt[i])
            led_set(i, OFF);
        else if(pwm_cnt == 0)
            led_set(i, ON);
    }

    INTClearFlag(INT_T4);//中断标志清零
}

