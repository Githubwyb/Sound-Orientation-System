#include "adcInst.h"
#include <plib.h>
#include <stdio.h>
#include <math.h>
#include "log.h"
#include "led.h"
ST_ADC_RESULT adc_result = {0};
void adc_getData_handler(void)
{
    int i = 0;
    uint16_t temp = 0;
    uint32_t sum = 0;
    for(i = 0; i< 16; i++)
    {
        temp = ReadADC10(i);
        sum += temp;
    }
    adc_result.v_avg = sum/16;
    SetDCOC3PWM(adc_result.v_avg);
    //LOG_DEBUG("ave \t= %u, v \t= %.4f\r\n", adc_result.v_avg, ADC_VREF * (float)adc_result.v_avg/1024.0f);
}

void adc_init(void)
{
    //首先关闭ADC
    CloseADC10();
    //                 使能模块   | 输出数据格式    |自动采样模式设置| 开启自动采样
    #define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
    //                ADC参考电压设置   | 输入失调校准模式关闭    | 关闭扫描      | 中断条件设置           | 2*byte         | 仅使用通道A
    #define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_16 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_OFF
    //                  ADC工作时钟设置为RC   | 采样次数15
    #define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_5
    //                  不扫描任何通道
    #define PARAM4    SKIP_SCAN_ALL
    //                  设置AN11为模拟输入
    #define PARAM5    ENABLE_AN11_ANA

    //                     设置A通道的负输入 | 设置A通道的正输入         |  设置B通道的负输入          | 设置B通道的正输入
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN11 |  ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN5); // configure to sample AN4 & AN5
    
    //开启ADC
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 ); // configure ADC using parameter define above
    EnableADC10(); // Enable the ADC
    
    //第一次采样值不可用
    while( ! mAD1GetIntFlag() );
    
    //开启读取定时器
    TIMER_RequestTick(adc_getData_handler, 100);
    TIMER_Start(adc_getData_handler);
}