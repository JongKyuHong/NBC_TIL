# C++

## 온라인 강의 복습하기

### 스택 vs 힙 메모리

> 스택

- 스택 메모리는 함수 호출시 자동 할당 및 자동 해제
- 크기가 제한적 1~8MB이며 LIFO (스택이므로)
- 지역 변수, 함수 매개변수, 반환 주소등이 저장

> 힙

- 프로그래머가 명시적으로 할당(new, malloc) 하고, 해제(delete, free)해야 합니다.
- 크기가 스택보다 훨씬크고 동적으로 크기를 결정할 수 있습니다.

> 둘의 차이

- 스택은 빠르지만 크기 제한이 있고, 수명이 짧다.
- 힙은 느리지만 큰 데이터나 긴 수명이 필요한 데이터에 적합하다.

### 댕글링 포인터

- 이미 해제된 메모리를 가리키는 포인터

> 발생 원인

- 메모리 해제 후 포인터를 nullptr로 설정하지 않음
- 여러 포인터가 같은 메모리를 가리킬 때 하나만 해제
- 스택 변수의 주소를 반환

> 해결법

- 해제 후 즉시 ptr = nullptr설정
- 스마트 포인터 사용
- 원시 포인터 사용 최소화

### 스마트 포인터

- 스마트 원리의 핵심은 new / delete를 사용하지 않는 자동 메모리 관리

#### unique_ptr

- 객체에 대한 단일 소유권을 관리
- move를 통해 소유권을 이동하는 식으로 관리

```c++
#include <memory>

unique_ptr<int> ptr1 = make_unique<int>(10);
unique_ptr<int> ptr2 = move(ptr1);
```

#### shared_ptr

- 하나의 객체를 여러 개의 포인터가 함께 참조할 수 있는 스마트 포인터
- 내부적으로 레퍼런스 카운터를 관리
- use_count()메서드로 현재 객체를 참조하는 포인터 수를 확인가능
- reset() 메서드로 소유중인 객체를 해제하거나 다른 객체로 변경 가능

```c++
#include <memory>

shared_ptr<MyClass> obj1 = make_shared<MyClass>(42);
shared_ptr<MyClass> obj2 = obj1;

cout << "obj1과 obj2의 참조 카운트: " << obj1.use_count() << endl; // 출력: 2

obj2->display(); // 출력: 값: 42

// obj2를 해제해도 obj1이 객체를 유지
obj2.reset();
cout << "obj2 해제 후 obj1의 참조 카운트: " << obj1.use_count() << endl; // 출력: 1
```

### weak_ptr

- 레퍼런스 카운터를 증가시키지 않는 약한 참조방식으로 동작
- lock() 호출 후 반환된 shared_ptr이 유효한지 확인 후에 사용해야 한다.

```c++
#include <iostream>
#include <memory>

class A{
public:
    void say_hello(){
        std::cout << "Hello from A\n";
    }
};

class B{
public:
    std::weak_ptr<A> a_ptr;
    void useA(){
        if (auto a_shared = a_ptr.lock()){ // a_ptr이 유효한지 확인한다.
            a_shared->say_hello();
        } else {
            std::cout << "A is no longer available.\n";
        }
    }
};

int main(){
    std::shared_ptr<B> b = std::make_shared<B>();

    {
        std::shared_ptr<A> a = std::make_shared<A>();
        b->a_ptr = a;
        b->useA();
    }
    b->useA();
}

```

- unique_ptr은 독점 소유권 모델, 복사 불가, 이동 가능
- shared_ptr은 공유 소유권 모델, 마지막 shared_ptr이 소멸될때 메모리가 해제된다.
- weak_ptr은 shared_ptr의 순환 참조 문제 해결, 참조 카운트를 증가시키지 않는 약한 참조

### 메모리 누수

> 원인

- new와 delete짝 안맞음
- 예외 발생시 delete실행 안됨
- 복사 생성자/대입 연산자에서 깊은 복사 구현 안함
- 순환 참조(shared_ptr)

> 해결법

- 스마트 포인터 사용
- 메모리 프로파일러 도구 사용

### 얕은 복사 vs 깊은 복사

> 얕은 복사

- 포인터의 주소만 복사, 두 객체가 같은 메모리를 가리킴

