#include "Typedefs.h"



typedef struct{
	uint16 Cnt_100ms; /* 10 ms counter */
}Stm_t;




void STMInitialize(void);
void STM_CH0_ISR(void);

extern void TaskScheduler_TaskCount();


