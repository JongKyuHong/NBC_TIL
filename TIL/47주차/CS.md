# 알고리즘

## Two Heap

게임 서버에 정수 데이터가 아주 많이 들어온다고 하자.
조건은 다음과 같다.

- 데이터가 계속 추가됨
- 현재 들어온 값들 중 중앙값을 자주 알고 싶음
- 매번 전체 데이터를 정렬하는 건 비효율적임

1. 데이터를 어떤 식으로 나눠서 관리하면 좋을까?
2. 새 값이 들어올 때 두 그룹의 균형을 어떻게 맞출까?
3. 중앙값은 어디에서 확인할 수 있을까?
4. 새 값 추가와 중앙값 조회 비용은 어느 정도가 될까?

---

MinHeap과 MaxHeap두개를 운용하면서 구할 수 있다.
우선 MaxHeap에는 작은 값들을 넣고, MinHeap에는 큰 값을 넣는다.

```
Max Heap           Min Heap
작은 절반          큰 절반

1  3  4            7  9  12
     ↑              ↑
   가장 큼         가장 작음
```

두 Heap의 크기가 1이상 차이나면 힙 크기를 조정하여 균형을 맞춘다.
중앙값의 경우 총 갯수가 홀수면 원소 하나가 더 많은 힙의 top()
짝수개라면 두 top을 이용해서 구한다. (짝수면 두 top의 합의 2분의1)

## deque

C++에서도 std::deque가 표준 라이브러리에 있음
헤더는 <deque>
단점은 vector처럼 완전히 연속된 메모리를 쓰지 않는다.
캐시 지역성이 떨어진다, 메모리 관리 오버헤드가 있다, 단순 순회 성능은 vector가 더 유리한 경우가 많다.

## lower_bound, upper_bound

오름차순으로 정렬된 점수 배열이 있다고 하자.
[10, 20, 20, 20, 30, 40, 50]
특정 점수 20이 처음 등장하는 위치와 마지막으로 등장하는 위치를 빠르게 찾고 싶다.
데이터는 수십만 개까지 커질 수 있다.

1. 어떤 방식으로 찾겠는가?
2. 같은 값이 여러 개 있을 때, 처음 위치를 찾으려면 일반 탐색과 무엇이 달라져야 하나?
3. 마지막 위치는 어떻게 찾을 수 있을까?
4. 전체 시간복잡도는 어느 정도인가?

---

lower_bound(20) : 20 이상이 처음 나오는 위치
upper_bound(20) : 20보다 큰 값이 처음 나오는 위치, 즉 20의 마지막 위치를 쓰려면 upper - 1

이진 탐색을 이용한 경계 찾기이다.

각각 O(log N)의 시간복잡도
lower_bound는 20을 찾고 더 왼쪽에 20이 있는지 계속 확인
upper_bound도 20을 찾고 더 오른쪽에 20이 있는지 확인

## MST(최소 신장 트리)

여러 지역을 도로로 연결하려고 한다.
각 도로에는 건설 비용이 있다.

```
A - B : 4
A - C : 2
B - C : 1
B - D : 5
C - D : 8
C - E : 10
D - E : 2
```

목표는

- 모든 지역이 서로 이동 가능해야 함
- 불필요한 도로는 만들고 싶지 않음
- 전체 건설 비용을 최소화하고 싶음

1. 어떤 방식으로 도로를 선택하겠는가?
2. 도로를 하나 추가할 때 어떤 조건을 확인해야 하는가?
3. 비용이 싼 도로부터 고른다고 해서 무조건 넣으면 안 되는 이유는?
4. 전체적으로 어떤 자료구조나 보조 구조를 같이 쓰면 좋을까?

---

사이클을 만들지 않으면서 가장 싼 간선을 고르는 것 MST
대표적인 방법으로는 크루스칼 알고리즘이 있다.

- 간선 비용을 오름차순으로 정렬
- 가장 싼 간선부터 확인
- 단, 그 간선을 넣었을때 사이클이 생기면 제외
- 모든 정점이 연결될때까지 반복

