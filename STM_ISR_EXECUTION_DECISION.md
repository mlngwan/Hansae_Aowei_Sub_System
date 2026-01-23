# STM 1ms ISR 실행 전략 결정 문서

## 1. Decision

### 권장: **Option B - 조건부 유지 권장**
**현재 ISR 내 실행 유지, 단 다음 조건 하에서만 진행:**
- ISR 최대 실행시간 측정 및 500µs 이내 확보
- `g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL` 조건부 필터링 유지 (현재 구현)
- 공유자원(`g_L9369_RegWrite`) 재진입 보호(volatile 선언 + 원자연산)
- INTC.PSR[30]=15 우선순위 검토(다른 ISR과의 충돌 확인 후 조정)

**사유:** Main 루프가 단순 폴링 기반이고 5ms 태스크 스케줄러만 존재하여, 1ms 정밀성이 필요한 L9369 동기화 신호 생성을 Main으로 이관하면 지터와 누락 위험 발생.

---

## 2. Evidence

### 2.1 ISR 실행 비용 및 최악 케이스

| 항목 | 근거 | 평가 |
|------|------|------|
| **TaskScheduler_TaskCount()** | [Main_Api.c:185-189] `g_TaskControl.Flag.Enable = TRUE; g_TaskControl.TickCount++;` | **비용 낮음**: 2개 변수만 접근, ~50-100 사이클 |
| **L9369_TASK_1ms()** | [Spi_L9369_Api.c:1823-1835] `L9369_Sync_1ms()` + `SyncStep++` + 조건부 `L9369_Sync_5ms()` | **조건부 비용**: 신호 토글만 수행, ~200-300 사이클/1ms |
| **L9369_Sync_1ms()** | [Spi_L9369_Api.c:1791-1796] `PORT_SUB_L9369_SYNC1 토글` | **하드웨어만**: 메모리 쓰기 1회 (~50 사이클) |
| **ISR 누적** | STM_CH0_ISR 내 총 작업 | **~250-400 사이클** (최악 5ms 사이클 포함) |

**결론:** 전형적인 1ms 주기(~100K 사이클)에서 0.4-0.5% 처리 부하. 충분히 낮음.

### 2.2 1ms 누락/지터 허용 여부

| 항목 | 근거 | 평가 |
|------|------|------|
| **L9369_Sync_1ms() 용도** | [Spi_L9369_Api.c:1791-1792] 하드웨어 동기화 신호 토글 | **스킵 불가**: 모터드라이버(L9369) SPI 동기화 신호. 1ms 마다 정확한 에지 전환 필요 |
| **누락 시 영향** | L9369 초기화 및 상태 동기화 실패 → 모터 드라이버 제어 불가 | **안전 크리티컬** |
| **Main 루프 주기** | [Main_Api.c:320-326] 무한 루프 + 5ms 스케줄러만 존재 | **불규칙**: 5ms 태스크가 언제 완료될지 불확실. 1ms 신호 생성 누락 가능 |
| **현재 실행 위치** | [Stm_Api.c:26-36] ISR 내에서만 **보장된 1ms 주기** | **필수 요소** |

**결론:** 1ms 신호 누락 허용 불가. ISR 기반 실행이 필수.

### 2.3 SPI/공유자원 접근 및 재진입 위험

