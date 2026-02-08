# SWC (Software Component) Layer

이 폴더는 AUTOSAR Application Layer의 SWC들을 포함합니다.

## 구조
- 각 SWC는 독립적인 서브폴더로 구성
- RTE를 통해서만 BSW/MCAL과 통신
- 직접적인 하드웨어 접근 금지

## 현재 SWC
- SwcA: 기존 레거시 로직을 래핑하는 임시 SWC (asw/swc/SwcA_Runnable.c)