```c++
class Shallow{
public:
    int* data;
    Shallow(int value){
        data = new int(value);
    }

    ~Shallow(){
        delete data;
    }
}

Shallow obj1(10);
Shallow obj2 = obj1; // data 포인터만 복사
// obj1.data와 obj2.data가 같은 주소!
// obj2 소멸 시 delete 실행
// obj1 소멸 시 이미 해제된 메모리 delete -> 크래시!
```

> 깊은 복사

- 메모리 내용 자체를 복사한다.
- 각 객체가 독립적인 메모리를 가짐

```c++
class Deep{
public:
    int* data;
    Deep(int value){
        data = new int(value);
    }

    // 복사 생성자: 깊은 복사
    Deep(const Deep& other){
        data = new int(*other.data); // 새 메모리 할당 후 값 복사
    }

    // 복사 대입 연산자 : 깊은 복사
    Deep& operator=(const Deep& other){
        if(this!=&other){
            delete data; // 기존 메모리 해제
            data = new int(*other.data); // 새 메모리 할당 후 값 복사
        }
        return *this;
    }

    ~Deep(){
        delete data;
    }
}

Deep obj1(10);
Deep obj2 = obj1; // 독립적인 메모리 할당
// obj1.data와 obj2.data는 다른 주소
// 각각 안전하게 소멸 가능
```

### 언리얼 엔진의 메모리 관리

- 자체 가비지 컬렉션 시스템 사용, UObject를 상속받는 모든 클래스는 자동으로 관리

> UObject와 가비지 컬렉션

```c++
UCLASS()
class AMyActor:public AActor{ // AActor는 UObject상속
    GENERATED_BODY()

public:
    UPROPERTY()
    UMyComponent* MyComponent; // UPROPERTY로 마킹
}
```

> 핵심 원칙

- UPROPERTY() 매크로 : 언리얼에게 `이 포인터를 추적해줘`라고 알림
- 참조 추적 : UPROPERTY로 마킹된 모든 참조를 추적
- 도달 불가능 객체 삭제 : 어떤 UPROPERTY에서도 참조되지 않으면 삭제

> 중요 규칙

- `UObject*` 포인터는 반드시 `UPROPERTY()` 매크로 사용
- UPROPERTY 없으면 가비지 컬렉터가 추적 안하고 삭제 가능
- 일반 C++ 포인터(int\* 등)는 수동 관리 필요

#### 나중에 공부

- NewObject vs CreateDefaultSubobject
- 스마트 포인터 (TSharedPtr, TWeakPtr, TUniquePtr)
  - UObject가 아닌 일반 C++ 객체는 언리얼의 스마트 포인터를 사용

### 언리얼 엔진의 리플렉션 시스템

- 런타임에 클래스, 함수, 프로퍼티 정보를 동적으로 조회하고 조작할 수 있는 시스템
- UCLASS()
  - 클래스를 언리얼 시스템에 등록
- 주요 지정자

  - Blueprintable : 블루프린트에서 이 클래스 상속 가능
  - BlueprintType : 블루프린트 변수로 사용 가능
  - Abstract : 추상 클래스 (직접 인스턴스화 불가)

- UPROPERTY()

  - 프로퍼티를 리플렉션 시스템에 노출

- UFUNCTION()

  - 함수를 블루프린트에 노출하거나 네트워크 복제를 설정합니다.

- USTRUCT()
  - 구조체를 리플렉션 시스템에 등록

> 리플렉션의 장점

- 에디터 통합 : 프로퍼티를 에디터에서 직접 편집
- 블루프린트 연동 : C++코드를 블루프린트에서 사용
- 직렬화 : 자동 저장/로드
- 네트워크 복제 : 자동 네트워크 동기화
- 가비지 컬렉션 : 자동 메모리 관리

> 주의 사항

- 클래스는 UCLASS()와 GENERATED_BODY() 필요
- 헤더에는 #include "MyClass.generated.h"가 맨 마지막에 와야함
- Unreal Build Tool이 자동으로 생성코드 작성

## 궁금했던것

### 스택 메모리 크기

> 내 질문

램 크기가 현재 평균적으로 32gb도 많이 보급하고 있고 64에서 많게는 100기가가 넘게다는 경우도있는데 이럴때도 스택메모리는 1~8MB정도 밖에안하는건지

> 답변

- 스택 메모리의 크기는 RAM과는 무관하다
- 스택 크기는 운영체제와 스레드 설정에 의해 고정
- windows 64비트 기준으로 1MB가 기본값이다. 스레드당 1MB

---