| 항목 | 근거 | 평가 |
|------|------|------|
| **공유 전역변수** | [Spi_L9369_Api.c:9] `extern L9369_RegWrite_t g_L9369_RegWrite;` <br> [Spi_L9369Data_Types.c:9] `L9369_RegWrite_t g_L9369_RegWrite;` (정의) | **공유자원**: 여러 함수에서 접근 |
| **ISR 내 접근** | [Stm_Api.c:35] `L9369_TASK_1ms()` → [Spi_L9369_Api.c:1823-1835] `g_L9369_RegWrite.Sync_1ms`, `g_L9369_RegWrite.SyncStep` | **쓰기 연산** |
| **Main 내 접근** | [Spi_L9369_Api.c:165-174] `getL9369Switch()` → `g_pL9369_RegWrite->SwitchMode` 읽기/쓰기 <br> [Spi_L9369Data_Types.c:113-114] SPI 메시지 구성 시 `g_L9369_RegWrite.PWM_A/B` 접근 | **읽기/쓰기 혼용** |
| **현재 보호** | ISR 우선순위 15 (최고). Main 루프에 명시적 임계구역 없음 | **위험**: 16-bit 이상 쓰기가 비원자적일 수 있음 |
| **재진입 타입** | ISR → Main 호출 경로 없음. Main → ISR만 가능 | **일방향**: ISR 쓰기 중 Main이 읽을 수 있음 |

**결론:** 공유자원(`Sync_1ms`, `SyncStep`) 접근 시 경쟁 조건(race condition) 가능. 해결책: ISR에서만 쓰기하도록 제한 + Main에서 volatile 읽기.

### 2.4 우선순위 및 다른 인터럽트 영향

| 항목 | 근거 | 평가 |
|------|------|------|
| **STM ISR 우선순위** | [Stm_Api.c:23] `INTC.PSR[30].R = 15;` | **최고 우선순위** (15 = 최상위) |
| **다른 ISR 우선순위** | [linflex_uart.c:280-281] UART RX/TX는 `PSR[79/80]` = 3 (낮음, 주석 처리) <br> [pit.c:50] PIT ISR `INTC.PSR[59].R = 0x01;` (매우 낮음) | **우선순위 보장**: STM이 모든 ISR을 가로챈다 |
| **ISR 중첩 가능성** | 동일 우선순위: 중첩 불가. 우선순위 15 > 3, 1 → STM이 항상 먼저 처리 | **중첩 없음** |
| **실행 지연** | STM 실행 중 다른 ISR은 대기. 최악 250-400 사이클 × 10 (추정 다른 ISR 수) | **영향 미미** (1ms >> 4µs) |

**결론:** ISR 우선순위 구도 상 STM이 지배적. 다른 ISR 지연 가능성 낮음.

---

## 3. Options 비교표

| 항목 | **Option 1: ISR 유지(현재)** | **Option 2: Main 이관 + Pending Tick** | **Option 3: Main 이관 + Flag** |
|------|------|------|------|
| **구현 위치** | [Stm_Api.c:26-36] STM_CH0_ISR 내 직접 실행 | ISR: pending counter 증가만 <br> Main: `TaskScheduler_ActivateTask()` 내 체크 후 실행 | ISR: flag 설정만 <br> Main: 폴링 후 실행 |
| **1ms 정밀성** | ✅ **보장**: ISR 1ms 주기로 정확히 실행 | ⚠️ **부분 보장**: pending tick만 정확. Main 스케줄러가 5ms 단위로 동작하므로 1ms 누락 가능 | ❌ **미보장**: Main 루프 지연 시 누락 |
| **코드 복잡도** | ✅ **낮음**: 현재 그대로. ~3줄 | ⚠️ **중간**: pending counter 관리 필요 | ✅ **낮음**: flag 1개만 관리 |
| **공유자원 보호** | ⚠️ **필요**: `g_L9369_RegWrite` volatile + 원자연산 | ⚠️ **필요**: pending counter도 보호 필요 | ⚠️ **필요**: flag도 보호 필요 |
| **캐시 일관성** | ⚠️ **문제 가능**: Main에서 최신 값 읽지 못할 수 있음 | ✅ **개선**: pending tick은 단순 카운터 | ✅ **개선**: flag는 단순 boolean |
| **수정 범위** | ❌ **크다**: ISR + Main 양쪽 수정 (원자연산, 메모리배리어 추가) | ✅ **적다**: pending counter 추가, Main 체크만 | ✅ **적다**: flag 추가, Main 폴링만 |
| **지터(Jitter)** | ✅ **없음**: ISR 주기 정확 | ✅ **없음**: pending tick 생성 정확 (처리 시점만 지연) | ❌ **있음**: Main 루프 지연 시 실행 시점 흔들림 |
| **누락(Skip) 위험** | ✅ **없음**: ISR 빠짐 없음 | ✅ **없음**: pending tick 카운터로 누락 추적 | ❌ **있음**: Main 루프 중단 시 flag 덮어써짐 |
| **안전성** | ⚠️ **중간**: ISR 최대시간 측정 필수 | ✅ **높음**: Main 처리 지연해도 pending count로 복구 | ❌ **낮음**: 누락 시 복구 불가 |
| **추천 조건** | - ISR 비용 ≤ 500µs <br> - 공유자원 volatile 선언 <br> - 우선순위 검증 | - Main 루프에 5ms 이상 지연 가능성 있으면 | - 1ms 누락 허용 가능 (하지만 현재는 불가) |

