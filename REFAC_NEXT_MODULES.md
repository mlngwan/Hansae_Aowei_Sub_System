# 다음 리팩토링 모듈 선정 (백로그)

## 1. Selection Criteria

| 기준 | 설명 | 우선순위 가중치 |
|------|------|------|
| **ISR-Safety 위반** | ISR 외부에서 실행되어야 할 로직이 ISR에서 실행되거나 ISR과 Main 간 경쟁 조건 위험 | **매우 높음 (P0)** |
| **주기 의존성 복잡** | 다양한 주기(1ms, 5ms, 10ms, 20ms)가 혼재되거나 타이밍 불확실 | **높음 (P1)** |
| **레이어 침범** | MCAL/BSW/FS/ASW 경계 모호, 상위 로직이 하위 계층에 침범 | **높음 (P1)** |
| **공유자원 접근** | 전역 상태 변수의 복잡한 갱신, 동기화 부족 | **높음 (P1)** |
| **파급도 vs 영향** | 변경 시 영향 범위 vs 개선 효과 (좋은 ROI 우선) | **중간 (P2)** |

---

## 2. Findings (Top Candidates)

### **후보 1: Port_Api (레이어 경계 침범, 최우선)**

**증거:**
- [Port_Api.c:1-20] MCAL/BSW 경계 혼재: `Mcal_Port_ConfigurePads()`, `Fs_SystemState_t`, `Fs_MotorCtrlCmd_t` 정의
- [Port_Api.c:5-11] 공유자원 혼재: `extern ADC_t g_ADC`, `extern L9369_RegWrite_t g_L9369_RegWrite`, `extern TC277_t g_TC277` 
- [Main_Api.c:315] 초기화 순서: `STMInitialize()` 전 `PORTInitialize()` 호출 → 타이밍 의존성

**문제 요약:**
- MCAL 계층(GPIO/PCR 설정)과 FS 계층(시스템 상태 정의)이 동일 파일에 혼재
- 다른 모듈(ADC, L9369, TC277)의 전역변수 직접 참조 → 느슨한 결합 위반
- 핀 설정과 상태 정의가 분리되지 않아 유지보수 어려움

**기대 효과:**
- MCAL 계층 순수화 (GPIO 설정만 담당)
- FS 계층 도출 (시스템 상태/신호 정의로 분리)
- 모듈 간 의존성 명확화

**리스크:**
- 모든 초기화 함수가 영향 (중간 영향)
- 외부 모듈과의 인터페이스 재정의 필요

---

### **후보 2: Diagnostic_Task (10ms 주기, 주기성 불명확)**

**증거:**
- [Main_Api.c:140] `Task_Diagnostic()` 호출이 `TaskScheduler_TaskCalculation()` 내에 있음
- [Main_Api.c:176-182] 내용: 단순 조건부 카운터 리셋만 수행 (실질적 기능 없음)
- [Main_Api.c:218] 호출: `Diagnostic_Task_10ms()` in `TaskScheduler_10ms()` → 실제 진단 작업이 별도 함수에 있음

**문제 요약:**
- 진단 관련 함수가 3개 이상 산재(Task_Diagnostic, Diagnostic_Task_10ms, Diagnostics_CAN.h)
- 10ms 주기 명시적이지만 실제 작업 로직 위치 불명확
- CAN 진단과 로컬 진단 분리 필요

**기대 효과:**
- 진단 태스크 통합 (BSW 계층으로 정리)
- 10ms 주기 명확화
- CAN 진단과 시스템 진단 분리

**리스크:**
- 낮음 (기능 분석 필요하나 영향 최소)

---

### **후보 3: L9369_TASK_1ms + CheckL9369RegStatus 분리 (주기성 혼재)**

**증거:**
- [Stm_Api.c:35] ISR 내: `L9369_TASK_1ms()` (동기화 신호 생성만)
- [Main_Api.c:271] 5ms 태스크 내: `CheckL9369RegStatus(SPI_L9369_RUN)` (레지스터 폴링)
- [Spi_L9369_Api.c:900] `CheckL9369RegStatus()`: 12 스텝 폴링, 각 단계마다 SPI Read/Write
- [Spi_L9369_Api.c:1823-1835] `L9369_TASK_1ms()`: 단순 신호 토글 + 5ms 조건부 처리

**문제 요약:**
- 1ms ISR과 5ms Main 간 L9369 제어 로직 분산
- ISR 내 신호 생성 vs Main 내 상태 폴링으로 역할 혼재
- 레지스터 폴링 상태 머신(Step 0-11)이 일관성 없게 분산

**기대 효과:**
- ISR: 1ms 신호 생성만 순수화 (CDD-like 예외로 명확화)
- Main: L9369 상태 폴링 통합 (5ms 또는 10ms 태스크로 통합)
- 책임 분리 명확화

**리스크:**
- 중간 (L9369 초기화 및 상태 관리와 연동)

---

### **후보 4: TC277 + CANSBC 제어 통합 (공유자원 경쟁)**

