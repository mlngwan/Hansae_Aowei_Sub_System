#ifndef RTE_TYPE_H
#define RTE_TYPE_H

#include <stdint.h>

typedef uint8_t Std_ReturnType;

#ifndef E_OK
#define E_OK ((Std_ReturnType)0u)
#endif
#ifndef E_NOT_OK
#define E_NOT_OK ((Std_ReturnType)1u)
#endif

#endif