- 힙은 프로세스의 가상 주소 공간에서 사용 가능한 모든 메모리를 사용할 수 있다.
- 32비트에서는 최대 2~3GB이고, 64비트는 이론상으로는 수십TB(실제로는 물리RAM + 스왑 공간)
- 예를들어 RAM이 16GB이면 힙은 거의 전체 16GB를 사용할 수 있습니다. (OS와 다른 프로세스가 사용하는 부분 제외)

### 스택과 힙의 수명

> 내 질문

스택 메모리는 힙보다 빠르고 크기가 작고 수명이 짧다는데, 1. 왜 빠른거고 얼마나 빠른건지 2. 수명은 차이가 왜 나는지? (변수가 블록에서 나가면 수명이 다해서 그런건지)

> 답변

1. 메모리 할당 방식

- 스택은 단순히 포인터를 증가/감소시키기만 하면 되서 속도가 빠르다.
- 힙은 복잡한 메모리 관리가 필요합니다.
  - 사용가능한 메모리 블록 검색
  - 적절한 크기의 블록 찾기
  - 메모리 단편화 처리
  - 할당 정보를 별도로 기록
  - 해제 시 인접 블록 병합 등

2. 캐시 지역성

- 스택은 메모리가 연속적으로 배치되고 접근 패턴이 예측 가능함, CPU는 스택의 다음 데이터를 미리 캐시에 로드합니다.
- 힙은 메모리가 흩어져있어서 예측하기가 힘듬 그래서 캐시 미스가 자주 발생한다.

3. 스레드 지역성

- 스택은 각 스레드마다 독립적이라 동기화가 필요없다. 힙은 모든 스레드가 공유하므로 멀티 스레드 환경에서 락이 필요하여 느립니다.
- 일반적으로 힙이 스택보다 10~100배 정도 느림, 특히 작은 객체를 반복해서 할당/해제하는경우 차이가 극명하게 난다.

4. 수명

스택 :

- 스택안에 들어가는 변수들은 컴파일타임에 수명을 정확하게 알 수 있음, 스코프에 의해 결정
- 블록/함수가 끝나면 스택 포인터를 원래 위치로 되돌리기만 하면 됨
  힙 :
- 수명이 프로그래머의 명시적 delete에 의해 결정
- 함수가 끝나도 유지가능
- 다른 함수에서도 접근 가능

### 댕글링 포인터

> 내 질문

1. 댕글링포인터 발생원인중에 스택 변수의 주소를 반환은 어떤이유로 댕글링 포인터가 발생하는가??

- 스택 변수 주소 반환문제

```c++
int* getDanglingPointer(){
    int localVal = 42;
    return &localVal;
}

int main(){
    int* ptr = getDanglingPointer();
    cout << *ptr << endl; // 정의되지 않은 동작

    // 다른 함수 호출
    someOtherFunction();

    cout << *ptr << endl; // 이제 완전히 다른 값 나옴
}
```

- 왜 문제인가?
  - getDanglingPointer()함수가 끝나면 localVar가 있던 스택 메모리는 "사용 가능"상태가 된다.
  - 포인터 ptr은 여전히 그 주소를 가리키고 있다.
  - 다른함수가 호출되면 같은 스택위치를 재사용한다.
  - ptr이 가리키는 메모리의 값이 변경된다.

2. 여러 포인터가 같은 메모리를 가리키고 하나만 해제하면 댕글링 포인터가 발생하는데 이렇게 여러 포인터가 같은 메모리를 가리킬일이 게임 개발할때 종종 있는가??

자주있다.

- 부모-자식 관계
- 캐싱된 참조
- 리소스 공유 등
  언리얼에서는 이를 UPROPERTY(), TWeakObjectPtr, 스마트 포인터 사용으로 해결한다.

### 언리얼 프로젝트

> 질문

언리얼엔진 5공식 유튜브에 33원정대 개발자가 인터뷰한것을 봤는데 비개발자인 사람과 협업하기 위해서 모든 로직을 블루프린트로 짰다고 인터뷰했는데 댓글에서 보니까 오해하기 쉬운 인터뷰라고 하는 사람이 있었다. 마치 모든 로직을 블루프린트로 만들수는 없다는것처럼 이런 것들을 보아하니 C++로 만드는 부분과 블루프린트로 만드는 부분이 나눠져있는거같은데 C++, 블루프린트를 어느 경우에 사용하는지??

> 답변

모든 로직을 블루프린트로 작성했다는것은 게임 로직(게임 플레이코드)를 의미할 가능성이 크다.

