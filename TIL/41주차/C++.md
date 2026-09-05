# 1일차

## 목표

> 이 객체, 변수는 메모리 어딘가에 실제로 어떻게 존재하는가? 를 이해함

---

## 객체란 무엇인가?

> 특정 타입의 값을 표현하기 위해 일정 시간동안 존재하는 메모리 영역

```c++
int a = 10;
```

이라고 하면 보통 int값을 저장할 메모리가 필요하다.
많은 환경에서 int는 4바이트다. 4바이트 메모리를 int 객체 a가 사용한다고 생각할 수 있다.

---

## 메모리 주소

C++에서는 객체가 저장도니 주소를 직접 확인할 수 있다.
a라고 하면 &a는 a의 주소를 얻는다.
&는 주소연산자라고 한다.

---

## 타입

```c++
int hp = 100; // 4byte
double speed = 3.5; // 8byte
char grade = 'A'; // 1byte

sizeof(int); // 4byte
```

int가 반드시 4바이트라고 C++이 보장하지는 않는다. 플랫폼/컴파일러에 따라 달라질 수 있다. 그래서 sizeof로 크기를 확인하면 된다.

## 클래스

```c++
class Player
{
public:
    int hp;
    int mp;
};

int main()
{
    Player player;
}
```

player도 당연히 메모리를 차지한다.
다만 클래스는 Padding, Alignment, virtual함수등의 영향을 받을 수 있어서 단순히 멤버 크기를 다 더한값과 항상 같지는 않는다.
이건 뒤에 다시 다룸

### 멤버함수는 객체마다 복사되어 들어가는가?

```c++
class Player
{
public:
    int hp;
    void Attack()
    {
    }
};

Player a;
Player b;
Player c;
```

Player객체가 3개만들어지면 Attack() 함수 코드도 3개가 복사될까??
-> 일반적으로 그렇지 않다.

객체마다 저장해야 하는 것은 주로 그 객체 고유의 데이터

```
Player a
-> hp = 100

Player b
-> hp = 50
```

Attack()의 기계어 코드는 프로그램 코드 영역에 따로 존재하고 여러 객체가 같은 함수 코드를 사용한다.
그래서 sizeof(Player)를 계산할때 일반적인 비정적 멤버 함수의 코드 크기가 객체 크기에 그대로 추가되지는 않는다.

다만 virtual이 들어가면 개게에 구현상 추가 정보가 들어가는 경우가 많다. 나중에 vptr / vtable를 배울때 더 깊게 판다.

---

## 객체를 만든다는것은 생성자를 호출하는것과 같은말인가??

Player p;

라고했을때 단순히 `생성자가 호출됐다` 만 일어나는게 아니다.

개념적으로는

```
1. Player 객체를 저장할 공간이 준비된다.
2. 그 공간에서 Player 객체의 초기화가 진행된다.
3. 생성자 본문이 실행된다.
4. 완전히 생성된 객체를 사용할 수 있다.
```

> 객체가 존재하려면 그 객체의 상태를 표현할 저장공간이 필요하다.

---

## 결론

객체란??

> C++에서 객체는 특정 타입의 값을 표현하기 위해 일정 시간동안 존재하는 메모리 영역

---

# 2일차

## 목표

> 지역 객체와 Stack
> 동적 객체와 Heap
> Scope와 LifeTime
> 함수 호출 시 Stack에서 어떤 일이 일어나는지

---

## Stack

```c++
void Test()
{
    int hp = 100;
}
```

일반적인 구현에서는 hp같은 지역변수는 함수호출시 Stack 쪽 저장공간을 사용한다.
Test()함수가 끝나면 hp 객체의 수명이 끝난다.

### Stack이란?

일반적인 실행 환경에서 함수가 호출될 때마다 그 함수가 사용하는 지역 데이터 등을 저장하기 위한 영역이 만들어진다.
보통 이를 Stack Frame이라고 부른다.
여기서 지역변수는 무조건 Stack에 있다고 볼 수는 없다. 실제로는 Stack에 있을수도 CPU Register에 있을수도 아예 메모리에 저장되지 않을수도있다. 그래서 일반적으로 Stack에 저장된다로 기억하면 됨

---

## Heap

```c++
void Test()
{
    int* p = new int(100);
}
```

여기에는 객체가 두개 있다고 생각하는게 중요하다.

```
Stack영역에는 p가 들어가고
Heap영역에는 int = 100이 들어간다
```

이때 Test함수가 끝난다고 p가 자동으로 사라지지 않는다. delete로 삭제해주어야 한다. Heap객체는 남아있는데 기억하던 p는 사라지기 때문에 메모리 누수가 발생할 수 있다.

delete p;
를 한다고 해서 p 변수 자체가 사라지는게 아니고, 그 안에 들어있는 주소는 더 이상 유효한 객체를 가리키지 않는다.

### 댕글링 포인터

```c++
void Test()
{
    int* p = new int(100);
    delete p;
    std::cout << *p;
}
```

