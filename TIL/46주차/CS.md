# atomic

가장 헷갈리는 부분
atomic은 락을 거는것이 아니고 원자적 연산을 보장해주는것
atomic은 연산을 쪼개지지 않게 보장 + 여러 코어에서 그 atomic값의 변경을 일관되게 관찰하도록 함
atomic을 안써도 캐시 일관성 프로토콜은 동작을 한다. 하지만 두 스레드가 하나의 변수에 대해서 읽고 둘다 값을 쓰는 경쟁상태까지는 막아주지 못한다.

여러 변수 사이의 상태 일관성은 자동으로 묶어주지 않는다.

```c++
struct PlayerState
{
    int HP = 100;
    bool IsDead = false;
};
```

// Thread A

```c++
HP -= 100;

if (HP <= 0)
    IsDead = true;
```

// Thread B

```c++
if (!IsDead)
{
    // 살아있는 플레이어라고 판단하고
    // 어떤 행동을 수행
}
```

여기서 HP, IsDead를 atomic으로 바꾸어도 이 문제는 해결되지 않는다.

ThreadA작업중에

```c++
HP -= 100;        // HP = 0

// 이 사이에 Thread B가 실행될 수 있음

IsDead = true;
```

---

mutex를 써서 해결할 수 있다.

```c++
struct PlayerState
{
    int HP = 100;
    bool IsDead = false;
    std::mutex Mutex;
};
```

// Thread A

```c++
{
    std::lock_guard lock(Mutex);

    HP -= 100;
    if (HP <= 0)
        IsDead = true;
}
```

// Thread B

```c++
{
    std::lock_guard lock(Mutex);

    if (!IsDead)
    {
        // 행동
    }
}
```

# 객체 슬라이싱

```c++
#include <iostream>

class Character
{
public:
    virtual void Attack()
    {
        std::cout << "Character Attack\n";
    }
};

class Player : public Character
{
public:
    void Attack() override
    {
        std::cout << "Player Attack\n";
    }
};

void DoAttack(Character character)
{
    character.Attack();
}

int main()
{
    Player player;
    DoAttack(player);
}
```

Player전체가 함수 안으로 유지되는게 아니고 Character에 해당하는 부분만 복사되어 별도의 Character 객체가 만들어진다. Player부분은 잘려나간다. 이걸 object slicing(객체 슬라이싱)이라고 한다

# forwarding reference

```c++
template<typename T>
void Func(T&& value);
```

보통 이런 형태에서 T가 함수 호출 시 추론되는 경우 T&&를 forwarding reference라고 함

```c++
int x = 10;

Func(x);   // x는 lvalue
           // T = int&
           // T&& = int& && → int&

Func(20);  // 20은 rvalue
           // T = int
           // T&& = int&&
```

C++ 제작자들은 Forward(T&&) 하나로 lvalue와 rvalue를 둘다 받고 싶어했고 lvalue(x)가 들어오면 타입 T자체를 int가 아니라 int&로 추론하자고 약속

한시 30분까지

# SIMD

하나의 명령으로 여러 데이터를 동시에 계산하는 방식

```
a[0] + b[0]
a[1] + b[1]
a[2] + b[2]
a[3] + b[3]
```

이렇게 4번연산한다면 SIMD는 개념적으로

```
[a0 a1 a2 a3]
+
[b0 b1 b2 b3]
=
[c0 c1 c2 c3]
```

처럼 한 번에 여러 원소를 병렬로 처리한다.

## 한 SIMD명령안에서는 같은 타입만 계산 가능한가?

한 SIMD명령 안에서는 보통 같은 타입/같은 크기의 데이터끼리 계산한다고 본다.

```
float 4개 + float 4개
int 4개   + int 4개
```

이런식이라서 128비트 SIMD 레지스터라면

```
float 32비트 × 4개
int32 32비트 × 4개
int16 16비트 × 8개
int8  8비트  × 16개
```

처럼들어갈 수 있다.
결론적으로 핵심은

> 같은 연산을 여러 데이터에 동시에 적용한다.
> Single Instruction Multiple Data

```c++
Position[i] += Velocity[i] * DeltaTime;
```

캐릭터 위치 업데이트를 캐릭터 1000개에 반복한다면 SIMD가 잘먹히는 대표적인 경우이다.

# Net Relevancy

이 Actor 정볼를 이 클라이언트에게 지금 보내줄 필요가 있느냐?
100명의 플레이어가 있는 큰 맵에서 내 위치에서 수 km 떨어진 플레이어까지 매 프레임 복제할 필요는 없다.

중요한건 Relevancy는 클라이언트마다 따로 판단한다. 같은 Actor라도 A 플레이어에게는 Relevant이고, B 플레이어에게는 Not Relevant일 수 있다. 언리얼은 기본적으로 AActor::IsNetRelevantFor()를 통해 특정 Connection에 대해 이 Actor가 Relevant한지를 판단한다.

보통

- 거리
- 소유한 클라만
- 자기 Owner의 판단 따라가기
- 일정 거리안에서
- 거리에 상관없이

등등의 조건이 있다.

```
Actor가 Replication 대상인가?
        ↓
    bReplicates
        ↓
이 Connection에게 보낼 필요가 있는가?
        ↓
    Net Relevancy
        ↓
      Relevant
        ↓
실제로 복제 후보가 됨
```

bReplicated = true라고 해도 Not Relevant면 복제를 하고 있지 않는다.

# Dormancy

이 Actor는 한동안 상태가 안바뀌니까, 아예 복제 검사 대상에서 빼두자