1. 어떤 방식으로 도로를 선택하는가?

최소 신장트리로 만든다. 크루스칼 알고리즘 사용

2. 도로 하나를 추가할 때 무엇을 확인하는가?

그 도로를 추가하면 사이클이 생기는지 확인한다.

3. 싼 도로부터 무조건 넣으면 안 되는 이유는?

이미 연결된 두 지역을 다시 연결하면 사이클만 만들고 전체 연결에는 도움이 되지 않기 때문

4. 어떤 보조 자료구조를 쓰는가?

Union-Find(Disjoing Set, DSU)를 같이 사용한다.
도로 A-B를 넣기 전에
Find(A), Find(B)를 해서 두 지역의 대표가 같은지 확인한다.
대표가 다름 -> 아직 다른 집합 -> 도로 추가 후 Union
대표가 같음 -> 이미 연결됨 -> 추가하면 사이클 -> 제외

## Quickselect

```c++
std::nth_element(begin, nth, end);
```

nth위치에 정렬했을 때 그자리에 올 값이 오도록 만들 수 있음
pivot값을 하나 잡고, pivot보다 작은 값, 큰값으로 나눈 다음 원하는 순위가 어느쪽에 있는지만 계속 본다.

```
[12, 3, 8, 20, 7, 15, 5]
```

여기서 만약에 3번째로 큰값을 찾는다고 했을때 pivot을 8로 잡으면

```
[12, 20, 15] | [8] | [3, 7, 5]
```

이런식으로 나뉜다. 근데 이미 8보다 큰값이 3개 있으니까

```
[12, 20, 15]
```

에대해서 한번더 pivot을 정하고 나눔
pivot을 20으로 잡으면

```
[] | [20] | [12, 15]
```

이런식으로 나뉜다. 20보다 작은값이 두개니까 12가 정답
분할에 O(N)이 들고 한쪽에만 계속 있다는 가정하에 최선 O(N)
최악의 경우는 pivot이 계속 극단적인 최소 최대값으로가서 O(N^2)까지 될 수 있음

# C++

## virtual 디스패치

```c++
#include <iostream>

class Character
{
public:
    Character()
    {
        PrintType();
    }

    virtual void PrintType()
    {
        std::cout << "Character\n";
    }
};

class Player : public Character
{
public:
    Player()
    {
        PrintType();
    }

    void PrintType() override
    {
        std::cout << "Player\n";
    }
};

int main()
{
    Player player;
}
```

`Player player;` 하나만 생성했는데 생성 과정에서 `PrintType()`이 두 번 호출된다.
각 호출에서 `Character`와 `Player`중 무엇이 출력될 것 같은지, 그리고 왜 그렇게 생각하는지 설명

---

부모 생성자가 먼저 실행 -> 자식생성자가 실행
부모 생성자안에서 호출된 PrintType은 virtual임에도 불구하고 Player::PrintType()으로 내려가지 않는다 왜냐하면 객체가 아직 Player 부분까지 완전히 생성되지 않았기 때문에 생성자안에서의 가상 함수 호출은 현재 생성중인 클래스를 기준으로 동작한다.

> 생성자 안의 virtual 호출이 파생 클래스까지 디스패치 되지 않음

```c++
class Character
{
public:
    virtual void PrintType()
    {
        std::cout << "Character\n";
    }

    void Test()
    {
        PrintType();
    }
}

class Player : public Character
{
public:
    void PrintType() override
    {
        std::cout << "Player\n";
    }
};

Player p;
p.Test();
```

여기서는 Player가 호출된다. 생성자 안에서 PrintType을 호출한게 아니므로

## explicit

생성자를 이용한 암시적 형변환을 막는 키워드

```c++
class Player
{
public:
    Player(int level)
    {
    }
}
```

이렇게 되어있으면

```c++
Player p = 10;
```

이것도 가능하다 컴파일러가 사실상

```c++
Player p = Player(10);
```

