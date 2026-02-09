/*===========================================================================
 * AUTOSAR-like Standard Types
 * Description: 프로젝트 공통 표준 타입 정의 (단일 출처)
 * Note: BSW, RTE, SWC 모두 이 헤더를 통해 Std_ReturnType 사용
 *===========================================================================*/
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

/*===========================================================================
 * Standard Return Type
 *===========================================================================*/
typedef uint8_t Std_ReturnType;

#define E_OK ((Std_ReturnType)0u)
#define E_NOT_OK ((Std_ReturnType)1u)

/*===========================================================================
 * Boolean (AUTOSAR-like)
 *===========================================================================*/
#ifndef TRUE
#define TRUE ((uint8_t)1u)
#endif

#ifndef FALSE
#define FALSE ((uint8_t)0u)
#endif

/*===========================================================================
 * Critical Section (임계구역)
 * APP_InterruptDisable/Enable 기존 프로젝트 API 활용
 *===========================================================================*/
extern void APP_InterruptDisable(void);
extern void APP_InterruptEnable(void);

#define SchM_Enter_Exclusive_Area() APP_InterruptDisable()
#define SchM_Exit_Exclusive_Area() APP_InterruptEnable()

#endif /* STD_TYPES_H */
