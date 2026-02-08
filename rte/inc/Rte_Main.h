#ifndef RTE_MAIN_H
#define RTE_MAIN_H

#include "Rte_Type.h"

/* RTE Task 엔트리 포인트 선언 */
void Rte_Task_1ms(void);
void Rte_Task_5ms(void);
void Rte_Task_10ms(void);
void Rte_Task_20ms(void);

/* RTE 초기화 */
void Rte_Start(void);

#endif