처럼 바꿔준다. 이것을 방지하려면 함수명 앞에 explicit을 붙이면 암시적 변환을 금지시킨다.

## 문자열 리터럴

```c++
#include <iostream>

void Print(int value)
{
    std::cout << "int\n";
}

void Print(double value)
{
    std::cout << "double\n";
}

void Print(const char* value)
{
    std::cout << "char*\n";
}

int main()
{
    Print(10);
    Print(3.14);
    Print("Hello");
    Print('A');
}
```

```
int
double
const char*
int
```

로 매칭된다.
"Hello"는 문자열 리터럴이고 이는 const char[6] 타입이다. 함수 인자로 전달될 때 const char\*로 변환된다.
'A'는 char형인데 현재 char오버로드는 없으므로 후보중 가장 적절한 char -> int변환이 가능해서 int로 승격

## pragma once

한 번역 단위에서 같은 헤더가 여러번 포함되는것만 막음, 여러 .cpp에 같은 정의가 생기는 건 막지 못한다.

Player.h

```c++
#pragma once

class Player
{
public:
    void Attack();
};

void PrintPlayer()
{
    // ...
}
```

Game.cpp / Lobby.cpp

```
#include "Player.h"
```

이렇게 있을때 Game.cpp, Lobby.cpp는 정상적으로 컴파일 됐는데 마지막에 실행 파일을 만드는 단계에서 PrintPlayer와 관련된 오류가 발생한다 왜그런가??

-> pragma once는 한 .cpp안에서 헤더가 중복 포함되는것만 막아주지, 여러 .cpp가 같은 함수 정의를 각각 가지는것은 막을 수 없다.

Game.cpp, Lobby.cpp에는 #include "Player.h"가 있기 때문에 사실상 void PrintPlayer()를 포함
각각의 컴파일은 성공하지만 마지막에 합칠때

```
Game.obj -> PrintPlayer 정의 있음
Lobby.obj -> PrintPlayer 정의 있음

링커
-> 같은 함수 정의가 여러개 있음
-> Multiple definition / already defined 오류
```

가 발생한다.
가장 일반적인 해결은 헤더에는 선언만 두는것

```c++
// Player.h
void PrintPlayer();
```

그리고 한 .cpp에서만 구현
혹은 헤더에 구현을 꼭 넣어야 한다면 inline추가

---

.cpp는 .h 내용을 포함한 뒤 하나의 번역 단위가 되어 .obj로 컴파일 됨, 링커는 여러 .obj를 합쳐서 .exe를 만듬

빌드는

1. 전처리
2. 컴파일
3. 링크

순서

1. 전처리는 #include를 실제 내용으로 붙여넣는것

Game.cpp에

```c++
#include "Player.h"
```

가 있으면 전처리기는 개념적으로 `Player.h` 내용을 가져와서 붙임
Game.cpp의 컴파일 직전 예시 모습

```c++
class Player
{
public:
    void Attack();
};

void PrintPlayer();

void Game()
{
    Player p;
    p.Attack();
    PrintPlayer();
}
```

이렇게 .cpp + 포함된 헤더들을 합친 하나의 컴파일 단위를 번역 단위(Translation Unit)이라고 함

2. 각 .cpp는 각각의 .obj로 컴파일

```
Player.cpp + Player.h
→ Player.obj

Game.cpp + Player.h
→ Game.obj

Lobby.cpp + Player.h
→ Lobby.obj
```

각 .obj에는 대략 이런 정보

```
Player.obj
- Player::Attack 실제 기계어 코드
- PrintPlayer 실제 기계어 코드

Game.obj
- Game 함수 실제 코드
- "Player::Attack을 호출해야 함"
- "PrintPlayer를 호출해야 함"
```

Game.obj는 Player::Attack()의 실제 구현이 어디 있는지 아직 완전히 몰라도 컴파일할 수 있음
헤더에서

```
void Attack();
```

이라는 선언을 봤기 때문에, 이런함수가 어딘가에 있긴 하구나 라고 믿고 컴파일