#### C++을 사용해야 하는 경우

1. 성능이 중요한 로직

블루프린트는 인터프리터 방식이라 C++보다 느리다.

```c++
// 매 프레임 수백~수천 번 실행되는 로직은 C++로
void Tick(float DeltaTime){
    // 1000개의 적 AI 업데이트
    for(AEnemy* Enemy : AllEnemies){
        Enemy->UpdatePathfinding();
    }
}
```

성능차이가 약 5배~10배 날 수있다.

2. 복잡한 알고리즘/수학계산

```c++
// 복잡한 물리계산, AI경로 탐색 등
TArray<FVector> CalculateOptimalPath(FVector Start, FVector End){
    // A* 알고리즘, 수백 개의 노드 검사
    // 블루프린트로 하면 노드만 수백개
}
```

3. 저 수준 메모리/네트워크 제어

```c++
UFUNCTION(Server, Reliable)
void ServerRPC_UpdatePosition(FVector NewPos){
    // 대역폭 최적화, 압축 등
}
```

4. 재사용 가능한 시스템/프레임워크

```c++
// 인벤토리 시스템, 스탯 시스템 등 기반 클래스
UCLASS(Blueprintable) // 블루프린트에서 상속 가능하게
class UInventoryComponent: public UActorComponent{
    GENERATED_BODY()

    UFUNCION(BlueprintCallable)
    void AddItem(UItem* Item); // C++로 기능 제공
}
```

#### 블루프린트를 사용해야 하는 경우

1. 빠른 프로토타이핑/반복 작업

```
레벨 디자이너가 문 여는 로직 테스트:
- 블루프린트: 5분 만에 만들고 바로 테스트
- C++: 코드 작성 → 컴파일 (5~10분) → 테스트
```

2. 비주얼한 로직 (이벤트 기반)

```
플레이어가 트리거 진입 → 문 열림 → 사운드 재생 → 이펙트 생성
```

이런 순차적/이벤트 기반 로직은 블루프린트가 훨씬 직관적입니다.

3. 디자이너/아티스트 협업

```c++
// C++로 기능 제공
UFUNCTION(BlueprintCallable)
void PlaySkillEffect(int SkillID);

// 디자이너가 블루프린트에서
// 어떤 스킬에 어떤 이펙트?
// 타이밍은?
// 사운드는?
// 모두 블루프린트로 조합
```

4. 조정 가능한 값들

```
HP : 100 -> 실시간으로 150으로 변경 -> 바로 테스트
C++로 하면 컴파일이 필요
```

| 기준                     | C++ | Blueprint |
| ------------------------ | --- | --------- |
| 성능 중요 (Tick, 반복문) | ✅  | ❌        |
| 복잡한 알고리즘          | ✅  | ❌        |
| 시스템/프레임워크        | ✅  | ❌        |
| 네트워크 세밀한 제어     | ✅  | ❌        |
| 빠른 프로토타이핑        | ❌  | ✅        |
| 이벤트 기반 로직         | ❌  | ✅        |
| 비개발자 협업            | ❌  | ✅        |
| 자주 조정되는 값         | ❌  | ✅        |

> 결론

- C++로 견고한 시스템 만들기
- `BlueprintCallable`, `Blueprintable`등으로 블루프린트에 노출
- 블루프린트에서 C++기능을 조합하여 게임플레이 구현
- 성능 문제 발생 시 해당 부분만 C++로 이동

이런느낌??~~

### 얕은 복사 깊은 복사

1. 깊은 복사는 복사 생성자, 복사 대입 연산자 저렇게 두개의 경우로 가능한건지? 둘의 문법에 대해서 자세히 알려줘

> 답변

#### 복사 생성자

새로운 객체를 생성하면서 초기화할 때 호출

```c++
class MyClass{
private:
    int* data;
    int size;

public:
    // 일반 생성자
    MyClass(int s): size(s){
        data = new int[size];
        cout << "생성자 호출\n";
    }

    // 복사 생성자(Copy Constructor)
    // const 참조로 받고, 같은 클래스 타입
    MyClass(const MyClass& other){
        size = other.size;
        data = new int[size]; // 새 메모리 할당

        // 값 복사
        for (int i = 0; i < size; i++){
            data[i] = other.data[i];
        }
    }

    ~MyClass(){
        delete[] data;
        cout << "소멸자 호출\n";
    }
};


MyClass obj1(5); // 일반 생성자
MyClass obj2 = obj1; // 복사 생성자 호출(새 객체 생성)
MyClass obj3(obj1) // 복사 생성자 호출(새객체 생성)

void function(MyClass obj){ // 복사 생성자 호출(값으로 전달)
    //
}
function(obj1);

MyClass createObject(){
    MyClass temp(10);
    return temp; // 복사 생성자 호출
}
```

