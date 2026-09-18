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

---

# Move Semantics

## 목표

> 왜 Move가 Copy보다 싸질 수 있는지
> Move Constructor / Move Assignment가 언제 선택되는지
> `std::move`가 실제로 하는 일
> 이동 후 원본 객체를 어떻게 다뤄야 하는지
> Move가 항상 싸거나 항상 발생하는 것은 아닌 이유
> 게임 코드에서 어떤 상황에 Move를 쓰는지

## Move가 필요한 이유

예를들어 이런 벡터가 있다고 치자

```c++
std::vector<Enemy> enemies;
```

내부적으로 vector 객체 자체에는 대략

```
vector 객체
┌────────────────┐
│ Heap 주소      │ ───────▶ Enemy Enemy Enemy Enemy ...
│ size           │
│ capacity       │
└────────────────┘
```

이런 정보가 있고, 실제 수많은 Enemy들은 보통 Heap쪽 버퍼에 있다.
이걸 복사하면 새 버퍼를 만들고 각각의 Enemy를 복사해야한다.

```c++
std::vector<Enemy> copy = enemies;
```

데이터가 커질수록 굉장히 비싸진다.

## Move는 기존 자원을 재사용할 수 있다.

```c++
std::vector<Enemy> moved = std::move(enemies);
```

개념적으로

```
이동 전

enemies
   │
   ▼
[Enemy][Enemy][Enemy][Enemy]


이동 후

enemies
   │
   └── 더 이상 기존 버퍼 소유 X

moved
   │
   ▼
[Enemy][Enemy][Enemy][Enemy]
```

즉, 모든 Enemy를 하나하나 복사하지 않고 버퍼의 소유권만 넘길 수 있다.
이게 흔히말하는

> Move로 비싼 Deep Copy를 피할 수 있다는 실제 의미

## 중요한것! std::move가 데이터를 옮기는건 아니다.

```
std::move(enemies);
```

이 move자체는 Enemy들을 옮기지 않는다.
실제로 하는 일은

> enemies라는 lvalue표현식을 xvalue로 변환한다.

자원이 이동가능한 상태로 만들겠다.
컴파일러가 보고 -> 아 이객체의 자원을 가져가도 되는 상황일 수 있겠구나 판단
T&&나 Move Constructor를 선택할 수 있게 된다.
실제 이동은 이동생성자나 이동대입연산자가 한다.

## Move Constructor

```c++
class Buffer
{
public:
    Buffer(Buffer&& other)
    {
        // other의 자원을 가져오는 로직
    }
};

Buffer a;
Buffer b = std::move(a);
```

여기서 b는 새 객체

따라서

```
MoveConstructor는
-> 기존 객체 a를 이용해 새로운 객체 b를 생성한다.
```

## Move Assignment

```c++
Buffer a;
Buffer b;
b = std::move(a);
```

이런 새로운 b를 만드는게 아니다. 기존 b의 상태를 교체한다.
따라서

```
Move Assignment
-> 기존 b의 자원 정리
-> a의 자원을 b가 가져감
```

이런 과정이 필요할 수 있다.

## 직접 자원을 관리한다면 Move는 어떻게 생겼는가

```c++
class Buffer
{
private:
    int* data = nullptr;

public:
    Buffer()
        : data(new int[1000])
    {
    }

    ~Buffer()
    {
        delete[] data;
    }

    Buffer(Buffer&& other) noexcept
        : data(other.data)
    {
        other.data = nullptr;
    }
};
```

data = other.data;
-> 주소를 가져온다.

other.data = nullptr;
-> 원본이 더 이상 그 자원을 소유하지 않도록 만든다.

이걸 안해주면 나중에 Double Delete가 날 수 있다.

Move는 단순 주소 복사가 아니라

> Ownership까지 새 객체 쪽으로 이전하는 설계

실제 vector, string, unique_ptr 같은 타입이 이걸 대신 해준다.

즉, MoveSemantics를 배우는 이유는

> Move Constructor를 매일 직접 짜려고 배우는 게 아니라, STL/스마트 포인터/게임 데이터를 이동시킬 때 어떤 일이 일어나는지 이해하기 위해서

## moved-from object

```c++
std::vector<int> a = {1, 2, 3};

std::vector<int> b = std::move(a);
```

여기서 a는 여전히 존재한다. C++에서 moved-from객체는 일반적으로

> valid but unspecified state

에있다고 표현한다.

- 객체 자체는 유효함
- 소멸 가능
- 다시 값을 대입할 수 있음
- 하지만 기존 내용이 정확히 무엇인지는 일반적으로 가정하지 말아야 함

예를들면

```
a = {4, 5, 6} -> 이렇게 값을 다시 대입하는건 정상

if (a.empty()) -> Move직후 반드시 true라고 모든 타입에 대해 일반화하면 안됨
```

> moved-from 객체는 파괴하거나, 새로운 값을 다시 넣어서 사용한다.

## std::move를 썼다고 반드시 Move되는게 아니다