3. 링커가 .obj들을 연결

예를들어 Game.obj가 Player::Attack 필요하다 라고 하면 Player.obj에 정의가 있으므로 링커가 둘을 연결함
이 작업들이 끝나고 Game.exe완성

문제 상황의 경우는 Game.obj, Lobby.obj 두 경우에 PrintPlayer에 대한 정의가 있어서 문제

# CS

## 메모리

크게

- 코드 영역
- 데이터 영역
- 스택 영역
- 힙 영역

으로 나뉜다.

특히 여기서 데이터, 코드영역이 헷갈리는 부분이 있어서 완벽하게 잡고 갈 예정이다.

### 데이터 영역

우선 데이터영역은 딱딱한 말로 하면

- 전역 변수, static 변수
- 초기화된 데이터
- BSS : 0또는 초기화되지 않은 전역/static

이라고 한다.
여기서 초기화된 데이터는 그냥 int x = 10 같은것을 말한다 근데 이게 이제 함수안에 들어있으면 stack영역으로 분류된다.

```c++
int x = 10; // 데이터 영역
void Func()
{
    int y = 10; // 스택 영역
    static z = 10; // 데이터 영역
    int* ptr = new int[10]; // ptr은 스택영역, new int자체는 heap영역
}
```

### 코드 영역

코드영역(Code/Text영역)은 프로그램이 실행할 기계어 명령들이 들어있는 영역

```c++
int Add(int a, int b)
{
    return a + b;
}
```

여기서 Add함수의 실행 코드 자체가 컴파일되면 기계어 명령으로 바뀌고, 그게 코드영역에 들어간다.

이게 무슨말이냐

```c++
int x = 10;
void Func()
{
    int y = 20;
    int* ptr = new int[10];
}
```

여기에서 x는 데이터 영역, y는 스택영역, ptr은 스택, new int는 heap영역이다. Func()를 실행시키기 위한 기계어 명령이 코드영역이다. 이 기계어 명령이 실행중에 만들어지는 지역변수 y는 스택영역

```
Code영역
-> Func()를 어떻게 실행할지에 대한 CPU명령어

Stack영역
-> Func()를 실행하는 동안 필요한 지역변수, 매개변수, 호출정보 등

Data 영역
-> 전역, static변수 등의 실제 데이터

Heap 영역
-> new등으로 동적 할당한 데이터
```

## 물리 코어, 하드웨어 스레드

같은 물리 코어 안의 두 하드웨어 스레드는 "실행 상태"는 따로 가지고, 실제 연산 장치와 캐시같은 코어 자원은 상당 부분 공유

### 하드웨어 스레드별로 따로

- 레지스터 상태
- Stack Pointer
- Program Counter
- 실행 컨텍스트

### 같은 물리코어에서 공유

- ALU / FPU같은 실행 유닛
- 캐시 일부
- 메모리 접근 대역폭
- 기타 CPU 내부 실행 자원

> 각 하드웨어 스레드는 자기 실행 상태는 따로 가지지만, 실제 계산 자원은 같은 물리코어 안에서 공유한다.

## Priority Inversion

우선순위가 높은 스레드가 낮은 스레드 때문에 오히려 기다리게 되는 현상

```
1. L이 mutex 획득
2. H가 실행됨
   → mutex 필요
   → L 때문에 대기
3. M이 실행됨
   → M은 L보다 우선순위가 높음
   → CPU를 계속 사용
4. L은 CPU를 못 받아서 mutex를 못 품
5. H는 계속 대기
```

높은 우선순위 H가 사실상 중간 우선순위 M보다도 늦게 실행되는 것처럼 보이는 증상

### 해결 방법

Priority Inheritance

```
H가 L의 mutex를 기다림
-> L의 우선순위를 임시로 H수준까지 올림
-> L이 빨리 실행돼서 mutex해제
-> H실행
-> L 우선순위 원래대로 복구
```

## DIP

의존성 역전 원칙