- const 로 원본 객체를 변경하지 않겠다고 선언하고
- & : 참조로 받음( 복사 생성자를 또 호출하는 무한루프 방지)
- 반환 타입 없음(생성자)

- MyClass obj2 = obj1은 왜 복사 생성자인가?
  - `선언과 동시에 초기화`이기 때문
  - 사실상 MyClass obj2(obj1)과 같다. obj2는 아직 존재하지 않는 새로운 객체라서
  - 이미 존재하면 복사 대입 연산자

#### 복사 대입 연산자

이미 존재하는 객체에 다른객체의 값을 할당할 때 호출

```c++
class MyClass{
private:
    int* data;
    int size;

public:
    MyClass(int s) : size(s){
        data = new int[size];
    }

    // 복사 대입 연산자
    // 참조를 반환하고, const참조를 받음
    MyClass& operator = (const MyClass& other){
        cout << "복사 대입 연산자 호출\n";

        // 자기 자신인지 확인
        if (this == &other){
            return *this; // 자기 자신이면 본인 반환(아무것도 안함)
        }

        // 기존 메모리 해제
        delete[] data;

        // 새 메모리 할당(깊은 복사)
        size = other.size;
        data = new int[size];

        // 값 복사
        for (int i = 0; i <size; i++){
            data[i] = other.data[i];
        }

        // *this반환 (연쇄 대입을 위해)
        return *this
    }

    ~MyClass(){
        delete[] data;
    }
}

// 복사 대입 연산자가 호출되는 경우
MyClass obj1(5); // 일반 생성자
MyClass obj2(10); // 일반 생성자
obj2 = obj1 // 복사 대입 연산자 호출(이미 존재하는 obj2에 할당)

// 연쇄 대입 가능
MyClass obj3(15);
obj3 =obj2 = obj1; // operator= 반환값이 참조라서 가능
```

2. 소멸자는 클래스안에서 포인터를 사용할때만 명시적으로 사용하는게 나은지? 아니면 클래스를 사용했다면 무조건 소멸자도 같이 표시하는게 나은지?

> 답변

#### 소멸자가 필요할 때

1. 동적 메모리 할당이 있을때
2. 파일/네트워크 등 리소스를 관리할 때
3. 상속할 기반 클래스일 때 (virtual 소멸자)

```c++
class Base{
public:
    virtual ~Base() {} // 필수(virtual로 선언)
};

class Derived: public Base{
private:
    int* data;
public:
    Derived(){
        data = new int[100];
    }

    ~Derived(){
        delete[] data;
    }
};

// 이렇게 사용할 가능성이 있으므로
Base* ptr = new Derived();
delete ptr; // virtual소멸자가 없으면 Derived 소멸자 호출 안 됨!
```

#### 소멸자가 불필요할때

1. 동적 메모리를 사용하지 않을때
2. 스마트 포인터를 사용할 때
3. STL 컨테이너만 사용할 때

> 추천 접근법

1. 원시 포인터를 가능하면 덜쓰기
2. 상속할 클래스는 항상 virtual 소멸자
3. 리소스 관리가 없으면 명시하지 않음
4. 리소스 관리가 있으면 Rule of five따르기
   - 복사 생성자
   - 복사 대입
   - 이동 생성자
   - 이동 대입

### 복사 대입 연산자를 사용하는 이유

- 복사 대입 연산자는 operator로 연산자를 재정의한다.
  - operator는 C++의 예약 키워드로 연산자에 특별한 의미를 부여하는 함수를 선언할때 사용
- C++에서는 사용자 정의 타입(클래스)도 기본 타입처럼 연산자를 사용할 수 있기 위해 사용

```c++
int a = 5;
int b = 10;
int c = a+b; // 연산자 사용 가능

MyClass obj1;
MyClass obj2;
MyClass obj3 = obj2 + obj1; // 연산자 사용 가능
```

**`MyClass안에서 MyClass& operator = (const MyClass& other){ ... } 같은 복사 대입 연산자를 사용했기 때문에 MyClass에 한해서는 = 대입연산자를 오버로드해서 obj2 = obj1같은 식이 가능한것이다`**