**증거:**
- [Main_Api.c:202] 20ms: `CheckTC277Status()`
- [Main_Api.c:198,233,266] 다양한 위치에서 `CheckCANSBCRegStatus()` 호출 (Shutdown, 5ms, 5ms 다시)
- [Spi_TC277_Api.c:18-40] TC277_RegStatus(): ADC 값 읽기 `g_ADC.Physical.Power.*` 직접 접근
- [Spi_TC277_Api.c:46-62] TC277_Response(): DSPI 레지스터 직접 조작

**문제 요약:**
- CAN SBC (TLE9461) 제어와 CAN 트랜시버(TC277) 상태 확인이 분리되지 않음
- ADC, TC277, CANSBC 간 공유자원 복잡한 접근 (g_ADC, g_TC277, SPI 경합)
- 호출 빈도 불일치 (20ms vs 5ms 혼재)

**기대 효과:**
- CAN 계층 통합 (COM 계층으로 정리)
- SBC 제어와 트랜시버 상태 분리
- 공유자원 접근 규칙 명확화

**리스크:**
- 높음 (CAN 통신과 진단 전체에 영향)

---

### **후보 5: 초기화 순서 및 상태 머신 정규화 (전반적 아키텍처)**

**증거:**
- [Main_Api.c:300-326] `SubEpbInitialize()`: 초기화 순서: BoardDisableInterrupts → ... → STMInitialize → ADCInitialize → SpiInitialize → CanInitialize → BoardEnableInterrupts
- [Main_Api.c:254-259] SystemStatus == INIT 상태에서 `TLE9461Init()`, `CheckWakeupStatus()`, `CheckPortStatus()`, `CheckAdcStatus()` 혼재
- [Main_Api.c:263-278] SystemStatus == NORMAL 상태에서 무한 폴링 루프 (`if(g_TC277.fMainfault == TRUE)...`)

**문제 요약:**
- 초기화와 Runtime 상태가 분리되지 않음
- 상태 머신(INIT→NORMAL→POWER_DOWN→EMERGENCY) 전환 로직이 메인 루프에 산재
- 각 상태별 태스크 집합이 명확하지 않음

**기대 효과:**
- 상태별 태스크 그룹화 (BSW 스케줄러 정규화)
- 초기화 단계 정렬 (의존성 명시)
- Main 루프 단순화

**리스크:**
- 매우 높음 (전체 아키텍처 재구성)
- P3 (장기 목표)

---

## 3. Recommendation

### **1순위 모듈: Port_Api (레이어 경계 정리)**

**선정 근거:**

1. **ISR-Safety 영향도 높음:** Port_Api가 시스템 초기화의 핵심이고, MCAL/FS 혼재로 인해 타이밍 의존성이 발생 중. STM ISR 도입 후 이 점이 더욱 중요해짐.

2. **파급도 vs ROI 최적:** MCAL 계층 순수화는 다른 모듈(ADC, L9369, TC277)의 레이어 정의에 선행 조건. 변경 범위는 `Peripheral` 계층에 국한되나, 효과는 전사적 아키텍처 개선으로 확대.

3. **코드 근거:** [Port_Api.c:1-20] MCAL 정의부터 FS 타입 정의까지 혼재된 구조. 이를 먼저 분리하면 후속 리팩토링(L9369, TC277)이 명확해짐.

**첫 리팩토링 스텝 (최소 변경):**

| # | 작업 | 대상 파일 | 예상 라인 |
|---|------|------|------|
| 1 | Mcal_Port_ConfigurePads() 분리 | Port_Api.c | L40-80 추출 → `Port_Mcal.c` |
| 2 | FS 상태 정의 분리 | Port_Api.c | L18-32 추출 → `PortFs_Types.h` |
| 3 | 공유자원 참조 정리 | Port_Api.c | L5-11 정리 (DI 패턴으로) |
| 4 | Main 초기화 호출 업데이트 | Main_Api.c | L315 변경 불필요 (인터페이스 동일) |

**검증 방법:**
- 컴파일 통과 확인
- 초기화 시퀀스 동작 동일성 확인 (SBC 상태, GPIO 출력)
- 다른 모듈의 Port_Api 의존성 검증

---

## 4. Appendix: STM CDD-like 정책 요약

**STM ISR 예외 정책:**

- **ISR 역할:** 1ms 주기 System Tick 발생 + TaskScheduler 업데이트 + 조건부 L9369 동기화 신호 생성만 수행
- **금지 사항:** ISR 내 SPI 통신, 상태 머신 폴링, CAN 메시지 전송, 상태 추적 로직 추가 금지
- **의존 함수:** `TaskScheduler_TaskCount()` (필수), `L9369_TASK_1ms()` (조건부, MTDRV_STATUS_NORMAL 만)
- **공유자원 규칙:** ISR은 쓰기만, Main은 읽기만 (volatile 선언)
- **격리 범위:** `STM` 모듈(MCAL)은 CDD 예외로 관리, 다른 MCAL/BSW는 AUTOSAR 레이어 준수

---

**문서 작성 완료일:** 2026-01-22  
**검토 대상:** 아키텍처 및 리팩토링 태스크포스
