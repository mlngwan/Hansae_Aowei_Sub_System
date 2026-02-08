# ADC Data Dictionary

## 1. 채널 매핑 테이블

### MCU 내장 ADC 채널

| 채널 ID | 매크로명 | 물리 핀 | 의미 | RTE Signal명 | 단위 | 타입 | 샘플링 주기 | 가공 주기 |
|---------|----------|---------|------|--------------|------|------|-------------|-----------|
| 32 | ADC_CH_SUB_IGN_MON | PB[8], ANS[0] | 이그니션 신호 | Rte_Adc_Ignition | mV | uint16_t | 5ms | 10ms |
| 9 | ADC_CH_SUB_UBVR_MON | PD[5], GPI[9] | 밸브용 배터리 전압 | Rte_Adc_BatteryValve | mV | uint16_t | 5ms | 10ms |
| 10 | ADC_CH_SUB_UBB_MON | PD[6], GPI[10] | 모터용 배터리 전압 | Rte_Adc_BatteryMotor | mV | uint16_t | 5ms | 10ms |

### L9369 외부 ADC 채널 (SPI 통신)

| 채널 | 의미 | RTE Signal명 | 단위 | 타입 | 샘플링 주기 | 가공 주기 |
|------|------|--------------|------|------|-------------|-----------|
| VSBRIDGE_A | 모터 A 전압 | Rte_Adc_MotorA_Voltage | mV | uint16_t | 5ms | 10ms |
| VSBRIDGE_B | 모터 B 전압 | Rte_Adc_MotorB_Voltage | mV | uint16_t | 5ms | 10ms |
| CS1_A | 모터 A 전류 | Rte_Adc_MotorA_Current | mA | int16_t | 5ms | 10ms |
| CS1_B | 모터 B 전류 | Rte_Adc_MotorB_Current | mA | int16_t | 5ms | 10ms |

## 2. Snapshot 구조체 정의

```c
typedef struct {
    uint16_t BatteryMotor_mV;      /* UBB: 모터용 배터리 전압 */
    uint16_t BatteryValve_mV;      /* UBVR: 밸브용 배터리 전압 */
    uint16_t Ignition_mV;          /* IGN: 이그니션 전압 */
    uint16_t MotorA_Voltage_mV;    /* 모터 A 전압 */
    uint16_t MotorB_Voltage_mV;    /* 모터 B 전압 */
    int16_t  MotorA_Current_mA;    /* 모터 A 전류 */
    int16_t  MotorB_Current_mA;    /* 모터 B 전류 */
    uint8_t  DataValid;            /* 데이터 유효성 플래그 */
} Bsw_Adc_Snapshot_t;
```

## 3. 사용처 분류

| Signal | 제어 | 진단 | CAN 로깅 | SPI 통신 | 우선순위 |
|--------|------|------|----------|----------|----------|
| BatteryMotor | ✓ | ✓ | ✓ | ✓ | HIGH |
| Ignition | ✓ | ✓ | ✓ | ✓ | HIGH |
| BatteryValve | - | ✓ | - | ✓ | MEDIUM |
| MotorA_Voltage | ✓ | ✓ | ✓ | - | HIGH |
| MotorB_Voltage | ✓ | ✓ | ✓ | - | HIGH |
| MotorA_Current | ✓ | ✓ | ✓ | - | HIGH |
| MotorB_Current | ✓ | ✓ | ✓ | - | HIGH |

## 4. 변환 공식

### MCU ADC (10-bit, 3.3V 기준)
- Raw → mV: `(raw * 3300) / 1024`
- 전압 분배 회로 적용: `physical_mV = adc_mV * (R1 + R2) / R2`
- UBB/UBVR/IGN: `physical_mV = adc_mV * (300 + 51) / 51 * 1000` (ADC_REF=1000)

### L9369 ADC (12-bit, SPI)
- VSBRIDGE: `V = 34.2 * (4095 - CODE) / 4096` (A채널)
- VSBRIDGE: `V = 34.2 * CODE / 4096` (B채널)
- CS (전류): `I(A) = ((CODE / 8192 * 410mV) - 205mV) / Rsense(mOhm)`

## 5. 가공 알고리즘

### 5ms 샘플링
- MCAL에서 Raw 값 읽기
- 기본 물리값 변환
- Snapshot 버퍼 갱신

### 10ms 가공
- 6회 샘플링 평균 (Min/Max 제거)
- 이동 평균 필터 (옵션)
- 진단 임계값 체크 (BSW 레벨)

## 6. 접근 패턴

| 계층 | 읽기 방식 | 쓰기 방식 | 주기 |
|------|-----------|-----------|------|
| MCAL | HW 레지스터 직접 | - | On-demand |
| CDD | SPI 통신 | - | On-demand |
| BSW | MCAL/CDD 호출 | Snapshot 갱신 | 5ms/10ms |
| RTE | BSW Snapshot 읽기 | SR Buffer 갱신 | 5ms/10ms |
| SWC | RTE SR Buffer 읽기 | - | 10ms/20ms |