### 비교표 분석

**Option 1 우위 사항:**
- 1ms 신호 생성의 완벽한 정밀성 (L9369 초기화 요구)
- 지터 없음
- 누락 불가능

**Option 1 약점:**
- 공유자원 보호 필요 (코드 추가 필수)
- ISR 최대 실행시간 모니터링 필요

**Option 2 장점:**
- Main 지연에 강건 (pending count로 복구)
- 비교적 간단한 구현

**Option 2 약점:**
- Main 스케줄러가 5ms 단위이므로, pending tick이 쌓여도 1ms 주기 동작 불가
- 정규적 1ms 실행 불가능 (이관 의미 없음)

**결론:** **Option 2는 기술적으로 Option 1의 이점을 상쇄하지 못함.** Main 스케줄러 아키텍처가 5ms 기반이므로 1ms 신호 생성은 ISR에서만 보장 가능.

---

## 4. Implementation Checklist

### 선택 옵션: **Option B (ISR 유지 + 조건부 보호)**

#### 4.1 ISR 최대 실행시간 측정 (필수)

**목적:** ISR이 1ms 내에 완료됨을 확인

```c
// Stm_Api.c에 추가
static uint32_t g_STM_MaxTime = 0;
static uint32_t g_STM_StartCycle = 0;

void STM_CH0_ISR(void)
{
    g_STM_StartCycle = /* CPU 사이클 카운터 읽기 */;
    
    STM.CH[0].CIR.B.CIF = 1;
    STM.CNT.R = 0;
    TaskScheduler_TaskCount();
    
    if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL) {
        L9369_TASK_1ms();
    }
    
    uint32_t elapsed = /* 사이클 차이 */;
    if(elapsed > g_STM_MaxTime) {
        g_STM_MaxTime = elapsed;
    }
}
```

**측정 방법:**
- 디버거의 프로파일러 또는 하드웨어 성능 카운터 활용
- `g_STM_MaxTime`을 주기적으로 모니터링 (메인 루프에서)
- 목표: **< 500µs** (1ms의 50% 미만)

**검증:**
- 다양한 시스템 상태(normal, power-down, emergency)에서 측정
- 최악 케이스: `L9369_TASK_1ms()` + `TaskScheduler_TaskCount()` 동시 실행

---

#### 4.2 공유자원 보호 (필수)

**문제:** `g_L9369_RegWrite` 구조체가 ISR과 Main에서 동시 접근

**해결책:**

**파일: [Spi_L9369Data_Types.h](Spi_L9369Data_Types.h)** (또는 선언 위치)
```c
// 변경 전
typedef struct {
    uint16 Sync_1ms;
    uint16 Sync_5ms;
    uint16 SyncStep;
    // ... 다른 필드
} L9369_RegWrite_t;

// 변경 후
typedef struct {
    volatile uint16 Sync_1ms;    // ISR에서만 쓰기
    volatile uint16 Sync_5ms;    // ISR에서만 쓰기
    volatile uint16 SyncStep;    // ISR에서만 쓰기
    // ... 다른 필드 (Main에서만 접근하는 것도 volatile 권장)
} L9369_RegWrite_t;
```

