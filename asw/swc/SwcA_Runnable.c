#include "Rte_SwcA.h"
#include "Main_Api.h"

/* 기존 레거시 함수들 */
extern void TaskScheduler_5ms(void);
extern void TaskScheduler_10ms(void);
extern void TaskScheduler_20ms(void);

void SwcA_Runnable_5ms(void)
{
    TaskScheduler_5ms();
}

void SwcA_Runnable_10ms(void)
{
    TaskScheduler_10ms();
}

void SwcA_Runnable_20ms(void)
{
    TaskScheduler_20ms();
}