이건 문제가 있다. delete p이후 p에 접근했기 때문에
p안에는 주소값이 남아 있을수도 있지만 그 주소에 살아있는 int 객체가 없다. 이런 포인터를 댕글링 포인터 (Dangling Pointer)라고 한다.
그리고 \*p 처럼 역참조하면 Undefined Behavior이다.

꼭, delete p후에는 p = nullptr을 하는 습관을 들여야 한다.

```c++

int* p = new int(100);
int* q = p;
delete p;
p = nullptr;

p -> nullptr
q -> 0x5000
```

q는 여전히 dangling pointer

---

## Scope와 LifeTime의 차이

> Scope는 이름을 사용할 수 있는 영역

```c++
{
    int x = 10;
}
```

x라는 이름은 중괄호 내부에서만 사용할 수 있다.

> LifeTime은 객체가 생성되어 실제 객체로 존재하다가 수명이 끝날때까지의 시간

```c++
void Test()
{
    Player p;
}
```

```
대략
1. Test진입
2. Player 저장공간준비
3. Player 객체 생성
4. Lifetime 시작
5. p 사용가능
6. 함수 종료
7. Destructor
8. Lifetime 종료
```

나중에 자세히 다룸

### Heap 객체의 경우 훨씬 명확하게 분리됨

```c++
void Test()
{
    int* p = new int(100);
}
```

p의 Scope : Test 함수 내부
p의 Lifetime : Test 함수 내부
하지만 new int(100)으로 만든 객체는 delete하기전까지 존재가능
즉 포인터 변수의 Scope/Lifetime과 Heap 객체의 Lifetime은 완전히 다르다.

---

## 함수에서 포인터 반환

```c++
int* CreateNumber()
{
    int* p = new int(100);
    return p;
}
```

p 자체는 함수 종료 시 사라지지만 p가 가지고 있던 주소값을 반환했기 때문에 문제가 없음

```c++
int* number = CreateNumber();
```

호출자 number는 같은 Heap 객체의 주소를 받는다.
하지만 나중에 반드시 delete number;해주어야 하고 이는 까먹으면 메모리누수기때문에 현대 C++에서는 raw new/delete보다 스마트 포인터를 많이 쓴다.

---

# 3일차

## 목표

> 주소를 저장하고 그 주소의 객체에 접근하는 과정이해하기

---

## 포인터는 주소를 저장하는 객체

```c++
int hp = 100;
int* p = &hp;
```

p와 hp는 서로 다른 객체, hp에는 100이 저장되어있고, p에는 hp의 주소가 들어있다. \*를 통해 원본에 접근가능하고, 원본 수정이 가능하다

### 포인터 타입은 왜 필요한가?

> 주소를 따라갔을때 그 메모리를 어떤 타입의 객체로 해석해야 하는지 알아야 하기 때문이다.

```c++
int* p = &hp;
```

여기서 `*p를 하면 컴파일러는 여기에는 int객체가 있다고 보고 int 크기와 규칙에 맞춰 접근하자`고 판단한다.

대신 포인터자체의 크기와 가리키는 타입 크기는 다르다.
보통 포인터의 크기는 8바이트인데 (주소값때문) 보장되지는 않는다

p -> pointer object
hp -> pointee, 즉 p가 가리키는 객체

## 포인터 복사

```c++
int hp = 100;

int* p = &hp;
int* q = p; // 주소값이 복사됨

*q = 50;

hp == 50;
*p == 50;
*q == 50;
```

### Ownership

```c++
int* p = new int(100);
int* q = p;
```

p, q둘다 같은 Heap객체를 가리킨다.
여기서 delete p;를하면 Heap객체는 사라진다.
하지만 p, q 는 둘다 예전주소를 가지고 있다.
이 객체를 누가 삭제할 책임이 있는가? -> 이게 Resource Ownership이고, 이후 RAII와 스마트 포인터로 이어진다.

---

## 포인터 변수의 주소도 있다.

```c++
int hp = 100;
int* p = &hp;
&p; // p도 객체라서 당연히 자신의 주소가 있다.
```

```c++
void A(int* p)
{
    p = nullptr; // 매개변수 p를 바꾸는것이라 원본에는 영향없음
}

void B(int* p)
{
    *p = 0; // 포인터가 가리키는 int객체를 바꿈
}

```

---

# 4일차

## 목표

> 레퍼런스를 이해한다.

---

## 레퍼런스

```c++
int hp = 100;
int& ref = hp;
```

hp -> 원래 int객체
ref -> hp객체를 참조하는 reference
중요한건 ref는 별도의 int값 복사본이 아니다. 그냥 별칭같은거임

레퍼런스는 생성시 반드시 초기화해줘야함
레퍼런스는 한번 지정하면 대상을 못바꿈, 포인터는 가능

---

## 포인터 전달과 레퍼런스 전달

```c++
void Damage(int* hp)
{
    *hp -= 10;
}
Damage(&playerHp);

void Damage2(int& hp)
{
    hp -= 10;
}
Damage2(playerHp);
```

