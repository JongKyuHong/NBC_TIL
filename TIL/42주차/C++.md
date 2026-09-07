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