이게 무슨 말인가 싶을 수 있는데
std::move(a)는 단순히 xvalue를 만들뿐이다.

실제 받는쪽이

```c++
void Func(const Data& data);
```

밖에없다면

```c++
Func(std::move(a));
```

를 해도 const Data&로 받을 수 있다.
어떤 overload가 존재하는지가 중요하다.

## const객체는 주의

```c++
const std::string name = "Player";

std::string copy = std::move(name);
```

std::move(name)의 타입 성질은 const std::string&& 쪽이된다.
일반적인 Move Constructor는

std::string(std::string&&)

처럼 non-const rvalue reference를 받는다.
왜 const를 못받을까?

Move는 보통 원본 객체의 내부 상태를 변경해야 하기 때문

```
원본 버퍼 주소 제거
-> nullptr 등으로 변경
```

원본이 const면 바꿀 수 없다.
그래서 이런 상황에서는 Copy Constructor가 선택될 수 있다.

> std::move를 썼다고 반드시 Move되는게 아니다.

## unique_ptr에서 Move가 특히 명확하다

```
std::unique_ptr<Weapon> weaponA =
    std::make_unique<Weapon>();
```

unique_ptr은 단독 Ownership이므로 복사가 안된다.

```c++
auto weaponB = std::move(weaponA);
```

하면 Ownership을 이전한다.

## 모든곳에 std::move를 붙이면?

```c++
void Render(const Player& player);
```

읽기만한 객체에 Render(std::move(Player));
를 붙일 이유가 없다.
const&로 읽기만 할거라면 이동하지 않는다.

또,

```c++
std::string BuildName()
{
    std::string result = "Player";
    return result;
}
```

이런 반환코드에서 지역 객체를 값으로 반환할때는 그냥 return result를 해라 std::move를 쓰지말고, Copy Elision최적화를 오히려 방해함

## Move가 항상 O(1)은 아니다

Move = 무조건 포인터 하나 넘겨서 O(1)
도 아님

std::vector 같은 타입은 일반적으로 내부 버퍼를 넘길 수 있어서 매우 싸지만, 타입에 따라 Move 구현이 다르다.
예를들어 작은 데이터를 객체 내부에 직접 저장하는 타입이라면 Move도 실제 데이터를 복사할 수 있다.
std::string도 Small String Optimization때문에 작은 문자열은 내부 배열을 직접 옮기거나 복사하는 식으로 구현이 가능하다.

> Move는 복사보다 싸게 구현할 수 있는 기회를 제공한다

## noexcept가 Move에서 중요한 이유

`std::vector<MyType>` 가 재할당 한다고 하자
기존 요소들을 새 버퍼로 옮겨야 한다.
MyType의 Move Constructor가 예외를 던질 가능성이 있으면, vector는 강한 예외 안정성을 위해 Move 대신 Copy를 선택할 수도 있다.

그래서 자원만 넘기는 Move Constructor라면

```c++
MyType(MyType&& other) noexcept;
```

처럼 noexcept로 만드는게 중요하다.

# Type Deduction / Forwarding Reference / std::forward

## 목표

> 아래 코드 이해하기

```c++
template <typename T>
void Add(T&& value)
{
    Container.Add(std::forward<T>(value));
}
```

T&&랑 forward가 뭔지 배우기

> 이 함수는 lvalue가 들어오면 lvalue로, rvalue가 들어오면 rvalue로 다음 함수에 그대로 전달하려고 이렇게 작성한 것이다.

## Type Deduction

```c++
template<typename T>
void Func(T value){}

int x = 10;
Func(x);
```

컴파일러가 x를 보고 T = int라고 생각한다
이게 Type Deduction(타입 추론)이다.

## Reference를 받으면 const도 중요해진다

만약에

```c++
const int x = 10;
Func(x);
```

라면? 이번에는 T는

```c++
T = const int
```

로 추론된다.
즉 Reference 기반 Type Deduction에서는 원본 const성질이 보존될 수 있다.

## T&&

보통 rvalue reference로 알고있음
그런데 템플릿에서는 조금 다르게 동작함

```c++
template<typename T>
void Func(T&& value)
{
}
```

T가 호출 인자를 보고 추론되는 상황이라면 T&&를 Forwarding Reference(전달 참조)라고 부른다.
이거는 rvalue, lvalue 둘다 받는다.

### 왜 lvalue도 T&&에 들어갈 수 있나?

```c++
template <typename T>
void Func(T&& value)
{
}

Enemy enemy;

Func(enemy);
```

enemy는 lvalue다.
그런데 특별하게 컴파일러가

```c++
T = Enemy&
```

로 추론한다.
Forwarding Reference에서는 T자체가 Reference 타입으로 추론될 수 있다.

```c++
// 원래 함수가
T&&
// 이렇게 되어있는데 T자리에 Enemy&를 넣으면

Enemy& &&
-> Enemy&
// 이렇게 된다 Reference가 두개 붙어있다.
```

