# 멀티 스레드

## atomic

다른 스레드가 이 연산의 중간에 끼어들어서 연산을 망가뜨리지 못하게 함
Read-Modify-Write 연산 자체가 원자적으로 수행됨
여기까지가 Atomicity(원자성)

---

여기에는 두가지 문제점이 있다.
문제 A - atomic 변수 그 자체

```c++
std::atomic<int> Count;
Count++;
```

Count에 대한 연산이 안전한가?
->Atomicity 문제

```c++
int Data;
std::atomic<bool> Ready;
```

Ready를 통해서 Data까지 다른 스레드에게 제대로 전달할 수 있는가?
-> Synchronization / Memory Visibility문제

> Atomic이라는 사실과 다른 메모리까지 동기화되는 것은 같은 의미가 아니다.

## memory_order_relaxed

```c++
Ready.store(true, std::memory_order_relaxed);
```

> atomic 변수 자체만 원자적으로 처리해달라, 주변 메모리의 순서는 신경쓰지 않도록

예시)

```c++
std::atomic<int> Count{0};
Count.fetch_add(1, std::memory_order_relaxed);
```

이 경우 Count 증가 자체는 원자적
여러 스레드가 동시에 증가시켜도 증가 작업 자체가 유실되지는 않음

```c++
int Data = 0;
std::atomic<bool> Ready = false;

// Thread A
Data = 100;
Ready.store(true, std::memory_order_relaxed);

// Thread B
if (Ready.load(std::memory_order_relaxed))
{
    Use(Data);
}
```

여기서 Ready자체는 문제가 없음
Thread B가 Ready를 읽을때 이상한 상태가 되는 문제는 없음
하지만 Ready == true를 봤다는 사실만으로 Data=100도 반드시 볼 수 있다는 관계를 relaxed만으로는 보장 못함

## release / acquired 가 필요한 이유

```c++
// Thread A
Data = 100;
Ready = true;

// Thread B
if (Ready == true)
{
    // 여기서는 반드시 Data == 100을 보고 싶다.
}
```

이걸 위해서 release / acquired를 사용할 수 있다.

```c++
Data = 100;
Ready.store(true, std::memory_order_release);

if (Ready.load(std::memory_order_acquire))
{
    Use(Data);
}
```

**release**

> Ready = true를 공개하기 전에, 내가 앞에서 해놓은 작업까지 함께 묶어서 공개한다.

**acquired**

> Ready == true를 확인했다면, 그것과 함께 공개된 이전 작업들도 내가 볼 수 있게 한다.

## Happens - Before

> A에서 한 작업이 B에서 관찰되기 전에 완료된 것으로 보장되는 관계

A에서 Data = 100과 B의 Use(Data)가 happens-before 관계에 놓임
B는 A가 앞에서 한 작업을 제대로 관찰할 수 있다.

## Memory Visibility

Thread A에서 Data = 100했다고 했을때 우리가 원하는것은
`B가 나중에 Data를 읽었을 때 그 100을 볼 수 있다` 를 원한다
이걸 Memory Visibility, 즉 다른 스레드에게 메모리 변경이 보이는 문제라고 생각하면 된다.

Atomicity -> 이 연산 자체가 중간에 깨지는가?
Visibility -> 다른 스레드가 내가 한 변경을 제대로 볼 수 있는가?

| 방식                | atomic 변수 자체 | 주변 메모리 전달                      |
| ------------------- | ---------------- | ------------------------------------- |
| `relaxed`           | 원자성 보장      | 보장하지 않음                         |
| `release / acquire` | 원자성 보장      | 특정 관계를 통해 주변 메모리도 동기화 |

## atomic으로 모든 멀티스레드 문제가 해결되는것도 아님

```c++
std::atomic<int> HP;
std::atomic<int> MP;

HP -= 10;
MP += 10;
```

각 줄은 각각 원자적일 수 있다.
근데 게임 규칙상

```
HP -10
+
MP +10
```

이렇게가 한 묶음이라면?

다른 스레드가 봤을때

```
HP는 이미 -10 됐는데
MP는 아직 +10 안 된 상태
```

