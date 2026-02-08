#ifndef RTE_SWCA_H
#define RTE_SWCA_H

#include "Rte_Type.h"
#include <stdint.h>

/* SWC가 사용할 Runnable들 */
void SwcA_Runnable_5ms(void);
void SwcA_Runnable_10ms(void);
void SwcA_Runnable_20ms(void);

/* (나중에 필요하면) 서비스 예시: 시간(ms) 얻기 */
Std_ReturnType Rte_Call_SwcA_GetTimeMs(uint32_t* ms);

#endif