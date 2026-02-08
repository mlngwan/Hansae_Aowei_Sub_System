#include "Rte_Main.h"
#include "Rte_SwcA.h"
#include "Rte_Adc.h"
#include "Bsw_Adc.h"
#include "SwcAdcMon.h"

/* RTE Task 스케줄러 - Runnable 디스패치 */

void Rte_Task_1ms(void)
{
    /* 1ms Runnable 디스패치 (현재 비어있음) */
    /* TODO: 1ms Runnable들 추가 시 여기서 호출 */
}

void Rte_Task_5ms(void)
{
    /* BSW ADC 샘플링 */
    Bsw_Adc_MainFunction_5ms();
    
    /* RTE ADC Publish */
    Rte_Adc_Publish_5ms();
    
    /* 5ms Runnable 디스패치 */
    SwcA_Runnable_5ms();
}

void Rte_Task_10ms(void)
{
    /* BSW ADC 가공 */
    Bsw_Adc_MainFunction_10ms();
    
    /* RTE ADC Publish (가공 결과) */
    Rte_Adc_Publish_10ms();
    
    /* 10ms Runnable 디스패치 */
    SwcAdcMon_Runnable_10ms();  /* 새로운 ADC 모니터 SWC */
    SwcA_Runnable_10ms();        /* 기존 레거시 래퍼 */
}

void Rte_Task_20ms(void)
{
    /* 20ms Runnable 디스패치 */
    SwcA_Runnable_20ms();
}

void Rte_Start(void)
{
    /* RTE 초기화 */
    Bsw_Adc_Init();
    SwcAdcMon_Init();
}