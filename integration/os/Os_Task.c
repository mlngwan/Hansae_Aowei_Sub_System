#include "Os_Task.h"
#include "Rte_Main.h"

/* OS Task 엔트리 포인트들 - RTE로 라우팅만 */

void OsTask_1ms(void)
{
    Rte_Task_1ms();
}

void OsTask_5ms(void)
{
    Rte_Task_5ms();
}

void OsTask_10ms(void)
{
    Rte_Task_10ms();
}

void OsTask_20ms(void)
{
    Rte_Task_20ms();
}