둘다 객체 수정이 가능하다. 차이는 인터페이스에 있는데
Pointer버전은 Damage(nullptr)이 들어올 수 있다.
레퍼런스 버전은 일반적으로 이 함수는 유효한 int 객체를 받아야 한다는 의도가 더 강하다

---

## Dangling Reference

```c++
int& GetNumber()
{
    int x = 100;
    return x;
}
```

이 코드는 문제가 있다. x는 지역 객체다.
함수가 끝나면 x의 LifeTime이 종료된다. 그래서
int& ref = GetNumber();
하면 ref는 이미 수명이 끝난 객체를 참조하게 된다 이를 Dangling Reference라고 한다.
ref를 읽는건 Undefined Behavior

---

## 레퍼런스는 실제로 메모리를 전혀 안먹는가?

언어수준에서는 레퍼런스를 기존 객체를 참조하는 별칭정도로 기억하면 된다.

하지만 컴파일러가 내부적으로 레퍼런스를 구현할 때 주소를 저장하는 식으로 구현할 수 있다.

```c++
struct Test
{
    int& ref;
}
```

같은 경우 실제 객체 크기에 Reference를 구현하기 위한 저장 공간이 반영되는 경우가 있다.
따라서

> 레퍼런스는 무조건 메모리를 차지하지 않는다 라고 단정하면 안됨

---

# 5일차

## 목표

> const 포인터 구분하기

---

## const Reference

```c++
int hp = 100;
const int& ref = hp;
```

ref = 50하면 에러, 왜냐면 ref가 const int\*에 대한 레퍼런스라서
hp = 50은 가능하다

---

## const Pointer

```
int a = 10;
int b = 20;
int c = 30;

const int* p = &a;
int* const p2 = &b;
const int* const p3 = &c;
```

### const int\*

이거는 int*에 const가 걸린거 그래서
*p = 30; 이건 컴파일에러가 난다.
p = &b -> 이거는 가능

### int\* const

얘는 판대임 p에 const가 걸린거라
p = &b -> 불가능
\*p = 30 -> 가능

### const int\* const p

이거는 당연히 둘다 불가

### 원래 객체가 const인경우

```c++
const int hp = 100;
```

이 객체를 일반 int*로 가리키게 할 수 있을까??
-> 안됨 왜냐면 *p = 50; 을 통해 원래 const객체를 수정할 수도 있기 때문 그래서

const int\* p = &hp;
이렇게 받아야 함

## 이게 왜중요?

```c++
void PrintPlayer(const Player* player)
{
    // player = nullptr; -> 가능하다
    player->hp = 0; // 불가
}
```

const Player\*이기 때문에 이 함수 선언 자체가 나는 이 포인터를 통해 Player객체를 수정하지 않을것이다 라는 선언과 같다.

void Print(const Player& player);
void Print(const Player\* player);

둘다 같은 의미, 물론 Pointer버전은 Print(nullptr) 가능성이 있다.
레퍼런스 버전은 일반적으로 유효한 객체가 필요하다.
const를 쓰면 API의 의도를 표현할 수 있다.

---

# 6일차

## 목표

> 함수에 인자를 넘길 때 무엇이 복사되고, 무엇이 원본과 연결되는지를 구분한다.

---

## 매개변수도 객체다.

```
void Damage(int hp)
```

여기서 `hp`는 함수안에서 새로 존재하는 `int`객체다

```c++
int playerHp = 100;
Damage(playerHp);
```

에는 서로다른 `int`객체가 있다.

playerHp = 100
hp = 100

값만 같음, 서로 다른 객체

---

## 결론

```c++
void Func(Player p) // 복사본을 받음, 원본 수정 안됨
void Func(Player& p) // 원본을 복사없이 읽기만 함, 큰 객체 전달할 때 자주 씀
void Func(Player* p) // 주소값이 복사됨, 그래도 같은 원본 객체를 가리킬 수 있음. nullptr가능할때 주로 사용
```

포인터 매개변수

```
p = other; // 함수 내부 포인터 변수만 다른 주소를 가리킴
p->hp = 50; // 실제 원본 객체 수정
delete p; // p가 가리키는 동적 객체의 수명 종료
```

---

```
Player* ptr = new Player();
Func(ptr);

void Func(Player* p) {}
```

여기서 ptr과 p는 서로 다른 포인터 객체
호출자 ptr, &ptr = 0x1000, 저장된 값 : 0x5000
함수 p, &p = 0x2000, 저장된 값 : 0x5000

ptr안에 들어있던 `Player객체의 주소 0x5000`이 p에 복사됨
그래서 p = nullptr하면 함수 내부 포인터 p에 저장된 0x5000만 없어지고 호출자의 ptr은 여전히 0x5000을 들고있다.

반면 delete p; 하면 p 안에 저장된 0x5000에 있는 Player객체를 파괴한다.

```
p -> 0x5000
ptr -> 0x5000

0x5000의 Player 객체 수명 종료
```

함수가 끝난 뒤 호출자의 ptr자체는 남아있지만
ptr -> 이미 파괴된 Player, 그래서 ptr은 dangling pointer가 된다.

---

#
