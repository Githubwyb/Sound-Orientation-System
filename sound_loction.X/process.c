#include "data.h"
#include "led.h"
#include "log.h"
#include <math.h>
#include <stdbool.h>
#include "process.h"
#include "cmp_extra.h"

#define CLEAR_TIMER2() {TMR2 = 0;}
#define OPEN_TIMER2() OpenTimer2(T2_ON | T2_SOURCE_EXT | T2_PS_1_1, 65535)
#define CLOSE_TIMER2() CloseTimer2() 
#define GET_TIMER2_CNT()  (TMR2)
#define SET_TIMER2_CNT(value) TMR2 = (value)

void config_count_timer()
{
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_6 | T2_INT_SUB_PRIOR_0);
}

void __ISR(_TIMER_2_VECTOR, ipl2) _Timer2Handler(void)
{
    //time out 
    CLOSE_TIMER2();
    data.processState = STATE_TIMEOUT;
    LOG_DEBUG("time out!");
    INTClearFlag(INT_T1);//中断标志清零
}


void TICK_PAUSE(ENUM_MK mk)
{
    switch(mk)
    {
        case MK0: {DISABLE_MK0();break;}
        case MK1: {DISABLE_MK1();break;}    
        case MK2: {DISABLE_MK2();break;}
    }
    switch(data.processState)
    {
        case STATE_WAIT_FIRST_PULSE:
            OPEN_TIMER2();
            data.record[0].mk = mk;
            data.record[0].cntT = 0;
            data.processState = STATE_WAIT_SECOND_PULSE;
            break;
        case STATE_WAIT_SECOND_PULSE:
            data.record[1].mk = mk;
            data.record[1].cntT = GET_TIMER2_CNT();
            data.processState = STATE_WAIT_THIRD_PULSE;
            break;
        case STATE_WAIT_THIRD_PULSE:
            CLOSE_TIMER2();//停止了timer 只是关掉了中断
            data.record[2].mk = mk;
            data.record[2].cntT = GET_TIMER2_CNT();
            data.processState = STATE_OVER;
            break;
        default:
            // 在STATE_OVER状态等待监测处理
            break;
    }
}

static uint8_t process_getLedNum_byDegree(uint16_t degree)
{
    uint8_t led = 0;
    if( (int)((float)(degree)/45.0 + 0.5) > 7 )
    {
        led  =  0;
    }
    return led;
}

/*清除标记，使能MK中断，禁止timer中断*/
void process_clear(void)
{
    data.processState = STATE_IDLE;

    CLEAR_TIMER2();
    ENABLE_MK();
}


//调节阈值、状态灯等
void process_reConfig(void)
{
    
}

#define PI (3.1415926535897932384626433832795)
#define FACTOR_T (1.0/16000000.0) //时间系数(周期)
#define FACTOR_LENGTH (FACTOR_T/340.0*1000.0) //距离系数 distance(mm) = cnt * FACTOR_LENGTH

#define BOARD_X0_CNT (2.9368393249511608098391401276485/FACTOR_LENGTH)
#define BOARD_Y0_CNT (22.307509380910734250752544355893/FACTOR_LENGTH)
#define BOARD_X1_CNT (-20.787289481503952012884121761428/FACTOR_LENGTH)
#define BOARD_Y1_CNT (-8.610377228214519863890349640684/FACTOR_LENGTH)
#define BOARD_MK_DISTANCE_CNT (77.942286340599478208735085367764/FACTOR_LENGTH)

#define DEGRE2PI(x) ((x)*180.0/PI)
#define DEGREE_SHIFT DEGRE2PI(22.5) //degree_mk1
#define DEGREE_CHANGE0(x) DEGRE2PI(x-22.5) //弧度值
#define DEGREE_CHANGE1(x) DEGRE2PI(x+115)

//#define PI (3.14159265358979323846)
bool process_dealData(void)
{
    int i = 0;
    float cnt[3];
    float c_temp = BOARD_MK_DISTANCE_CNT /2;
    float a_temp,b_temp;
    float x,y,k1,k2,ct1,ct2;

    if((data.record[1].mk == data.record[0].mk)||(data.record[1].mk == data.record[2].mk)) return false;
    
    //mk排序
    for(i = 0; i < 3; i++)
    {
        int j =0;
        for(; j<3; j++)
        {
            if(data.record[j].mk == i)
            {
                cnt[i] = data.record[j].cntT;
            }
        }
    }

    a_temp = fabs(cnt[0] - cnt[1])/2;
    if(c_temp < a_temp) return false;
    b_temp = sqrt(c_temp*c_temp - a_temp*a_temp);
    //相对偏角
    ct1 = atan2(b_temp,a_temp); // -pi~pi
    ct1 = b_temp/a_temp;
    if(cnt[0] > cnt[1]) ct1 = -1.0*ct1;

    
    a_temp = fabs(cnt[1] - cnt[2])/2;
    if(c_temp < a_temp) return false;
    b_temp = sqrt(c_temp*c_temp - a_temp*a_temp);
    //相对偏角
    ct2 = atan2(b_temp,a_temp); // -pi~pi
    ct2 = b_temp/a_temp;
    if(cnt[0] > cnt[1]) ct2 = -1.0*ct2;


    //转换为绝对偏角
    ct1 = DEGREE_CHANGE0(ct1);
    ct2 = DEGREE_CHANGE1(ct2);

    //转换为绝对斜率
    k1 = tan(ct1);
    k2 = tan(ct2);

    x = ((BOARD_Y1_CNT - BOARD_Y0_CNT) - (ct2*BOARD_X1_CNT - ct1*BOARD_X0_CNT))/(ct1 - ct2);
    y = BOARD_Y1_CNT + ct2*(x - BOARD_X0_CNT);

    //out
    data.degree = ((int)(atan2(y, x)*180.0/PI) + 360) % 360;
    data.distance = (uint32_t)(sqrt(x*x + y*y) * FACTOR_LENGTH);
    
    return true;
}

void process_resultOut(void)
{
    led_set(process_getLedNum_byDegree(data.degree), ON);
    LOG_DEBUG("result, mk:%d %d %d, record:%d %d %d, degree:%d, led:%d",
                        data.record[0].mk+1, data.record[1].mk+1, data.record[2].mk+1,
                        data.record[0].cntT, data.record[1].cntT, data.record[2].cntT,
                        data.degree, process_getLedNum_byDegree(data.degree));
}

/*在检测过程中 除了time2中断表示超时，不准其它中断！*/
/*监测tick拥有最高优先级*/
void process_run(void)
{
    do
    {
        switch(data.processState)
        {
            case STATE_IDLE:
                //清除标志位 开始任务
                data.processState = STATE_WAIT_FIRST_PULSE;
                LOG_DEBUG("STATE: STATE_IDLE");
                process_clear();
                break;
            case STATE_OVER:
                LOG_DEBUG("STATE: STATE_OVER");
                //处理数据
                if(false == process_dealData())
                {
                    LOG_DEBUG("deal data error");
                    data.processState = STATE_IDLE;
                    break;
                }
                //检测配置
                process_reConfig();
                //输出
                process_resultOut();     
                data.processState = STATE_IDLE;
                break;
            case STATE_TIMEOUT:
            case STATE_ERROR:
                LOG_DEBUG("STATE: STATE_ERROR");
                data.processState = STATE_IDLE;
                break;
        }    
    }
    while (1);
    
}

