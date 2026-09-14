# 16일차

## Copy Semantics / Ownership 정리

Copy Semantics는 이 객체를 복사한다는게 프로그램에서 무슨 의미인가? 를정하는것

예를들어

```c++
class Inventory
{
    std::vector<Item> Items;
};
```

Inventory를 복사한다면 보통 아이템 목록도 독립적으로 복사된 새로운 Inventory가 자연스럽다

반대로

```c++
class Enemy
{
    AIController Controller;
    PhysicsBody Physics;
    ...
};
```

Enemy를 복사한다는게 애매할 수 있다.

- 같은 AI 상태까지 복사?
- 같은 월드 위치?
- 네트워크 ID?
- 물리 객체?
- 등록된 이벤트?

이런 경우는 애초에 Enemy는 복사 불가능하게 만드는게 더 올바른 설계
즉, 모든 객체가 복사 가능해야 하는게 아니다.

---

Ownership 판단

게임코드에서

`값으로 직접 가지고 있음`

```
std::vector<Item> Items;
```

객체가 그 데이터를 직접 소유한다.
부모객체가 죽으면 같이 정리된다.

`unique_ptr`

```c++
std::unique_ptr<Weapon> Weapon;
```

이 객체가 Weapon을 단독 소유한다.

그래서 기본적으로 복사가 안된다.
오히려 이게 소유권이 두 군데 생기는것을 막아줘서 좋은것

`shared_ptr`

```c++
std::shared_ptr<Resource> Resource;
```

복사하면 Resource자체가 복사되는게 아니라 같은 객체를 공동 소유한다.
그래서 정말 공동 소유가 필요한 경우에만 사용한다.

`Raw pointer / Reference`

```c++
Enemy* Target;
Enemy& Owner;
```

내가 소유하는게 아니라 다른 곳의 객체를 참조한다.
는 의미로 사용할 수 있다.
따라서 복사해도 같은 Enemy를 가리키는게 자연스러울 순 있는데 Dangling을 조심해야함

### 결론

클래스를 만들때

1. 이 객체는 복사되는게 자연스러운가?
2. 멤버 중 누가 무엇을 소유했는가?
3. 복사한다면 Resource도 복제할 것인가, 공유할 것인가?
4. 복사의 의미가 애매하면 차라리 복사를 금지할 것인가?

판단이 중요하다.

---

## 목표

> Copy Constructor / Copy Assignment의 실전 의미
> 어떤 클래스를 복사 가능하게 만들고, 어떤 클래스는 복사를 막아야 하는지 판단한다.

Copy Constructor와 Copy Assignment가 언제 필요한 연산인지 이해한다.

---

## 둘의 차이

Copy Constructor(복사 생성자)

```c++
Player b = a;
```

b가 아직 없었다.
-> a를 이용해서 새로운 b를 만든다.

Copy Assignment(복사 대입 연산자)

```c++
Player a;
Player b;

b = a;
```

### 직접 만들어 사용??

```C++
class PlayerConfig
{
public:
    std::string Name;
    std::vector<int> Stats;
    int Level;
};
```

이 클래스는 멤버들이 알아서 올바르게 복사된다. (string, vector, int모두 자체적으로 깊은복사를 지원)
그래서 직접

```c++
PlayerConfig(const PlayerConfig& other)
```

같은걸 구현할 이유가 거의 없다. 차라리 컴파일러에게 맡기는게 낫다

```c++
PlayerConfig A;
A.Name = "Hero";
A.Stats = {10, 20, 30};
A.Level = 5;

PlayerConfig B = A; // 복사 생성자 자동 호출
```

이때 컴파일러는 Name -> 문자열 메모리를 새로 할당받아 내부 텍스트("Hero")를 안전하게 깊은 복사함
Stats -> 동적 배열 메모리를 새로 할당받아 원소들을 깊은 복사함
Level -> 기본 타입이므로 값(5)을 단순 복사

이를
Rule of Zero

> 자원 관리를 이미 잘하는 타입들을 멤버로 사용하면 내 클래스가 직접 복사/소멸 코드를 작성할 일이 줄어든다.

만약 클래스가 원시포인터를 직접 동적할당하여 관리하고 있다면

```c++
class DangerousPlayer
{
public:
    char* Name; // 원시 포인터
    int Level;
};
```

이 경우 컴파일러가 자동 생성한 복사는 포인터 주소만 그대로 복사하는 얕은복사를 수행한다.

## 복사가 자연스러운 예시

```
struct WeaponStats
{
    float Damage;
    float FireRate;
    float Range;
};

struct CharacterConfig
{
    std::string Name;
    std::vector<float> Stats;
};
```

이런 데이터들은 복사가 자연스럽다. 값 자체가 의미인 객체라서
하나 복사해서 수정해도 원본과 별개인게 자연스럽다.

## 복사가 이상한 객체

```c++
class NetworkConnection
{
};

class ThreadPool
{
};

class GameServer
{
};
```

이런 객체는 `복사본을 하나 만든다`라는 의미 자체가 이상할 수 있다.

ThreadPool을 복사하면

- Worker Thread들도 복사?
- Queue도 복사?
- 진행중 Task도 복사?

이런 객체들은 복사를 막는게 자연스러움

```c++
class ThreadPool
{
public:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
};
```

> 복사가 의미없는 타입이면 API수준에서 복사를 금지한다.

## unique_ptr이 있다면?

```c++
class Character
{
    std::unique_ptr<Weapon> Weapon;
};
```

기본적으로 Character도 자동 복사가 안된다. Weapon을 누가 소유해야 하는지 C++이 마음대로 정할 수 없기에

**선택 1**

Character 자체를 복사하지 않는다.
게임 Entity라면 이쪽이 자연스러울 수 있다.

**선택 2**

Character를 복사하면 Weapon도 새로 복제한다.
이 경우에만 직접 Copy Semantics를 정의한다.

직접 Copy Constructor를 구현하는건 실제 설계 요구가 있는경우만 하는게 맞다

## Copy Assignment가 더 까다로운 이유

```c++
Player a;
Player b;

...
b = a;
```

이 경우에 b가 이미 무언가를 가지고 있을 수 있다.
예를들어 a -> Weapon A소유, b -> Weapon B소유
b = a했을때 기존 Weapon B는 어떻게 할것인가?
RAII타입을 사용하면 이런 정리를 멤버 타입이 알아서 해줌
그래서 결론적으로 RAII타입들을 이용해서 직접 자원 관리 코드를 줄이는게 중요하다.

## 게임 개발에서의 기준

```c++
class Character
{
public:
    CharacterStats Stats;
    std::vector<Item> Inventory;
    std::unique_ptr<Weapon> Weapon;
    Enemy* Target;
};
```

복사 요구사항을 판단할때

- Stats -> 값이므로 복사가 자연스러움
- Inventory -> 독립적인 Inventory복사가 자연스러울 수 있음
- Weapon -> 독점 소유, 복사 할지 새 Weapon을 만들지 결정 필요
- Target -> non-owning이라면 같은 Enemy를 가리켜도 될 수 있음

그래서 결론적으로 `Character자체를 복사하는게 게임 설계상 필요한가?`를 고민
