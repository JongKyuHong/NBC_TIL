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

## 비트마스킹

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