1. 상위 수준 모듈은 하위 수준 모듈에 의존하지 않아야 한다. 둘 다 추상화에 의존해야 한다.
2. 추상화는 구체적인 구현에 의존하지 않고, 구체적인 구현이 추상화에 의존해야 한다.

상위 수준 = 무엇을 할지 결정하는 정책
하위 수준 = 그 정책에 필요한 구체적인 구현

이는 문과 상호작용하는 캐릭터를 통해서 익히면 된다.
문은 캐릭터한테 열쇠가 있는지 알아야 하고, 열쇠가 있다면 열리게끔 해야하는데 문에 캐릭터관련 의존성을 넣으면 문을 여는 다른 NPC나 pet같은게 생겼을때 계속 늘어나야 하고, 캐릭터에 문 의존성을 넣자니 캐릭터가 문의 열림 조건과 동작까지 알아야 하고, 새로운 상호작용 대상이 추가될 때마다 캐릭터 코드를 수정해야 하는 문제가 발생한다. 이때 Interface를 사용해서 캐릭터와 문이 Interface에 의존하도록 만든다.

```c++
class IItemProvider
{
public:
    virtual bool HasItem(int itemId) const = 0;
    virtual ~IItemProvider() = default;
};

class Player : public IItemProvider{
    ...
    bool HasItem(int ItemId) const override
    {
        return InventoryData.HasItem(ItemId);
    }
};
```

# Unreal

## TSubClassOf

TSubClassOf<T>는 T자체 또는 T를 상속받은 클래스 정보만 저장할 수 있는 타입
실제 객체 인스턴스를 저장하는게 아니고, 클래스 자체를 저장한다.

```c++
TSubclassOf<AWeapon> WeaponClass;
```

```
AWeapon
 ├─ BP_Rifle
 ├─ BP_Shotgun
 └─ BP_RocketLauncher
```

이렇게 AWeapon을 저장해두고 나중에 뭘 Spawn할지를 저장
BP에서도 AWeapon을 상속한 클래스만 선택지로 뜬다.

```c++
AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(
    WeaponClass,
    SpawnLocation,
    SpawnRotation
);
```

처럼 실제 인스턴스를 만들 수 있다.

## TWeakObjectPtr

TWeakObjectPtr<T>는 UObject 계열객체를 `소유하지 않고 잠깐 참조`할 때 쓰는 약한 포인터

## Push Model

복제 변수 값이 바뀌었는지 엔진이 계속 비교해서 찾는 대신, 값이 바뀐 순간 개발자가 '이 변수 바뀌었다'고 표시하는 방식

기본적인 Replication방식은

```
Polling

서버
-> Replicated 변수들을 주기적으로 확인
-> 이전 값과 비교
-> 바뀌었으면 클라이언트에 전송
```

Actor와 Replicated Property가 많아지면 안바뀐값까지 계속 확인하는 비용이 생길 수 있다.

Push Model은 반대로

```
Health변경
-> Health바뀌었음 표시
-> Replication 시스템
-> 변경된 Property를 복제
```

dirty표시하는 MarkPropertyDirty기능이 있다.

```c++
UPROPERTY(Replicated)
float Health;

//
Health = 50.f // Health변경
MARK_PROPERTY_DIRTY_FROM_NAME(
    AMyCharacter,
    Health,
    this
); // dirty표시
```

처럼 Health가 바뀌었다고 알려줌

## Delegate

AddDynamic, AddStatic, AddUObject 차이점

- AddDynamic : 리플렉션 시스템을 통해 호출할 UFUNCTION을 등록
- AddUObject : 일반 C++ Delegate에 UObject의 멤버 함수를 등록
- AddStatic : 일반 C++ Delegate에 정적 함수 또는 전역 함수를 등록

AddDynamic은 언리얼 리플렉션 시스템이 함수를 찾고 호출할 수 있어야 하므로, 해당 함수가 UFUNCTION()으로 등록되어 있어야 함

