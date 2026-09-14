# Copy Semantics / Ownership 정리

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

## 결론

클래스를 만들때

1. 이 객체는 복사되는게 자연스러운가?
2. 멤버 중 누가 무엇을 소유했는가?
3. 복사한다면 Resource도 복제할 것인가, 공유할 것인가?
4. 복사의 의미가 애매하면 차라리 복사를 금지할 것인가?

판단이 중요하다.

---

# Copy Constructor / Copy Assignment

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

## 직접 만들어 사용??

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

---

# Value Category

## 목표

```c++
std::string a = "hello";
std::string b = a;
std::string c = std::string("hello");
```

여기서 어떤 경우는 복사, 어떤경우는 이동 가능한 후보가 되는지 설명할 수 있는가

## 왜 Value Category가 필요한가?

C++에서는 어떤 표현식이

- 이미 존재하는 특정 객체를 나타내는지
- 곧 사라질 임시 값인지
- 존재하는 객체지만 이제 자원을 가져가도 되는 상태인지

에 따라 다른 함수를 선택할 수 있다.

```c++
void SetName(const std::string& name); // 읽기/복사 용도
void SetName(std::string&& name);      // 이동 가능한 값
```

호출

```c++
std::string name = "Kim";

SetName(name);
SetName(std::string("Kim"));
```

두 호출의 name 표현식과 임시 std::string("Kim")의 성질이 다르기 때문에 서로 다른 overload가 선택될 수 있다.

## 큰 구조

C++의 분류를 보자

```
Expression
│
├─ glvalue
│   ├─ lvalue
│   └─ xvalue
│
└─ prvalue


rvalue
├─ prvalue
└─ xvalue
```

즉

```
glvalue = lvalue + xvalue
rvalue = prvalue + xvalue
```

`xvalue는 둘에 동시에 포함된다.`

## lvalue

```c++
std::string name = "Player";
```

여기서 표현식 name은 lvalue다

특징은

> 현재 존재하는 특정 객체를 나타내며, 그 객체의 정체성을 계속 추적할 수 있다.

쉽게말해 name이라는 그 객체가 실제로 계속 존재한다.

```c++
name = "Enemy";
name.size();
&name;
```

이것처럼 계속 사용할 수 있다.

여기서 주의할건 lvalue는 왼쪽에 올 수 있는 값이 아니다.

```c++
const int x = 10;
x = 20;
```

const라는 예외가 있다.
lvalue = 대입 가능이 아니다.

## prvalue

```c++
10
또는
std::string("Player");
```

같은 표현식은 대표적인 prvalue이다.

```c++
std::string name = std::string("Player");
```

여기서도 오른쪽 std::string("Player")는 prvalue이다.

> 특정 장기 생존 객체의 이름을 나타내기보다는 새로운 값을 계산하거나 생성하는 표현식

이라고 이해하면 좋다.

```c++
FVector GetSpawnPosition();

FVector pos = GetSpawnPosition();
```

GetSpawnPosition()이 값으로 FVector를 반환한다면 그 함수 호출 표현식도 보통 prvalue다.

## xvalue

```c++
std::string name = "Player";
std::move(name);
```

std::move(name) 표현식은 xvalue다.
name객체가 사라진건 아니다, 여전히 name은 존재 하지만 std::move(name)이라는 표현식은

> 이 객체는 이제 자원을 가져가도 되는 대상으로 취급해도 된다.

라는 형태의 표현식

그래서 xvalue는

- 실제 특정 객체를 나타냄 -> glvalue
- 이동 가능한 대상으로 취급됨 -> rvalue

둘다 해당한다.

## 가장 실용적인 세분류

**lvalue**

```c++
std::string a = "hello";
a
```

-> 살아있는 기존 객체

**prvalue**

```c++
std::string("hello");
```

-> 새 값을 만드는 표현식/임시 값

**xvalue**

```c++
std::move(a);
```

-> 기존 객체인데 이동 가능한 대상으로 표시된 표현식

이 세개를 구분하면 Move Semantics의 대부분이 이해된다!

## 중요점

객체와 표현식을 구분해야 한다.

```c++
std::string a = "hello";
```

여기서 a객체가 lvalue객체인게 아니다.
a라는 표현식의 Value Category가 lvalue다.

왜냐하면 같은객체를 a로 쓰면 lvalue인데 std::move(a)로 쓰면 xvalue이기 때문이다. (둘다 같은 a객체를 나타낸다)
객체가 변신한게 아니라 그 객체를 나타내는 표현식의 분류가 달라진 것

