SWC는 RTE만 의존한다(SWC에서 BSW/CDD/MCAL 직접 호출 금지)

의존성: SWC → RTE → BSW/CDD → MCAL

RTE 버퍼는 Rte_Buffers.c에 static, extern 노출 금지

RTE API는 Read/Write/Call만 제공(데이터=Read/Write, 서비스=Call)

BSW는 Get/Set 서비스, MCAL은 레지스터만

SWC 내부는 determine/apply 분리