다이나믹 델리게이트는 주로 블루프린트에서도 바인딩할 수 있게 만들고 싶을때 사용한다.
엔진이 이미 Dynamic으로 선언한 경우도 AddDynamic을 사용

C++ 코드에서만 이벤트를 처리하면 -> Native Delegate
블루프린트에서도 이벤트 바인딩이 필요하다? -> Dynamic Delegate
바인딩 정보의 직렬화 필요 -> Dynamic Delegate
엔진이 이미 제공하는 델리게이트 -> 선언된 종류에 따라 다름

## BlueprintImplementableEvent, BlueprintNativeEvent

### BlueprintImplementableEvent

- C++에서 함수구현 불가
- 블루프린트에서 구현 가능
- 블루프린트 구현이 없으면 아무 동작도 하지 않음
- 주 사용 목적 : BP에 동작을 맡길 때

### BlueprintNativeEvent

- C++에서 함수 구현
- 블루프린트에서 구현 가능
- 블루프린트 구현이 없다면 C++기본 구현 실행
- 주 사용목적은 C++ 기본동작을 BP에서 확장하거나 변경할 때

## check, ensure, verify

1. check

- 반드시 참이어야 하는 조건 검사
- 조건이 false면 실행 중단
- Shipping에서는 조건식 실행, 검사실패 중단이 없음
- 절대 null이면 안되는 포인터에서 사용

2. ensure

- 비정상 상황을 보고하되 실행 유지
- 조건이 false여도 오류 보고 후 계속 실행
- Shipping에서 조건식은 실행하지만, 검사 실패로 중단하지 않음
- 예상치 못한 상태 감지에 사용

3. verify

- 반드시 실행해야 하는 식의 결과 검사
- 조건이 false면 실행 중단
- Shipping에서 조건식 실행, 검사 실패로 중단하지 않음
- 초기화 함수의 성공 여부 검사에 사용

IsValid와 nullptr의 차이

- nullptr은 해당 포인터에 주소가 들어있는지만 확인
- IsValid는 null이 아닌지, 주소가 존재하더라도 파괴 예정상태(Pending Kill)또는 Garbage로 표시된 상태가 아닌지 확인(이미 해제되어 댕글링 포인터가 된 임의 주소까지 IsValid가 안전하게 검사해주지는 않음)

## 리슨서버, 데디서버

리슨서버 : 서버 역할을 하면서 그 머신에 로컬 플레이어도 존재
데디케이트 서버 : 로컬플레이어가 없는 헤드리스 서버

데디서버에서는 로컬 PC, UI, 입력, 렌더링이 있다고 전제하면 안된다.

## Actor 복제와 Actor이동 복제

bReplicates = true는 이 Actor가 네트워크를 통해 다른 클라이언트에도 존재하고, 복제 프로퍼티를 전달할 수 있게 만드는 설정
서버에서 바뀌는 위치/회전까지 자동으로 보내려면 보통 이동 복제를 별도로 켜야함
SetReplicateMovement(true);

# 네트워크

## TCP

3 Way HandShake는 회선을 연결하는게 아니라 TCP연결 상태와 초기 시퀀스 번호등을 설정하는 과정
TCP는 패킷단위가 아니고 바이트 스트림을 제공한다

## Jitter

패킷의 지연시간이 일정하지 않고 변동하는 것

- 라우터에 트래픽이 몰려 패킷이 대기
- 패킷마다 경로 또는 처리 시간이 달라짐
- 송수신 측의 처리 지연이 달라짐

## Interpolation

과거에 수신한 두 위치 사이를 계산해서 중간 위치를 만들어내는 방식
다만 보간할때 두 시점의 데이터를 미리 가지고 있어야 하므로, 일반적으로 일정 시간 과거의 상태를 화면에 표시한다.
단점으로는 화면을 부드럽게 만드는대신 표시지연을 발생시킨다.
만약 100ms의 보간 버퍼를 사용한다면 클라이언트는 서버의 시점보다 약 100ms 이전의 상태를 표시하게 되고, 서버에서 데이터가 도착하기까지의 네트워크 지연도 존재한다.
버퍼를 크게 잡으면 불규칙한 도착에 더 잘대응하지만 화면에 보이는 상태는 더 오래전것이 됨, 반대로 버퍼를 작게 잡으면 패킷이 늦게올때 보간할 데이터가 부족할 수 있다.