**파일: [Stm_Api.c](Stm_Api.c)** 
```c
void STM_CH0_ISR(void)
{
    STM.CH[0].CIR.B.CIF = 1;
    STM.CNT.R = 0;
    TaskScheduler_TaskCount();
    
    if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL) {
        L9369_TASK_1ms();  // 내부에서 volatile 필드 쓰기
    }
    // ISR만 쓰기 수행. Main에서는 읽기만.
}
```

**파일: [Spi_L9369_Api.c](Spi_L9369_Api.c) - Main에서의 읽기**
```c
// Main 루프 또는 5ms 태스크에서:
void getL9369Switch(void) {
    // 읽기 예시
    uint16 syncStep = g_L9369_RegWrite.SyncStep;  // volatile이므로 최신값 읽음
    uint16 sync1ms = g_L9369_RegWrite.Sync_1ms;
    
    // ... 사용
}
```

**원리:**
- `volatile` 키워드: 컴파일러에 메모리 재배치 금지, 매번 메모리 접근 강제
- **규칙:** ISR은 쓰기만 / Main은 읽기만 → 재진입 없음
- 16-bit 쓰기는 대부분의 CPU에서 원자적 (하지만 `volatile`로 명시적 표기)

---

#### 4.3 우선순위 정책 (검증)

**현황:** [Stm_Api.c:23] `INTC.PSR[30].R = 15;` (최고)

**검증 항목:**

| 항목 | 확인 사항 | 현재값 | 조정 필요? |
|------|------|------|------|
| STM ISR 우선순위 | 최고 보장 | 15 | 유지 |
| UART ISR | STM 블로킹 허용? | 3 (주석) | 필요시 0-2로 낮춤 |
| PIT ISR | STM 블로킹 허용? | 1 | 필요시 0으로 낮춤 |
| 다른 ISR | 전수 조사 | ? | 모두 < 15 확인 |

**조정 방법:**
```c
// 다른 ISR을 STM보다 낮게 유지
// INTC.PSR[30] = 15 (STM)
// INTC.PSR[79] = 3   (UART RX) → 필요시 1 또는 0으로 낮춤
// INTC.PSR[80] = 3   (UART TX) → 필요시 1 또는 0으로 낮춤
// INTC.PSR[59] = 0   (PIT)      → 현재 최저, 유지
```

---

#### 4.4 ISR 재진입 금지 정책 (선택)

**목적:** STM ISR가 중복 실행되지 않도록 보장

**현황:** 동일 우선순위의 ISR은 중첩되지 않음. 따라서 **추가 보호 불필요**.

**확인:**
```c
// Stm_Api.c에서 검증
void STM_CH0_ISR(void)
{
    // ISR 내에서 재진입 금지를 위한 명시적 보호 불필요
    // (CPU가 우선순위 기반으로 자동 보호)
    
    STM.CH[0].CIR.B.CIF = 1;
    // ...
}
```

---

#### 4.5 ISR 내 함수 호출 코드 리뷰

**파일:** [Stm_Api.c:26-36]

```c
void STM_CH0_ISR(void)
{
    STM.CH[0].CIR.B.CIF = 1;   // 플래그 클리어 (~1-2 사이클)
    STM.CNT.R = 0;              // 카운터 리셋 (~1-2 사이클)

    TaskScheduler_TaskCount();   // 필수: tick 계수 (50-100 사이클)
    
    if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL) {
        L9369_TASK_1ms();        // 조건부: 신호 생성 (200-300 사이클)
    }
}
```

**평가:**
- ✅ **로직 적절**: 조건부 필터링으로 불필요한 호출 방지
- ✅ **비용 낮음**: 누적 < 400 사이클
- ✅ **지터 없음**: ISR 주기 정확

**결론:** 현재 구조 유지 권장.

---