이런 상황에서 볼 수도 있음

-> Mutex로 해결

# False Sharing

> 서로 다른 변수를 건드리는데도, 우연히 같은 캐시라인에 들어 있어서 코어끼리 캐시를 계속 뺏고 뺏기는 성능 문제

```
하나의 64바이트 Cache Line
┌──────────────────────────────────────────────┐
│ a │ b │        나머지 공간                  │
└──────────────────────────────────────────────┘
  ↑   ↑
Core 1 Core 2
```

Core1이 a를 수정하려면 자신이 가지고 있는 이 캐시 라인을 쓰기 가능한 상태로 만들어야 한다. 그러면 Core2가 가지고 있던 같은 캐시 라인의 복사본은 무효화 될 수 있음

그 다음 Core2가 b를 수정할때

```
Core 1
a 수정
↓
"이 캐시 라인 내가 수정할 거야"
↓
Core 2의 캐시 라인 무효화
```

```
Core 2
b 수정
↓
"이번에는 이 캐시 라인 내가 수정할 거야"
↓
Core 1의 캐시 라인 무효화
```

이게 반복된다. 이때 a와 b를 공유한게 아님 그래서 CPU입장에서는 a,b를 따로 관리하지도 않고 이 64바이트 덩어리를 관리한다 해서
False Sharing, 가짜 공유

```c++
std::atomic<int> count;

Thread 1: count++;
Thread 2: count++;
```

이거는 실제로 같은 데이터를 공유하므로 True Sharing이라고 볼 수 있다.

```c++
std::atomic<int> a;
std::atomic<int> b;

Thread 1: a++;
Thread 2: b++;
```

변수는 다른데 같은 cache line때문에 충돌하면 False Sharing

```
True Sharing

Core1 ──→ count ←── Core2
             ↑
        진짜 같은 변수


False Sharing

Core1 ──→ a │ b ←── Core2
          └───┘
       같은 Cache Line
```

## 방지하는 대표적 방법

다른 cache line에 배치하는 것

개념적으로

```c++
struct alignas(64) Counter
{
    std::atomic<int> value;
};

Counter counter1;
Counter counter2;
```

이렇게 떨어뜨려 놓으면

```c++
Cache Line 1
┌────────────────────────┐
│ counter1               │
└────────────────────────┘

Cache Line 2
┌────────────────────────┐
│ counter2               │
└────────────────────────┘
```

Core 1이 counter1를 수정해도 Core2의 counter2 캐시라인을 건드릴 이유가 없어짐

alignas(64)는 Counter객체의 시작 주소는 최소 64바이트 정렬 조건을 만족해야함을 뜻함

# Branch Prediction

CPU가 분기 결과를 기다리지 않고 다음 명령을 실행하다가, 예측이 틀리면 그 작업을 버리고 다시 시작하는 과정

```c++
if (x > 0)
{
    a = b + c;
}
else
{
    a = d + e;
}
```

cpu는 x > 0 의 결과가 완전히 확정되기 전에 다음 명령을 계속 처리하고 싶어함, 그냥 기다리면 파이프라인이 놀기 때문

```
Branch Prediction
      ↓
Speculative Execution
      ↓
예측 결과 확인
   ↙       ↘
맞음       틀림
 ↓          ↓
계속 실행   Pipeline Flush
               ↓
          올바른 경로부터 다시 실행
```

## Speculative Execution(추측 실행)

예측한 경로의 명령을 실제로 미리 실행, 추측 실행
중요한건 이 결과를 프로그램의 확정된 상태로 반영한 것은 아님
일단 계산 -> 나중에 예측이 맞으면 반영하자

## Pipeline Flush

만약 예측이 틀린경우 잘못된 경로에서 수행한 작업을 버려야 함, 이것을 Pipeline Flush라고 한다.

```
잘못된 경로

A
B
C
↓
전부 폐기

올바른 경로
↓
D
E
F
```

---

CPU는 명령 하나를 처음부터 끝까지 하나씩 처리하지 않고 여러 명령을 겹쳐서 처리함

