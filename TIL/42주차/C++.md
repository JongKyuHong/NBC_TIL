# 7일차

## 목표

> 동적 메모리 / Memory Leak / Dangling Pointer / Double Delete 정리

---

## new가 하는 일

```c++
Player* p = new Player();
```

### new Player()는 무엇인가?

1. Player객체가 들어갈 동적 메모리를 확보한다.
2. 그 메모리에 Player 객체를 생성한다.
3. 생성된 Player 객체의 메모리 주소를 반환한다.

예를들어 Heap의 0x5000의 Player가 만들어졌다고 하면 new Player() 의 결과는 개념적으로 0x5000이라는 Player객체의 주소

### p는 무엇인가?

p는 Player\* 타입의 포인터 변수 즉,

> Player객체의 주소를 저장할 수 있는 변수

new Player()가 반환한 0x5000을 p에 저장한다.
여기서는 주소를 명확히 구분해야 한다.

예를들어

```
&p = 0x1000
p = 0x5000
```

이라고 하면

- &p -> 포인터 변수 p 자체가 저장된 주소
- p -> Player 객체가 저장된 주소
- \*p -> 0x5000에 존재하는 Player객체

### delete p는 무엇을 삭제하는가?

```c++
delete p;
```

이건 포인터 변수 p를 삭제하는게 아니다.

delete 전

```
p [0x5000] -> 0x5000 [Player 객체]
```

delete p후

```
p [0x5000] -> 0x5000 [Player객체는 더이상 존재하지 않음]
```

중요한건 p 변수 자체는 아직 존재한다는 점

## Dangling Pointer

```c++
Player* p = new Player();

delete p;
```

delete p 이후

- p 포인터 변수 -> 아직 살아 있음
- p가 들고 있던 0x5000 -> 그대로 남아 있을 수 있음
- 0x5000의 Player 객체 -> 이미 파괴됨

p는 이미 수명이 끝난 객체의 주소를 들고 있는 포인터
이게 Dangling Pointer다.

p안에는 주소가 있긴 하지만, 그 주소에 살아있는 Player객체가 없기 때문이다.

## nullptr

delete p는 p가 가리키는 동적 Player객체를 파괴하는 문법이였다.
nullptr은 포인터 변수p안에 지정된 주소값을 nullptr로 변경한다.

### Memory Leak

```
Player* p = new Player();

p = nullptr;
```

이런식으로 delete p를 하지않고 nullptr을 넣으면 문제가 또 있다.
포인터 변수 p의 주소값에 nullptr로 덮어씌워서 기존 new Player에 도달할수가없다.
new Player는 살아있는데 지울수도 없으므로 이게 메모리 leak를 발생시킨다.

## 포인터를 복사하면?

```c++
Player* p = new Player();
Player* q = p;
```

p안에 있는 0x5000을 q가 같이 공유한다.
같은 하나의 Player객체를 두 포인터가 가리키는 꼴이 된다.

delete p를하면 Player객체가 없어지니까 둘다 댕글링 포인터가 되고
p = nullptr하면 q는 안바뀌고 p만 바뀐다.

## Double Delete

```
Player* p = new Player();

delete p;
delete p;
```

-> Double Delete
-> Undefined Behavior

### 포인터 두개여도 문제

```
Player* p = new Player();
Player* q = p;

delete p;
delete q;
```

## new[] / delete[]

```c++
int* arr = new int[10];

delete[] arr; // 해제할때 delete[]로
```

## 클래스에서는 생성자 / 소멸자도 연결됨

```c++
class Player
{
public:
    Player()
    {
        std::cout << "생성\n";
    }

    ~Player()
    {
        std::cout << "소멸\n";
    }
};
```

이런 클래스가 있다고 했을때

```c++
Player* p = new Player();
```

하면 Player의 생성자가 실행된다.

그다음

```c++
delete p;
```

를하면 Player 소멸자가 실행되고 메모리가 해제된다.

---

# 8일차

## 목표

> 클래스는 객체가 어떤 데이터를 가지고, 어떤 함수를 사용할 수 있는지 정의하는 타입이고, 실제 객체마다 멤버 변수는 따로 존재하지만 멤버 함수 코드는 보통 공유된다.

## class는 객체 자체가 아니다

```c++
class Player
{
public:
    int hp = 100;
}
```

이건 `Player`라는 타입을 정의한 것
아직 실제 `Player`객체가 만들어진 건 아니다.

실제 객체는

```c++
Player a;
Player b;
```

여기서 만들어진다.

---

### 멤버 변수/함수

```c++
class Player
{
public:
    int hp;
    void Damage()
    {
        hp -= 10;
    }
};

Player a;
Player b;
Player c;
```

멤버변수는 당연히 객체마다 존재하지만
멤버함수는 객체마다 복사되는것이 아니다. (기억하기) -> static멤버도 객체마다 따로 있지 않음

Damage()안의 hp -= 10은 이게 a.hp인지 b.hp인지 어떻게 아는가? -> this포인터

---

# 9일차

## 목표

> 생성자는 객체가 만들어질 때 그 객체를 올바른 초기 상태를 만드는 함수

---

## 생성자

```
1. Player 객체를 위한 저장 공간 준비
2. 멤버 초기화
3. 생성자 본문 실행
4. 완전히 생성된 Player 사용
```

> 생성자 본문이 실행되기 전에 이미 멤버 초기화 단계가 있다.

### 초기화 리스트

```c++
class Player
{
public:
    int hp;

    Player()
        : hp(100)
    {
    }
};
```

