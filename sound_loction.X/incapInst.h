#ifndef __INCAP_INST_H__
#define __INCAP_INST_H__

#define MAX_DELAY  (6000) //mk之间的最大延迟
#define MAX_DELAY_SHAKE  (200)//mk沿最大抖动时间 832 = 52us

#define MAX_DELTA_CNT  (16000)//同一mk两个沿之间的最大时间差

#define MK0_BUFFER_FULL()  (IC2CONbits.ICOV)
#define MK1_BUFFER_FULL()  (IC3CONbits.ICOV)
#define MK2_BUFFER_FULL()  (IC4CONbits.ICOV)

void incap_timer_init(void);
void incap_timer_reset(void);
void ipc_inst_init(void);
int main2(void);
#endif 