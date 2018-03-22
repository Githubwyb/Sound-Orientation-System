#include "data.h"
#include "process.h"

void config_count_timer()
{
    ConfigIntTimer2(T1_INT_ON | TIMER_INT_PRIOR | TIMER_INT_SUB_PRIOR);
    OpenTimer2(TIMER_ON | CLOCK_DIVIDER | TIMER_SOURCE_INTERNAL,PR1_SETTING);  //分频设置为1
}

