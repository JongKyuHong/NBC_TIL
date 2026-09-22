# Rule of 3 / 5 / 0 + Copy Elision

## 목표

> 자원을 직접 관리하는 클래스를 만들 때 왜 Copy/Move/Destructor를 함께 생각해야 하는가?
> 함수에서 큰 객체를 값으로 반환해도 왜 실제 Copy가 발생하지 않을 수 있는가?

---

## Rule of 3

```c++
class Buffer
{
private:
    int* Data;

public:
    Buffer()
        : Data(new int[100])
    {
    }

    ~Buffer()
    {
        delete[] Data;
    }
};
```

예를들어 이런 오래된 스타일의 클래스가 있다고 할때, 이 클래스는 `Data`라는 Heap 메모리를 직접 소유하고 있다. 여기서 문제는 복사할때이다

```c++
Buffer a;
Buffer b = a;
```

기본 복사를 그대로 사용하면 `Data` 주소만 복사돼서 a.data와 b.data가 같은 힙메모리를 가리킬 수 있다.
그러면 둘 다 소멸할때 같은 메모리를 delete[]하려고 한다. 그래서 이런 클래스는 보통

- Destructor
- Copy Constructor
- Copy Assignment

이 셋을 검토해야 한다.

> 내가 자원의 소멸 방법을 직접 정의했다는 건, 복사될 때 그 자원을 어떻게 처리할지도 직접 고민해야 할 가능성이 높다.

## Copy Constructor와 Copy Assignment 둘다 필요한가?

- Copy Constructor

```c++
Buffer b = a;
```

-> 새 b를 a로부터 생성

- Copy Assignment

```c++
Buffer a;
Buffer b;

b = a;
```

-> 이미 존재하는 b를 a의 상태로 변경

이전에 배웠듯이 Copy Assignment는 기존 b가 이미 자기 Resource를 가지고 있을 수 있기때문에 이를 어떻게 처리할지, A를 복사할지, 둘이 독립적으로 가져야 하는지를 결정해야 한다

## Rule of 5

C++11 이후 Move가 생겼다.

그래서 Resource를 직접 관리하는 클래스라면

- Destructor
- Copy Constructor
- Copy Assignment
- Move Constructor
- Move Assignment

다섯개를 같이 검토해야 한다.
이게 rule of 5이며

왜 Move까지 생각해야 하냐면

```c++
Buffer b = std::move(a);
```

일때 굳이 큰 Resource를 복사하지 않고 Ownership만 이전할 수 있기 때문이다.

## Rule of Zero

```c++
class LevelData
{
public:
    std::string Name;
    std::vector<EnemyData> Enemies;
}
```

에서는 `string`, `vector`가 이미

- Destructor
- Copy
- Move

를 올바르게 구현하고 있다.
그래서 `LevelData`가 직접

소멸자, 복사 생성자, 이동 생성자를 만들 필요가 없다

이게 Rule of Zero다

> 자원관리는 이미 잘 만들어진 RAII 타입에게 맡기고, 내 클래스에서는 특별 멤버함수를 직접 작성하지 않는 것을 우선한다.

## unique_ptr이 있으면?

```c++
class LevelData
{
public:
    std::string Name;
    std::vector<EnemyData> Enemies;
    std::unique_ptr<NavigationData> Navigation;
};
```

이 클래스는 기본적으로 복사가 불가능하다. unique_ptr 자체가 복사가 불가능하기 때문
이때 Move는 가능한데, `LevelData를 복사 가능하게 만들어야 하나?` 가 중요한 판단인다.
꼭 필요하지 않다면 굳이 직접 Deep Copy를 구현하지 않고 복사 불가능 타입으로 두는게 더 좋은 설계일 수 있다.

## Copy Elision

Copy Elision은 복사 생략이라는 최적화 규칙

```
함수 내부 enemy 생성
        ↓
반환 객체로 Copy/Move
        ↓
enemy 파괴
```

를 Copy Elision을 하면

```
호출자가 받을 최종 저장 공간에 처음부터 enemy를 직접 생성
```

즉 Copy도 Move도 아예 안일어난다.

## NRVO

Named Return Value Optimization이라고 한다.
이름있는 리턴 밸류 최적화

```c++
Enemy CreateEnemy()
{
    Enemy enemy;
    return enemy;
}
```

여기서 이름있는 지역 객체 enemy를 반환할때 적용되는 최적화

```c++
Enemy result = CreateEnemy();
```

라고 했을때 함수 내부 `enemy`를 따로 만들었다가 옮기지 않고, 아예 result가 들어갈 위치에 직접 생성할 수 있다.

## RVO

```c++
Enemy CreateEnemy()
{
    return Enemy{};
}
```

처럼 임시 객체를 바로 반환하는 경우는 보통 RVO - Return Value Optimization이라고 부른다.
C++17이후 중간 Copy/Move없이 직접 생성되도록 언어 규칙이 보장하는 경우가 있다.

## 게임에서의 활용

```c++
MeshData BuildMesh()
{
    MeshData data;

    // 수십 MB의 vertex/index 데이터 생성

    return data;
}

MeshData mesh = BuildMesh();
```

이걸 보고 MeshData가 크니까 반환할때 수십 MB전체를 복사하겠네
-> NRVO가 적용되면 mesh가 들어갈 최종위치에 data를 처음부터 만들 수 있다

즉, 값 반환 자체가 반드시 비싼 Copy라는 옛날식 인식은 틀릴 수 있다

## return std::move(result)는 왜 별로인가

NRVO는 지역 객체 이름 자체를 반환하는 패턴을 보고 적용할 수 있다.

```c++
return result;
```

그런데

```c++
return std::move(result);
```

라고 하면 반환 표현식이 더 이상 단순한 result가 아니라 std::move(result)가 된다.
그래서 NRVO조건을 깨뜨릴 수 있다.

## NRVO가 안되면 Copy가 되는가?

꼭 그렇지는 않다.
NRVO가 적용되지 않는 상황에서도 반환 과정에서 Move가 선택될 수 있도록 특별한 규칙이 있다.
그렇기 때문에

> 지역 객체를 값으로 반환할 땐 그냥 return하라

## 실전에서는 Rule of Zero가 중요함

```c++
class LevelData
{
    std::string Name;
    std::vector<EnemyData> Enemies;
    std::unique_ptr<NavigationData> Navigation;
};
```

자원관리를 STL/RAII 타입이 처리하도록 만듬
그러면 여러 실용적 장점이 생긴다.