: hp(100)
이 부분이 멤버 초기화 리스트이다.
이건 hp를 생성자 본문에서 나중에 대입하는게 아니라, 멤버 초기화 단계에서 바로 100으로 초기화한다.

### 클래스 멤버 객체에서 차이

```c++
class Weapon
{
public:
    Weapon()
    {
        std::cout << "Weapon Default\n";
    }

    Weapon(int damage)
    {
        std::cout << "Weapon Damage\n";
    }
};

class Player
{
public:
    Weapon weapon;

    Player()
    {
        weapon = Weapon(100);
    }
};
```

이 경우 `weapon` 멤버는 생성자 본문에 들어오기 전에 이미 먼저 생성되어야 한다.

```
1. weapon 기본 생성자 실행
2. Player 생성자 본문 진입
3. Weapon(100) 임시 객체 생성
4. weapon에 대입
```

```c++
class Player
{
public:
    Weapon weapon;

    Player()
        : weapon(100)
    {
    }
};
```

이면

```
1. weapon을 처음부터 Weapon(100)으로 생성
2. Player 생성자 본문 실행
```

이런 차이가 있다.
그래서 클래스 타입 멤버는 초기화 리스트를 사용하는게 중요한 경우가 많다.

---

### const 멤버는 초기화 리스트가 필요하다.

```c++
class Player
{
public:
    const int maxHp;

    Player()
    {
        maxHp = 100;
    }
};
```

이건 안된다.
maxHP는 const라서 한 번 초기화된 뒤 대입할 수 없기때문에
그래서

```c++
class Player
{
public:
    const int maxHp;

    Player()
        : maxHp(100)
    {
    }
};
```

이렇게 초기화 리스트 사용해야된다.

---

### Reference 멤버도 마찬가지

```c++
class Player
{
public:
    int& hpRef;

    Player(int& hp)
    {
        hpRef = hp;
    }
};
```

얘도 안됨 레퍼런스는 생성될 때 바로 참조 대상을 정해야 하기때문

```c++
class Player
{
public:
    int& hpRef;

    Player(int& hp) : hpRef(hp)
    {
    }
};
```

초기화해야한다.

---

### 멤버 초기화 순서

```c++
class Player
{
public:
    int hp;
    int mp;

    Player()
        : mp(50), hp(100)
    {
    }
};
```

초기화 리스트에는 mp를 먼저 썼다.
그러면 mp가 먼저 초기화될까?
아님
멤버는 클래스에 선언된 순서대로 초기화된다.
그렇기 때문에 초기화 리스트 순서도 보통 멤버 선언 순서와 같은 순서대로 작성하는게 좋다.

---

# 10일차

## 목표

> 소멸자는 객체의 수명이 끝날때 호출되고, 객체가 가지고 있던 자원을 정리하는데 사용된다.

## 여러 객체는 역순으로 해제된다.

```c++
void Test()
{
    Player a;
    Player b;
    Player c;
}
```

생성은 a -> b -> c
파괴는 c -> b -> a
같은 Scope의 지역 객체는 일반적으로 생성의 역순으로 파괴된다.

---

# 11일차

## 목표

> 어떤 클래스가 다른 클래스 객체를 멤버로 가지고 있을 때, 누가 먼저 생성되고 누가 먼저 파괴되는지를 코드 보고 바로 판단하기

---

## 멤버 객체는 바깥 객체보다 먼저 생성된다.

```c++
class Weapon
{
public:
    Weapon()
    {
        std::cout << "Weapon 생성\n";
    }

    ~Weapon()
    {
        std::cout << "Weapon 소멸\n";
    }
};

class Player
{
public:
    Weapon weapon;

    Player()
    {
        std::cout << "Player 생성\n";
    }

    ~Player()
    {
        std::cout << "Player 소멸\n";
    }
};
```

이제

```c++
Player p;
```

를 만들면 순서는

```
Weapon 생성
Player 생성
```

순서로 된다. Player 생성자 본문을 실행하려면, 그 안에 포함된 weapon객체가 먼저 정상적으로 만들어져 있어야 하기 때문

---

### 파괴는 반대 순서

Player p;가 Scope를 벗어나면

Player 소멸자 본문 실행 -> weapon 소멸

즉,

```
생성
멤버 -> 바깥 객체

파괴
바깥 객체 -> 멤버
```

## 핵심 정리

```
멤버 객체 생성
-> 멤버 선언 순서

Player 생성자 본문
-> 멤버 객체들이 다 생성된 뒤 실행

Player 파괴
-> Player 소멸자 본문
-> 멤버 객체를 선언 역순으로 파괴

Weapon weapon;
-> 실제 멤버 객체
-> Player와 수명이 묶임

Weapon* weapon;
-> 주소만 저장
-> pointee 수명은 별개
```

---

# 12일차

## 목표

> 객체가 정확히 언제 생성되는지
> 중첩 {} Scope에서 언제 파괴되는지
> 함수가 다른 함수를 호출할 때 각 함수의 지역 객체가 언제 사라지는지
> 전체 생성/소멸 순서를 코드 실행 순서대로 추적하기

---

## Scope

같은 Scope에서는 역순으로 파괴

### 중첩 Scope

```c++
void Test()
{
    Player a;

    {
        Player b;
        Player c;
    }

    Player d;
}
```

```
a 생성
b 생성
c 생성
c 소멸
b 소멸
d 생성
d 소멸
a 소멸
```

---

# 13일차

static 객체 / static 멤버

## 목표

> 일반 지역 객체와 static 지역 객체의 수명 차이
> 일반 멤버와 static 멤버의 차이
> static이 `객체마다 하나`인지 `타입 전체에 하나`인지 판단하기