예시)

```
Instruction 1: Fetch → Decode → Execute → ...
Instruction 2:         Fetch → Decode → Execute → ...
Instruction 3:                 Fetch → Decode → ...
```

근데 분기를 만났다고 매번 결과가 나올 때까지 멈추면 CPU성능이 크게 떨어짐
그래서 기다리지말고 예측해서 가는것을 Branch Prediction + Speculative Execution이라고 함
그 대신 예측 실패의 결과로 Pipeline Flush가 생기는 것

> Branch Prediction으로 갈 길을 예측하고, Speculative Execution으로 미리 실행하고, 예측이 틀리면
> Pipeline Flush로 잘못 실행한 명령들을 버리고 올바른 경로부터 다시 실행한다.

분기가 예측하기 쉬우면 빠르고, 결과가 자꾸 뒤집혀서 예측 실패가 많으면 느려질 수 있다.

## CPU Pipeline

CPU Pipeline은 명령 하나를 여러 단계로 나눠서, 여러 명령을 동시에 겹쳐 처리하는 구조

```
Fetch   →   Decode   →   Execute   →   Retire
가져오기    해석하기      실행하기      결과 확정
```

명령을 하나씩 끝까지 처리한다면

```
명령 A: Fetch → Decode → Execute → Retire

그 다음

명령 B: Fetch → Decode → Execute → Retire
```

이렇게 돼서 CPU내부의 많은 부분이 놀게 됨
그래서 Pipeline을 사용하면

```
시간 →

명령 A   Fetch  Decode Execute Retire
명령 B          Fetch  Decode Execute Retire
명령 C                 Fetch  Decode Execute Retire
명령 D                        Fetch  Decode Execute
```

처럼 동시에 여러 명령을 처리함

# Instruction-Level Parallelism

한 스레드 안의 여러 CPU 명령어를 동시에 겹쳐서 실행하는 것

```c++
a = b + c;
d = e + f;
g = h + i;
```

CPU 명령어 수준에서

```
ADD b, c
ADD e, f
ADD h, i
```

이 세연산은 서로 의존성이 없다.
CPU는 가능하면

```
ADD b, c  ─┐
ADD e, f  ─┼─ 동시에 실행
ADD h, i  ─┘
```

처럼 여러 실행 유닛에 나눠서 처리할 수 있음, 이게 ILP임

> ILP = 하나의 소프트웨어 스레드 안에서 서로 독립적인 여러 명령어를 CPU가 동시에 실행해서 처리량을 높이는 것

```
ILP
→ 한 스레드의 여러 명령을 병렬 처리

Thread-Level Parallelism
→ 여러 스레드를 병렬 처리
```

## Out-of-Order Execution

```
Instruction A
Instruction B
Instruction C
```

순서가 이러한데 B가 A 결과를 기다려야 한다고 치자

```
A: 느린 메모리 접근
B: A 결과 필요
C: 완전히 독립적인 계산
```

순서대로만 실행하면

```
A 기다림
B 기다림
C도 기다림
```

이 된다.
현대 CPU는 보통

```
A실행 중
B -> A때문에 대기

C -> 독립적이네?
-> 먼저 실행
```

할 수 있음 이를 Out-of-Order Execution이라고 함
내부적으로 가능한 명령부터 먼저 처리

```
ILP
-> 이 명령들 중 동시에 실행 가능한게 얼마나 있지?
-> 병령성 자체

Out-of-Order Execution
-> 앞 명령이 막혔으니 독립적인 뒤 명령부터 실행하자
-> ILP를 활용하는 방법
```

## SuperScalar CPU

파이프라인은

```
명령 A: Fetch → Decode → Execute
명령 B:         Fetch → Decode → Execute
명령 C:                 Fetch → Decode
```

서로 다른 처리 단계를 겹치는 것이고

ILP는

```
Execute 단계에서

ADD 1 ──→ ALU 1
ADD 2 ──→ ALU 2
MUL   ──→ MUL Unit
```

