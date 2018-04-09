/*
*simple pwm
*/

#ifndef __PWM_SIM_H__
#define	__PWM_SIM_H__

#include "data.h"

#define MAX_PWM_INST 10
#define PWM_PERIOD_CNT 32

#define PWM_ERROR_INDEX (-1)

typedef (void*)pwm_cmp_handler(void);

typedef enum
{
    PWM_CMP_L,
    PWM_CMP_H,
}ENUM_PWM_CMP_OUT;

typedef struct
{
    bool en;
    uint32_t cmpCnt;
    ENUM_PWM_CMP_OUT lstState;
    pwm_cmp_handler handler_l;
    pwm_cmp_handler handler_h;

}ST_PWM_INST;

void pwm_run(void);
void pwm_stop(void);
int pwm_request (uint32_t rate, pwm_cmp_handler handler_l, pwm_cmp_handler handler_h);
bool pwm_reConfig(int pwmIndex, uint32_t newRate);
bool pwm_cancle(int pwmIndex);

#endif	/* __PROCESS_H__ */

