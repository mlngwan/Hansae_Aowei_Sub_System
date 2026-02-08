#include "Rte_SwcA.h"

/* 지금 단계(MVP)에서는 Call을 "스텁"으로 두고 시작 가능 */
Std_ReturnType Rte_Call_SwcA_GetTimeMs(uint32_t* ms)
{
    if (ms == 0) return E_NOT_OK;
    *ms = 0u; /* TODO: 나중에 Bsw_Time_GetMs()로 라우팅 */
    return E_OK;
}