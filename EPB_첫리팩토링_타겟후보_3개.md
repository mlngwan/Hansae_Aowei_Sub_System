# EPB 프로젝트 첫 리팩토링 타겟 기능 후보 3개

## 후보 기능 분석표

| 항목 | 후보 1: ADC 센서 데이터 처리 | 후보 2: GPIO 포트 상태 체크 | 후보 3: L9369 동기화 신호 생성 |
|------|------------------------------|----------------------------|-------------------------------|
| **기능명** | ADC 센서 값 읽기 및 물리값 변환 | IGN 스위치 및 GPIO 상태 모니터링 | 모터 드라이버 동기화 신호 토글 |
| **위치** | `EPB/Peripheral/ADC/Adc_Api.c` / `CheckAdcStatus()` | `EPB/Application/Port_Api.c` / `CheckPortStatus()` | `EPB/Application/MotorControl/Spi_L9369_Api.c` / `L9369_TASK_1ms()` |
| **호출 주기** | 5ms (모든 시스템 상태에서) | 5ms (모든 시스템 상태에서) | 1ms (ISR 내, MtDrvStatus == NORMAL일 때만) |
| **Hardware Touch** | `ADC.MCR.R`, `ADC.CDR[ch].R`, `ADC.NCMR[].R` 직접 접근 | `PORT_SUB_L9369_CTRL`, `PORT_SUB_EPB_SBC_FAULT` GPIO 매크로 | `PORT_SUB_L9369_SYNC1` GPIO 매크로 |
| **g_MAIN R/W** | Read: 없음 / Write: 없음 | Read: 없음 / Write: 없음 | Read: `SystemStatus` / Write: 없음 |
| **g_TaskControl R/W** | Read: 없음 / Write: 없음 | Read: 없음 / Write: 없음 | Read: 없음 / Write: 없음 |
| **기타 전역 R/W** | Read: 없음 / Write: `g_ADC` 구조체 전체 | Read: `g_ADC.Physical.Power.IGN`, `g_TC277.fMainfault` / Write: `g_PORT` 구조체 | Read: 없음 / Write: `g_L9369_RegWrite.Sync_1ms`, `g_L9369_RegWrite.SyncStep` |
| **난이도** | **낮음** | **중간** | **높음** |
| **난이도 이유** | 단순한 ADC 레지스터 읽기와 수학 연산만 수행. 하드웨어 의존성 명확하고 부작용 없음 | IGN 상태에 따른 시스템 전원 제어 함수 호출. 다른 모듈과의 연동 존재 | 1ms ISR 내 실행으로 타이밍 크리티컬. 모터 드라이버와 직접 연결된 안전 기능 |
| **검증 포인트** | `g_ADC.Raw.*` 값 확인, `g_ADC.Physical.*` 물리값 변환 결과, `SSMInput.SSMInVoltage` 전달 확인 | `g_PORT.IgnOffCount` 카운터, `SetSystemOn/Off/Down()` 호출 로그, IGN 상태 변화 추적 | `g_L9369_RegWrite.Sync_1ms` 토글 상태, `PORT_SUB_L9369_SYNC1` GPIO 출력, `SyncStep` 카운터 |

## 상세 분석

### 후보 1: ADC 센서 데이터 처리 (권장도: 높음)

**현재 구현**:
```c
void CheckAdcStatus(void) {
    GetRawData();    // ADC 레지스터 직접 읽기
    SetLogicData();  // 물리값 변환 및 전역 변수 설정
}
```

**Hardware Touch 상세**:
- `ADC.MCR.B.NSTART = 1` (변환 시작)
- `ADC.CDR[adc_ch].R & 0x000003FF` (데이터 읽기)
- `ADC.NCMR[0/1/2].R` (채널 선택)

**Wiring 변경 대상**:
- ADC 레지스터 직접 접근 → MCAL ADC 드라이버 호출
- `g_ADC` 전역 구조체 직접 쓰기 → 인터페이스 함수 통한 데이터 전달

**ASW Intent 불변 요소**:
- UBB, IGN, UBVR 전압 측정 로직
- 물리값 변환 수식 (`ConvertToPhysicalBatVolt()`)
- 모터 전압/전류 계산 알고리즘

### 후보 2: GPIO 포트 상태 체크 (권장도: 중간)

**현재 구현**:
```c
void CheckPortStatus(void) {
    if(g_ADC.Physical.Power.IGN > IGN_ON) {
        SetSystemOn();  // 시스템 전원 관리 함수 호출
    } else {
        SetSystemDown();
        // IGN OFF 타이머 처리
    }
    // L9369 제어 신호 설정
    // SBC 고장 상태 읽기
}
```

**Hardware Touch 상세**:
- `PORT_SUB_L9369_CTRL` GPIO 출력 제어
- `PORT_SUB_EPB_SBC_FAULT` GPIO 입력 읽기

**Wiring 변경 대상**:
- GPIO 매크로 직접 접근 → MCAL Port 드라이버 호출
- `g_PORT`, `g_TC277` 전역 변수 직접 접근 → 인터페이스 함수

**ASW Intent 불변 요소**:
- IGN ON/OFF 판단 로직 (IGN_ON 임계값)
- IGN OFF 지연 타이머 (IGN_OFF_TIME)
- 시스템 전원 상태 전환 알고리즘

### 후보 3: L9369 동기화 신호 생성 (권장도: 낮음)

**현재 구현**:
```c
void L9369_TASK_1ms(void) {
    if(g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL) {
        L9369_Sync_1ms();  // GPIO 토글
        g_L9369_RegWrite.SyncStep++;
        if(g_L9369_RegWrite.SyncStep >= 5) {
            g_L9369_RegWrite.SyncStep = 0;
            L9369_Sync_5ms();
        }
    }
}
```

**Hardware Touch 상세**:
- `PORT_SUB_L9369_SYNC1` GPIO 출력 토글
- `PORT_SUB_L9369_SYNC5` GPIO 출력 (5ms 주기)

**Wiring 변경 대상**:
- GPIO 매크로 직접 접근 → MCAL Port 드라이버 호출
- `g_L9369_RegWrite` 전역 구조체 직접 접근 → CDD 인터페이스

**ASW Intent 불변 요소**:
- 1ms 동기화 신호 토글 주기
- 5ms 주기 동기화 신호 생성 로직
- SyncStep 카운터 기반 타이밍 제어

**위험 요소**:
- 1ms ISR 내 실행으로 타이밍 지연 시 모터 제어 영향
- L9369 모터 드라이버와 직접 연결된 안전 크리티컬 기능
- 시스템 상태 의존성 (`g_MAIN.SystemStatus` 체크)

## 권장 순서

1. **후보 1 (ADC 센서 데이터 처리)**: 가장 독립적이고 부작용 위험 낮음
2. **후보 2 (GPIO 포트 상태 체크)**: 시스템 전원 관리와 연동되어 있어 중간 난이도
3. **후보 3 (L9369 동기화 신호)**: ISR 내 실행 및 안전 크리티컬로 가장 위험

**근거**: 각 함수의 코드 분석 및 하드웨어 의존성, 전역 변수 사용 패턴, 호출 주기 분석