## Socket

프로그램이 네트워크 통신을 하기 위해 운영체제에 만들어 놓은 통신용 객체/창구

```
소켓
-> 통신 상태와 정보를 관리하는 OS 객체

포트
-> 그 소켓이 사용할 수 있는 논리적인 번호
```

서버 프로그램이 UDP로 통신하려면

```
프로그램
-> UDP 소켓 생성
-> 로컬 포트 7777에 bind
-> 7777번 포트로 들어오는 UDP 데이터 수신
```

UDP소켓과 TCP소켓

```
UDP 소켓
-> 연결 없이 패킷 단위 통신
-> 하나의 로컬 포트에서 여러 송신자의 데이터를 받을 수 있음

TCP 소켓
-> 연결 기반 통신
-> 서버에서는 보통 연결마다 별도의 connected socket이 생김
-> 접속을 받는 listening socket하나가 있고, 각 연결을 담당하는 여러개의 연결 소켓이 생기는 셈
```

## MTU

한 번의 링크 전송에서 실을 수 있는 IP 패킷 크기의 상한에 가까운 개념
MTU(Maximum Transmission Unit) : 해당 네트워크 구간에서 한 프레임에 실을 수 있는 최대 크기

## IP 단편화

큰 IP패킷 하나를 여러 조각으로 쪼개 보내는 것
IP패킷이 MTU보다 크면 IPv4에서는 조건에 따라 IP계층이 여러 fragment로 쪼갤 수 있다.

```
게임 데이터
↓
UDP가 헤더를 붙임
→ UDP 데이터그램
↓
IP가 헤더를 붙임
→ IP 패킷
↓
Ethernet/Wi-Fi가 헤더를 붙임
→ Frame
↓
실제로 전송
→ 전기/무선 신호, 즉 비트들
```

```
[Ethernet 헤더
    [IP 헤더
        [UDP 헤더
            "Hello"
        ]
    ]
]
```

IP계층에서 MTU보다 크면 조건이 맞다면 IP패킷을 여러개 만들고 이게 각각의 프레임으로 전송된다.

# 운영체제

## 게임을 실행했을때 동작 흐름

```
런처가 Windows에 Game.exe 실행 요청
-> 운영체제가 게임 전용 실행 환경 생성
-> 실행 파일 매핑, 가상 주소 공간 구성
-> CPU 실행 기회 획득, DLL, 런타임 초기화
-> main() / WinMain()등에 도달
```

### 1. Windows에 실행 요청

런처는 Windows의 CreateProcess()같은 API를 이용해서 새로운 프로그램 실행하도록 요청
Windows는 필요한 커널 작업을 수행

### 2. 운영체제가 프로세스를 생성

운영체제는 Game.exe를 관리하기 위한 프로세스 객체를 만듬, 실행에 필요한 정보를 준비

- 프로세스를 구분하는 PID
- 프로세스의 가상 주소 공간
- 메모리와 실행 상태를 관리하기 위한 정보
- 프로그램을 실행할 초기 스레드

프로세스는 단순히 RAM에 올라간 실행 파일 자체가 아니고, 운영체제가 관리하는 실행 중인 프로그램의 환경

### 3. 실행 파일을 읽고 메모리를 준비

운영체제는 Game.exe파일의 형식을 확인하고, 실행에 필요한 코드와 데이터가 프로세스의 가상 주소공간에 배치되도록 준비함

### 4. 스레드가 실행되고 초기화가 진행

운영체제의 스케줄러가 스레드에 CPU 실행 기회를 주면, 사용자 모드의 로더가 필요한 DLL을 준비하는 등의 초기화가 진행됨 이후, C/C++ 런타임 초기화, 전역 객체 생성등의 과정을 거쳐 프로그램 진입 함수에 도달