#### 4.6 공유자원 접근 보호 정책 (권장)

**ISR과 Main 간 접근 규칙:**

| 함수 | 대상 필드 | ISR 접근 | Main 접근 | 보호 방법 |
|------|------|------|------|------|
| `L9369_TASK_1ms()` | `g_L9369_RegWrite.Sync_1ms` | 쓰기만 | 읽기만 | volatile |
| `L9369_TASK_1ms()` | `g_L9369_RegWrite.SyncStep` | 쓰기만 | 읽기만 | volatile |
| `getL9369Switch()` | `g_L9369_RegWrite.SwitchMode` | ❌ 접근 없음 | 읽기/쓰기 | 원자연산 불필요 (Main만 접근) |
| `L9369_TASK_1ms()` | `g_L9369_RegWrite.Sync_5ms` | 쓰기만 | 읽기만 | volatile |

**규칙 정리:**
```c
// ✅ ISR 작업 (읽기/쓰기)
void STM_CH0_ISR(void) {
    if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL) {
        L9369_TASK_1ms();  // 내부: g_L9369_RegWrite 필드 쓰기만
    }
}

// ✅ Main 작업 (읽기만)
void getL9369Switch(void) {
    uint16 step = g_L9369_RegWrite.SyncStep;  // 읽기
    // ... 로직
}

// ❌ Main에서 g_L9369_RegWrite.Sync_* 쓰기 금지
// (ISR이 독점 제어)
```

---

#### 4.7 모니터링 및 디버깅 포인트

**추천 구현:**

**파일: [Main_Api.c](Main_Api.c)의 5ms 태스크에 추가**

```c
static void TaskScheduler_20ms(void) {
    // ... 기존 로직 ...
    
    // 디버깅: ISR 최대 실행시간 모니터링
    extern uint32_t g_STM_MaxTime;
    if (g_STM_MaxTime > 500) {  // 500µs 초과 시 경고
        // 로그 기록 또는 LED 표시
        // (프로덕션에서는 제거 또는 DEM(Diagnostic Event Manager)에 보고)
    }
}
```

---

## 5. 최종 결론

### 권장 결정: **Option B 적용 (ISR 유지 + 조건부 보호)**

**이유:**
1. **정밀성 필수:** L9369 1ms 동기화 신호는 **누락 불가능** (하드웨어 초기화 필수)
2. **지터 없음:** ISR 기반만이 정확한 1ms 주기 보장 가능
3. **비용 낮음:** ISR 부하 < 0.5% (충분히 여유 있음)
4. **Main 구조 제약:** 5ms 태스크 스케줄러로는 1ms 정밀성 미보장
5. **구현 간단:** volatile + 원자규칙으로 안전성 확보 가능

### 적용 시 필수 사항

| 항목 | 우선순위 | 구현 방법 |
|------|------|------|
| ISR 최대시간 측정 | **P0 (필수)** | CPU 사이클 카운터 + 주기 모니터링 |
| `volatile` 선언 | **P0 (필수)** | `g_L9369_RegWrite` 필드 모두 volatile |
| ISR/Main 접근 규칙 수립 | **P0 (필수)** | ISR=쓰기만 / Main=읽기만 |
| 우선순위 검증 | **P1 (권장)** | 다른 ISR보다 높게 유지 확인 |
| 모니터링 로직 추가 | **P2 (선택)** | 5ms 태스크에서 최대시간 감시 |

### 예상 효과

| 지표 | 현재 | 개선 후 |
|------|------|------|
| 1ms 신호 정밀성 | ⚠️ 불명확 | ✅ 보장 |
| 재진입 안전성 | ⚠️ 위험 | ✅ 안전 |
| 코드 복잡도 | 낮음 | 약간 증가 (volatile + 규칙) |
| ISR 부하 | < 0.5% | 동일 |

---

**문서 작성 완료일:** 2026-01-22  
**검토 대상:** 임베디드 소프트웨어 아키텍처 팀