```
a
→ lvalue expression
        │
        ▼
   같은 string 객체

std::move(a)
→ xvalue expression
        │
        ▼
   같은 string 객체
```

## 이게 함수선택에 영향을 줌

```c++
void Func(const std::string& value)
{
    std::cout << "const&";
}

void Func(std::string&& value)
{
    std::cout << "&&";
}

std::string str = "hello";
Func(str);
```

str은 lvalue 표현식이므로 보통 Func(const std::string&)쪽이 선택된다.
반면 Func(std::string("hello"))로 호출한다면 prvalue이므로 Func(std::string&&)에 바인딩 될 수 있다.

## rvalue는?

rvalue = prvalue + xvalue

prvalue = 새로 만들어진 임시 값
xvalue = 기존 객체를 이동 가능한 대상으로 나타냄
둘다 rvalue reference `T&&`에 연결될 수 있는 쪽이라는 공통점이 있다.

## glvalue는?

glvalue = lvalue + xvalue

둘의 공통점은, `특정 객체의 정체성을 나타낸다.`
a(특정 a객체), std::move(a) (여전히 특정 a객체)

10같은 prvalue는 특정 기존 객체를 찾아가는 표현식이 아니다.
xvalue가 왜 lvalue와 rvalue의 성질을 일부 동시에 갖는지 이렇게 확인가능

## 헷갈리는것 이름있는 T&&는 lvalue다.

```c++
void Func(std::string&& value)
{
}
```

value의 타입은 std::string&&이다.
그런데 함수 안에서 value라는 표현식은 lvalue다.
왜?

value라는 이름이 있고, 함수 안에서 같은 객체를 계속 특정해서 사용할 수 있기때문에

```c++
void Func(std::string&& value)
{
    OtherFunc(value);
}
```

그래서 위와같이 value를 그냥 넘기면 lvalue 판정이 된다.
다시 이동 가능한 값으로 넘기려면 std::move(value)처럼 해야한다.

```c++
void Func(std::string&& value)
{
    OtherFunc(std::move(value));
}
```

## 결론

```
이 표현식은 기존 객체인가?
        │
        ├─ 그렇다
        │    ↓
        │  이동 가능하다고 표시됐나?
        │    ├─ X → lvalue
        │    └─ O → xvalue
        │
        └─ 새 값을 만들어내는 표현식인가?
             ↓
           prvalue
```

## 문제

### 1

```c++
void Submit(std::vector<Enemy> enemies)
{
    EnemyManager.SetEnemies(enemies);
}
```

`enemies`는 함수에 값으로 들어왔고 더 이상 `Submit()`에서 사용할 필요가 없다고 하자.

`SetEnemies()`로 넘길 때 불필요한 대형 vector 복사를 줄이려면 왜 그냥 `enemies`를 넘기는 것보다 다음 형태가 의미가 있을지 설명해봐.

```c++
EnemyManager.SetEnemies(std::move(enemies));
```

-> lvalue상태로 넘어가면 `enemies`라는 vector를 전부 다 복사해야한다. 즉, `SetEnemies`의 매개변수 `enemies`를 만들기 위해서 큰 복사비용이 발생한다. 하지만 std::move를 통해 xvalue로 넘기게 되면 vector의 move constructor를 이용할 수 있다. 벡터는 보통 이동비용이 싸다. 기존 벡터가 가지고 있던 내부 버퍼의 소유권을 새 vector가 가져간다. 즉, 데이터가 만개라면 만개를 하나씩 복사하는게 아니라, 대략 포인터/size/capacity같은 내부 상태만 넘기는 것.

정리하자면

std::move는 enemies를 xvalue로 변환할 뿐이며, 실제 이동 여부는 받는 함수의 매개변수 형태에 따라 결정됨. SetEnemies가 값으로 받거나 std::vector<Enemy>&&로 받는 등 이동을 활용할 수 있는 형태라면 vector의 이동 연산이 선택되어 복사비용을 줄일 수 있다. const std::vector<Enemy>&로 받는다면 std::move를 붙여도 이동은 일어나지 않는다.

### 2

```c++
void Process(std::string&& name)
{
    Use(name);
}
```

`name`의 타입이 `std::string&&`인데도 `Use(name)`에서 `name`이 자동으로 rvalue 취급되지 않는 이유를 네 말로 설명해봐.

-> 매개변수 name은 Process의 함수블록안에서 name이라는 lvalue로써 존재하기 떄문, name이라는 특정 객체로 존재해서 name.size() 같은 작업들이 가능하다 그렇기 때문에 rvalue취급이 안되고 std::move(name)을 사용해서 xvalue로 만들어주어야 rvalue취급을 받을 수 있다.