처럼 여러 명령 자체를 동시에 실행하는것을 포함함
현대 cpu에서는 보통 한 사이클에 명령 하나만 실행하지 않음
실행 유닛이 충분하고 의존성이 없다면 여러 명령을 한번에 실행 가능

---

SuperScalar란 CPU가 한 사이클에 여러 명령을 동시에 처리 단계에 올릴 수 있다는 뜻

파이프라인이 이렇게 있을때

```
Fetch → Decode → Execute → Retire
```

일반적인 단순 CPU가 한 사이클에 명령 하나만 처리한다면

```
Cycle 1: 명령 A 처리
Cycle 2: 명령 B 처리
Cycle 3: 명령 C 처리
```

이런식인데, SuperScalar CPU는 실행 유닛이 여러개 있어서, 서로 독립적인 명령이라면 같은 사이클에 여러 개를 실행할 수 있음

```
한 사이클 동안

ADD 명령 → ALU 1
SUB 명령 → ALU 2
LOAD 명령 → Load Unit
MUL 명령 → Multiply Unit
```

예를들어 코드가

```c++
a = b + c;
d = e - f;
g = h * i;
```

이렇게 있을때, 서로 의존성이 없으니까 CPU내부에서

```
Cycle N

ADD b,c  → ALU
SUB e,f  → ALU
MUL h,i  → MUL Unit
```

처럼 동시에 실행할 수 있음
여기서 중요한건 명령의 latency랑 throughput은 다름
예를들어 곱셈 하나가 완료되기까지 3사이클이 걸릴수도 있음
그렇다고 3사이클동안 아무것도 못하는게 아니라, 파이프라인화 되어있으면 다음 곱셈도 들어갈 수 있다.

```
Cycle 1: MUL A 시작
Cycle 2: MUL B 시작
Cycle 3: MUL C 시작
Cycle 4: MUL D 시작
```

> SuperScalar = CPU내부에 여러 실행 유닛을 두고, 독립적인 여러 명령을 같은 사이클에 병렬로 처리하는 구조

# 멀티스레드 상태 관리 Snapshot / Double Buffering

## Snapshot

한쪽 스레드에서 게임 상태를 계속 수정하고, 다른쪽에서는 렌더링을 한다고 했을때
서로 다른 상태가 섞일 수 있음

```
Render Thread: Player 위치 읽음

Game Thread: Enemy 위치 수정
Game Thread: Player 위치 수정

Render Thread: Enemy 위치 읽음
```

그러면 Render Thread가 본 상태는

```
Player = 이전 상태
Enemy  = 새로운 상태
```

처럼 서로 다른 시점의 데이터가 섞일 수 있음

Snapshot은 이걸 막으려고 특정 순간의 상태를 하나 만들어 두는것

```
원본 World
    ↓
Snapshot 생성

Snapshot
┌─────────────────────┐
│ Player 상태         │
│ Enemy 상태          │
│ 기타 상태           │
└─────────────────────┘
```

Render Thread는 Snapshot만 읽음
그래서 렌더링 도중 게임 상태가 바뀌어도 렌더러 입장에서는
`내가 보고 있는 데이터는 한 시점의 상태`가 보장됨

> Snapshot의 목적 = 일관된 읽기 상태를 보장하는 것

---

Snapshot에는 복사비용이 있을 수 있다.
상태를 복사하기 때문에 월드가 엄청 크면 매 프레이 전체 복사는 비쌀 수 있음
그래서 실제 구현때는

```
- 전체 복사
- 필요한 부분만 복사
- Copy-on-Write
- immutable data
- versioning
- double buffering
```

등을 사용할 수 있음

## Double Buffering

버퍼를 두개 준비하는 방식

```
Buffer A
Buffer B
```

Read Buffer, Write Buffer 따로두는 방식

```
Frame N

Render Thread
    ↓
Buffer A 읽음

Game Thread
    ↓
Buffer B 작성
```

둘의 작업이 끝나면 버퍼 역할을 바꿈 (Swap)

```
Frame N+1

Render Thread
    ↓
Buffer B 읽음

Game Thread
    ↓
Buffer A 작성
```

---