이것을 하나로 정리하는게 Reference Collapsing(참조 축약)이다.

## Reference Collapsing

규칙은 이것뿐이다.

```
& + & -> &
& + && -> &
&& + & -> &
&& + && -> &&
```

## rvalue를 넣으면?

```c++
Func(Enemy{});
```

Enemy{}의 경우 rvalue이다.

```c++
T = Enemy
```

로 추론된다. 그러면 `T&&`는 그대로
`Enemy&&`로 추론된다.

이게 Forwarding Reference의 핵심

## 왜 이런게 필요한가?

게임 엔진에 범용 Container 함수가 있다고 했을때

```c++
template <typename T>
void Add(T&& value)
{
    Container.Add(value);
}
```

호출자가

```c++
Enemy enemy;

Add(enemy);
```

lvalue로 호출할수도

```c++
Add(Enemy{});
```

rvalue로 호출할수도 있다.

첫번째는 기존 enemy가 앞으로 사용될수도 있으므로 함부로 Move하면 안되고
두번째는 임시 객체니까 함수 호출 뒤사라진다. 그러면 가능하면 Move를 해서 Copy를 줄이고 싶다

```
Add(enemy)
→ 기존 객체이므로 lvalue로 전달

Add(Enemy{})
→ 임시 객체이므로 rvalue로 전달
```

**그런데 함수안에서는 value가 무조건 lvalue다.**

rvalue호출을 해도 매개변수 탓에 무조건 lvalue로 판정된다.
그렇다고 std::move(value)를 쓰면?

```c++
template <typename T>
void Add(T&& value)
{
    Container.Add(std::move(value));
}
```

rvalue가 들어왔을때는 Move를 하면 되니까 상관없지만
lvalue의 경우에는 Move가 제약적이다.

그래서 forward를 쓴다.

### std::forward

```
원래 lvalue였으면 → lvalue 유지
원래 rvalue였으면 → rvalue 유지
```

```c++
template <typename T>
void Add(T&& value)
{
    Container.Add(std::forward<T>(value));
}
```

forward를 쓰면 실제로 저렇게 쓸 수 있다.

```c++
Enemy enemy;
Add(enemy);
```

아까 `T = Enemy&`로 추론되었다.
그래서

```c++
std::forward<T>(value);
```

forward는 value를 lvalue로 전달한다.

### std::move와의 차이

```c++
std::move(value);
```

> 나는 이제 이 객체를 이동 가능한 대상으로 취급하겠다.

```c++
std::forward<T>(value);
```

> 이 값이 처음 들어왔을 때의 lvalue/rvalue 성질을 유지해서 다음 함수로 보내겠다.

## 이것을 Perfect Forwarding이라고 한다.

```c++
template <typename T>
void Add(T&& value)
{
    Container.Add(std::forward<T>(value));
}
```

위 함수처럼 중간 Wrapper가 원래 인자의 성질을 보존하면서 다음 함수로 넘기는것을 Perfect Forwarding(완벽 전달)

### 실 사례

`emplace_back`

대표적인 실사례이다.

```c++
Enemies.emplace_back(name, hp, position);
```

`emplace_back`은 내부에서 `Enemy`를 직접 생성해야 한다.

대충 개념적으로

```c++
template <typename... Args>
void emplace_back(Args&&... args)
{
    // vector 내부 메모리에 Enemy 직접 생성
    Enemy(std::forward<Args>(args)...);
}
```

처럼 생각할 수 있다.
왜 `forward`가 필요하냐면 각 생성자 인자가

- lvalue일수도
- rvalue일수도
- const일수도 있고

그 성질을 `Enemy` 생성자까지 그대로 전달해야 하기 때문이다.

## 일반 T&&와 Forwarding Reference는 다르다

```c++
void Func(Enemy&& enemy);
```

위는 그냥 rvalue reference다.

```c++
Enemy e;

Func(e);        // 불가능
Func(Enemy{});  // 가능
```

템플릿이 붙어서 T추론이 가능하면 Forwarding Reference가 된다.

## 정리

### Type Deduction

컴파일러가 함수 호출 인자를 보고 `T`를 결정한다.

### Forwarding Reference

```c++
template <typename T>
void Func(T&& value);
```

T가 추론되는 상황에서

```
lvalue입력 -> T가 T& 형태로 추론
rvalue입력 -> T가 일반 T 형태로 추론
```

해서 둘다 받을 수 있다.

### Reference Collapsing

```
하나라도 &가 있으면 결과는 &
둘다 &&일때만 &&
```

그래서 lvalue도 `T&&`함수에 들어갈 수 있다.

### std::forward

Wrapper 함수에서

```
원래 lvalue -> lvalue
원래 rvalue -> rvalue
```

로 다음 함수에 전달한다.

### 왜 배우는가??

```c++
emplace_back
make_unique
Factory
Container API
Task System
```

같은 Modern C++코드를 볼때 왜 `T&&`과 `std::forward`가 같이 나오는지 이해하기 위함