### 5. 게임 코드 실행

> 결론적으로 Game.exe를 실행하면 운영체제는 Game.exe를 실행 가능한 환경인 프로세스를 만든다. 프로세스에서 실행에 필요한 정보들을 모아서 가상 주소 공간에 준비시킨다. 스케줄러가 스레드에 CPU 실행 기회를 주면 그때 초기화가 진행되면서 게임에 진입할 준비가 되었다.

## User Mode

일반 프로그램이 실행되는 CPU의 제한된 권한 모드

## Kernel Mode

운영체제가 하드웨어와 시스템 자원을 직접 제어할 수 있는 CPU의 높은 권한의 모드

## System Call

User Mode 프로글매이 Kernel Mode 기능을 요청하는 통로

```
CPU가 User Mode로 실행 중
-> 일반 프로그램 명령 실행
-> 특권 명령 실행 불가

CPU가 Kernel Mode로 실행 중
-> OS 커널 코드 실행
-> 하드웨어/메모리 관리 같은 특권 명령 실행 가능
```

## Interrupt

하드웨어가 CPU에 특정 사건이 발생했다고 알리는 방법

## 스레드 상애

Waiting -> Ready -> Running

- Waiting : I/O 완료 같은 특정 사건을 기다림
- Ready : 기다리던 사건이 완료되어 CPU를 할당받을 수 있음
- Running : 스케줄러에 의해 CPU를 할당받아 실행 중

## 메모리 공간 확보

만약에 게임 프로그램이 앞으로 사용할 데이터를 위해 2GB 크기의 메모리 공간을 미리 확보하려 한다. 하지만 게임을 시작한 직후 사용하는 데이터는 10MB정도뿐, 운영체제는 메모리 공간을 어떻게 할당해줘야할까?

1. Reserve

주소 자리만 찜하기

```
프로세스 가상 주소 공간

[---------------- 2GB ----------------]
          예약만 해둠
```

`MEM_RESERVE`는 말 그대로, 이 2GB 주소 범위는 내가 쓸거니까 다른 용도로 쓰지말라는 예약
이때는 가상 주소 공간 2GB는 확보되었지만 실제 RAM을 사용하는건 아니다.

2. Commit

이 주소를 실제로 사용할 수 있게 보장, 2GB중 처음 10MB만 필요하다고 했을때

```
예약된 2GB

[10MB Commit][---------------- 아직 Reserve만 ----------------]
```

이 10MB는 이제 프로그램이 사용해도 된다.
라고 OS에 요청하는게 `MEM_COMMIT`이다.

Commmit = RAM 10MB를 지금 즉시 꽂아주는건 아니다. Windows는 이 10MB에 대해 필요할 때 데이터를 저장할 수 있도록 커밋 자원(backing store)을 보장해 놓음

여기에 실제로 접근하면

```c++
memory[0] = 100;
```

```
CPU가 가상 주소 접근
-> 아직 물리 페이지 없음
-> Page Fault
-> OS가 물리 RAM 페이지 준비
-> Page Table에 연결
-> 다시 실행
```

실제 접근한 페이지는 물리 RAM에 실제 페이지 연결된다.
Commit만 하였을때도 해당 자리는 실제 물리 RAM에서는 다른 프로그램이 사용하고 있을 수 있다.

Reserve = RAM자리 미리 찜
Commit = 필요할때 이 메모리를 실제 제공할 수 있도록 시스템 자원 보장

## OS캐시

저장장치에서 읽은 데이터를 RAM에 임시로 보관해서, 같은 데이터를 다시 요청할 때 하드까지 다시 가지 않게하는 기능
OS가 관리하는 RAM영역, RAM이 남아있는데 비워둘 이유가 없으므로 최근 읽은 파일 데이터를 가지고 있는것
하지만 다른 프로그램이 RAM을 많이 필요로 하면 OS파일 캐시는 일부 페이지를 회수할수도 있다.