Double Buffering은 Snapshot을 제공하는 방법 중 하나로 볼 수 있음

### 왜 멀티스레딩에서 좋은가?

하나의 데이터를 같이 읽고/쓰면

```
Thread A: 읽기
Thread B: 쓰기
```

동기화가 필요하다 (Mutex, lock, atomic)같은
반면 Double Buffering이면

```
Thread A
→ Read Buffer만 읽음

Thread B
→ Write Buffer만 씀
```

작업 중에는 서로 같은 메모리를 건드리지 않음
그리고 안전한 시점에서

```
swap(ReadBuffer, WriteBuffer);
```

# shared_ptr

## Control Block

shared_ptr들이 공동으로 참고하는 관리용 정보 덩어리

```
Control Block
- strong reference count   ← shared_ptr 개수
- weak reference count     ← weak_ptr 관련 카운트
- 객체 삭제에 필요한 정보
```

```c++
std::shared_ptr<Player> A = std::make_shared<Player>();
std::shared_ptr<Player> B = A;
```

A와 B가 각각 별도의 카운트를 들고 있는게 아니라, 같은 Control Block을 가리키면서 그 안의 Reference Count를 공유한다.

```
A ─┐
B ─┼──→ Control Block
C ─┘       count = 3
              │
              ↓
            Player
```

## weak_ptr::lock()

```c++
class Player
{
public:
    std::weak_ptr<Guild> GuildPtr;
};

if (auto Guild = PlayerA->GuildPtr.lock())
{
    Guild->DoSomething();
}
```

lock()은 weak_ptr이 가리키던 객체가 아직 살아있는지 확인하고, 살아 있다면 shared_ptr을 생성해 그 객체의 생명을 잠시 연장한다. 이미 객체가 파괴되었다면 빈 shared_ptr을 반환한다.

# RVO, NRVO

```c++
Player CreateA()
{
    return Player{};
}

Player a = CreateA();
```

여기서 Player{}는 prvalue이다.
C++17부터는

```
A가 들어갈 최종 메모리 공간
┌──────────────┐
│              │
└──────────────┘
       ↑
Player{}를 여기 바로 생성
```

이렇게 판정이 되어서

```c++
class Player
{
public:
    Player() = default;

    Player(const Player&) = delete;
    Player(Player&&) = delete;
};
```

이렇게 복사 생성자와 이동생성자가 둘다 없어도 정상적으로 가능하다

---

```c++
Player CreateB()
{
    Player P;
    return P;
}
```

P는 이미 실제 지역 객체이다.

```
CreateB 스택 프레임

┌───────────┐
│ Player P  │
└───────────┘
```

여기서 컴파일러가 NRVO를 적용하면

```
실제로 P를 여기 만들지 않고

호출자
┌──────────────┐
│ 최종 Player   │ ← 이 공간을 처음부터 P로 사용
└──────────────┘
```

하기때문에 이동/복사가 없다.
하지만 NRVO는 보장되지 않는다.
컴파일러가 NRVO를 안 한다고 생각해보면 이동, 복사 생성자가 필요하다 왜냐 실제 P가 있으니까

```
컴파일러가 반드시 해야 한다 ❌
컴파일러가 해도 된다 ✅
```

NRVO는 optional copy elision이라고 한다.
NRVO를 하지 않아도 코드가 성립되어야 함

RVO같은 prvalue반환은 guaranteed copy elision이 보장된다.

## Reader-Writer Lock

읽기 작업끼리는 동시에 진행해도 데이터가 깨지지 않기 떄문에 여러 Render를 같이 허용할 수 있다

일반 mutex

```
Reader A ─ lock
Reader B ─ 대기
Reader C ─ 대기
Writer   ─ 대기
```

Reader-Writer Lock

```
Reader A ─┐
Reader B ─┼→ 동시에 읽기 가능
Reader C ─┘
```

```c++
std::shared_mutex Mutex;

// Reader
std::shared_lock Lock(Mutex);

// Writer
std::unique_lock Lock(Mutex);
```

보통 C++에서는 shared로 Reader를 unique로 Writer를 사용
