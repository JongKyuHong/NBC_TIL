# C++

## Class 개념

우리는 유지보수를 용이하게 하고, 재사용성을 높이는 방향으로 구현해야 합니다. C++에서는 이를 위해 객체지향 프로그래밍을 활용할 수 있습니다.

### class의 정의

- 자동차를 예로들면 우리는 자동차의 세부사항을 몰라도 기능을 이해하기 때문에 제어할 수 있습니다.
- 자동차에서 제공하는 기능 이외에 세부사항은 제어하지 못해야합니다. 왜냐면

  - 사용성이 떨어진다 : 사용자가 알아야할 정보가 많아지기 때문에
  - 구현 난이도가 증가한다 : 발생할 예외를 모두 고려해야 하므로

- 멤버 함수는 외부에서도 정의가능한듯
- class키워드를 사용할 때 기본값은 private

### 생성자

- 정의된 class를 변수로 선언하면 해당 객체가 메모리에 올라간다. 이를 인스턴스화 라고 함
- 생성자는 선언과 함께 정의도 해야한다.

### 헤더파일 정의

- #ifndef로 같은 헤더가 정의되어있지 않은 경우에만 코드를 실행하라
- #define으로 정의
- #endif로 ifndef가 끝났다는것을 알림

### 숙제 1.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q1.cpp)

### 숙제 2.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q2.cpp)
- multiply에서 접근 제어 지시자들은 class 단위로 적용되기때문에 other의 멤버변수에 .으로 바로 접근할 수 있다.

## 객제지향 프로그래밍

### 멤버 초기화 리스트

생성자 함수에서 사용할 수 있는 문법

Vehicle(string c, int s) : color(c), speed(s) {}
생성자 내부보다 먼저 실행되고 color를 선언함과 동시에 초기화함

-

* 자식 클래스의 생성자는 부모 클래스의 생성자를 호출 할 수 있다.

### 다형성

- 기본이 되는 클래스를 만들어 함수의 인터페이스를 정의하고 실제 구현은 파생 클래스에서 담당하는 기법
- 동적 바인딩을 통해 실제 호출된 객체의 타입에 따라 적절한 파생 클래스의 함수가 실행되도록 하려면 함수 앞에 virtual 키워드를 붙여야 한다.

![](https://velog.velcdn.com/images/kyu_/post/12675e9b-c33a-4684-bd7d-83ce4dd84358/image.png)

- virtual 키워드를 붙이면 가상 함수 테이블이 생성된다.

  - 가상 함수 테이블에는 그림처럼 기본 클래스에서 virtual로 선언된 함수가 파생 클래스에서 재정의된경우 그 위치가 기록되어 동적 디스패치가 가능하다.
  - 동적 디스패치 : 어떤 함수를 실행할지 런타임(프로그램이 돌아가는 도중)에 결정하는 기술
  - 만약에 virtual을 붙이지 않으면 부모의 메서드가 호출된다. 파생클래스의 정보를 알수 없으므로

- 개선 코드

```
#include <iostream>
#include <string>

using namespace std;

class Animal
{
public:
	Animal() {};
	virtual void bark() {};
};

class Lion : public Animal
{
public:
	Lion(string word) : m_word(word) {};
	void bark() {
		cout << "Lion : " << m_word << endl;
	};
private:
	string m_word;
};

class Wolf : public Animal
{
public:
	Wolf(string word) : m_word(word) {};
	void bark() {
		cout << "Wolf : " <<  m_word << endl;
	};
private:
	string m_word;
};

class Dog : public Animal
{
public:
	Dog(string word) : m_word(word) {};
	void bark() {
		cout << "Dog" << " " << m_word << endl;
	}
private:
	string m_word;
};

void print(Animal* animal)
{
	animal -> bark();
}

int main()
{
	Lion lion("ahaaaa!");
	Wolf wolf("ohhhhhh");
	Dog dog("ooooooooooooooops");

	print(&lion);
	print(&wolf);
	print(&dog);

	return 0;
}

```

- ->(화살표) 는 주소(포인터)를 따라가서 멤버를 찾으라는 뜻이고, .(점)은 실제 객체에서 바로 멤버를 찾으라는 차이가 있다.
- virtual을 안붙이면 정적 바인딩, 붙이면 동적 바인딩

### 가상함수

- 인터페이스는 필요하지만 기본 클래스에서 구현할 필요가 없는 경우 순수 가상함수로 선언한다.
- 예시) 사자, 개 모두 특정 울음소리를 가지지만 동물이라는 개념자체에는 특정 울음소리가 없다.
- 가상 함수를 정의하고 =0 을 붙이면 된다.
- 순수 가상 함수를 하나 이상 포함하는 클래스를 추상 클래스라고 한다.
- 추상 클래스는 직접 인스턴스를 생성할 수 없고, 포인터 혹은 참조를 통해서만 사용할 수 있다.

### 숙제 1.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q3.cpp)

### 1주차 숙제

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q4.cpp)

## 자원 관리

### 스택 메모리

- 일반 변수들은 대부분 스택 메모리 공간을 차지한다.
- 스택메모리의 가장 큰 특징은 변수의 생존 주기가 끝나면 선언 시 할당되었던 메모리가 자동으로 회수된다.
  - 즉, 사용자가 직접 메모리를 해제할 필요가 없다.

### 힙 메모리

#### 스택 메모리의 단점

- 스택메모리는 자동으로 회수해가는 대신 단점도 존재한다.
  - 일반적으로 할당 가능한 스택메모리의 크기가 제한적
  - 변수의 스코프를 벗어나면 자동으로 해제되므로, 메모리를 더 길거나 유연하게 관리하기 어렵다.
- 이 문제 해결을 위해 힙(동적) 메모리를 사용할 수 있다.

### 힙 메모리

- 동적 할당시 new, 해제 시 delete
- 스택과 달리 자동으로 해제되지 않으므로 메모리 누수등의 위험이 있을 수 있다.
- 동적 할당된 객체의 생존 주기는 사용자가 delete로 해제할때까지 유지

### Dangling Pointer

- 어떤 식당을 방문하려고 했는데 그 건물이 이미 철거된 상태라면 헛걸음을 하게된다.
- C++에서도 마찬가지 인데 포인터는 메모리가 해제되었는지 자동으로 알 수 없다.
- 이미 해제된 메모리를 포인터가 가리키고있는것은 매우 위험하다.
- 이런 포인터를 댕글링 포인터라고 한다.

### Memory Leak

- 동적으로 할당한 메모리를 사용후 제대로 해제하지 않으면 계속해서 사용하지 않는 메모리가 쌓이게된다.
- 이를 메모리 누수라고 한다.

### 스마트 포인터

- Heap은 여러가지 장점을 제공하지만 메모리를 직접 관리해야한다는 부담이 있다.
- C++에서는 이를 해결하기 위해 스마트 포인터를 제공한다.
- new/delete를 쓰지 않는 자동 메모리 관리

#### unique_ptr

- unique_ptr은 객체에 대한 단일 소유권을 관리합니다.
- 객체의 소유권을 명확히 하고 소유권 이전을 통해 효율적인 자원관리가 가능하다.
- move를 통해 소유권을 이동한다.

```c++
#include <memory> // unique_ptr사용
...
unique_ptr<int> ptr1 = make_unique<int>(10);
cout << "ptr1의 값: " << *ptr << endl;

unique_ptr<int> ptr2 = move(ptr1);
if (!ptr1){
    cout << "ptr1은 이제 비어 있습니다." << endl;
}
cout << "ptr2의 값 : " << *ptr2 << endl;
...
```

#### shared_ptr

- 레퍼런스 카운트를 관리
- 현재 객체를 참조하는 포인터의 개수를 카운팅하는 것
- 카운터가 0이되면 자동으로 메모리 해제
- 이를 통해 댕글링포인터와 메모리누수를 방지

```c++
...
#include<memory>
shared_ptr<int> ptr1 = make_shared<int>(10);
cout << "ptr1의 참조 카운트 : " << ptr1.use_count() << endl;

shared_ptr<int> ptr2 = ptr1;
cout << "ptr2 생성 후 참조 카운트 : " << ptr1.use_count() << endl;

ptr2.reset();
cout << "ptr2 해제 후 참조 카운트 : " << ptr1.use_count() << endl;
...
```

#### weak_ptr

- 객체의 소유권을 공유하지 않음
- shared_ptr은 유용하지만 순환참조가 발생할 수 있다.
- 서로 순환하고 있는 shared_ptr중 하나를 weak_ptr로 대체하면 순환고리를 끊음으로써 문제를 해결할 수 있다.
- 정리하면 shared_ptr은 관찰과 소유를 한다면, weak_ptr은 관찰만 한다고 표현한다.

```c++
#include <iostream>
#include <memory>

class B; // Forward declaration

class A {
public:
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

class B {
public:
    std::weak_ptr<A> a_ptr; // weak_ptr로 변경
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();

    a->b_ptr = b;
    b->a_ptr = a; // weak_ptr로 참조

    return 0;
}
```

- 이 코드에서는 forward declaration(전방선언)이 나오는데 class A안에 B가 들어있으므로 미리 B를 선언해주었다. (전방 선언 하지 않았다면 에러 발생 할수도)

### 얕은 복사와 깊은 복사

- 결론적으로 안전한 메모리 관리를 위해 깊은 복사를 사용하는 편이 좋다.

#### 얕은 복사

- 클래스 내의 포인터 멤버를 복사할 때 포인터가 가리키는 데이터가 아닌 포인터가 저장하고 있는 주소값만 복사하는 것을 의미한다.
- 얕은복사를 진행한 후 원본 객체가 메모리를 해제하면 복사된 객체의 포인터는 해제된 메모리를 가리키고 있고 이는 댕글링 포인터가 발생할 수 있다.

```
int* A = new int(30);

int* B = A;
```

#### 깊은 복사

- 클래스의 포인터 멤버가 가리키는 동적 데이털르 새로 할당된 독립적인 메모리 영역에 복제하는 것을 의미
- 원본 객체와 복사된 객체는 서로 독립적인 메모리 공간을 소유하므로 댕글링 포인터가 발생하지 않는다.

```
int* A = new int(30);

int* B = new int(*A);
```

### 언리얼엔진의 메모리 관리

- 언리얼 엔진은 메모리 관리를 자동화 하기 위해 가비지 컬렉션 사용
- 더이상 프로그램에서 사용하지 않는다고 판단되는 UObject들을 식별하여 메모리에서 제거
- `UObject`에는 가비지 컬렉션 동작 방식을 제어하는 다양한 플래그가 존재
  - RF_RootSet : 이 플래그가 설정된 객체는 루트셋의 일부로 관리됨, 설정된 시점부터 가비지 컬렉션 대상이 아니다.
  - RF_BeginDestroyed : 객체의 BeginDestroy()함수가 호출되었음을 나타냄, 객체가 실제로 메모리에서 해제되기 전에 필요한 정리 작업을 수행하는 함수
  - RF_FinishDestroyed : 객체의 FinishDestroy() 함수가 호출되었음을 나타냄 해당 함수는 객체 소멸의 마지막 단계, 이 함수 호출 후 객체의 메모리가 완전히 해제됨

#### 가비지 컬렉터 단계

기본적으로 마크앤 스윕 알고리즘 방식으로 동작

1. 루트셋에서 시작

- 루트셋에 포함된 객체 식별
- 이 객체들은 항상 살아있다고 간주되는 특별한 객체
- 게임 엔진 자체, 플레이어 컨트롤러 등이 루트셋에 포함될 수 있다.
- 이는 가비지 컬렉션 대상이 아니다.

2. 마크 단계 - 도달 가능성 분석

- 루트셋 객체에서 시작해서 직간접적으로 참조하는 UObejct를 마크, 이는 객체가 사용중임을 나타냄

3. 스윕 단계 - 메모리 회수

- 마크 단계가 완료되면 마크되지 않은 객체들이 차지하고 있던 메모리를 회수, 이 과정에서 해당 객체의 소멸자가 호출되고 메모리가 반환

#### 언리얼엔진의 리플렉션 시스템

리플렉션이란 프로그램이 실행 중에 자신의 구조와 상태를 검사하고 수정할 수 있는 능력을 말합니다.
C++의 경우 자체적인 리플렉션 기능이 없기 때문에 언리얼 엔진은 자체적인 리플렉션 시스템을 구축합니다.

리플렉션은 UObject를 위한 운영체제와 같습니다.
언리얼 내부에서 동작하는 여러 모듈(가비지 컬렉터, 스크립트 시스템)등은 모두 UObject 기반
사용자가 정의한 타입의 경우 언리얼엔진에서 알수 없으므로 이를 처리할 수 있도록 타입정보를 공유해야함 이를 위한 작업이 리플렉션이다.

![](https://velog.velcdn.com/images/kyu_/post/6f86f5c2-cac3-4917-b2ea-87df964094f9/image.png)

- 리플렉션의 핵심은 UHT 코드 생성기

UHT는 C++ 컴파일러가 수행되기 전에 동작
C++ 코드 내에서 메타 데이터를 얻고, 내부적으로 소스 코드를 생성합니다.
이 동작이 완료된 이후에 C++ 컴파일러가 수행

![](https://velog.velcdn.com/images/kyu_/post/d5163f4a-dcf8-46c1-8fc7-a90cbaccbfaf/image.png)

##### 핵심 리플렉션 매크로

- UCLASS() : C++클래스를 UObject 기반의 리플렉션 시스템에 등록, 클래스 정의 앞
- UPROPERTY() : 멤버 변수를 리플렉션 시스템에 노출 , 멤버 변수 선언 앞
- UFUNCTION() : 멤버 함수를 리플렉션 시스템에 노출, 멤버 함수 선언 앞
- USTRUCT() : C++구조체를 리플렉션 시스템에 등록, 구조체 정의 앞
- GENERATED_BODY() : UHT가 생성하는 리플렉션 및 엔진 지원 코드를 위한 삽입 지점, 클래스/구조체 본문 첫 줄

![](https://velog.velcdn.com/images/kyu_/post/afd885a2-0fbb-48e0-ab61-c786cfb3a970/image.png)

#### 숙제 1.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q5.cpp)

#### 숙제 2.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q6.cpp)

### 궁금한거 정리

#### 리터럴값과 상수값의 차이?

- 리터럴도 변경할 수 없는 값 그 자체, 상수도 변경할수없는 값인데 둘의 차이가 무엇인가?
- 리터럴 : 이름이 없는 값 그 자체, 코드에 10이라고 적으면 그게 리터럴
- 상수 : 값이 변하지않는 변수(공간)이다. const int maxScore = 100에서 maxScore가 상수, 100은 리터럴

#### 캡슐화와 추상화의 차이?

- 추상화 : "복잡해서" 감추는 것
  - 설계 단계의 개념이자 이 객체가 무엇을 하는지만 보여주고 어떻게 하는지는 몰라도 되게 만듬
- 캡슐화 : "위험해서" 감추는 것
  - 구현 단계의 개념 데이터와 동작을 하나로 묶고, 데이터를 외부로부터 보호

#### 멤버변수를 다룰때 왜 getter, setter를 사용할까?

- 안정성과, 유지보수성을 위해

1. 무결성 보장

- 만약에 멤버변수에 들어가면 안되는값 예시) HP=-500 이런식으로 넣게 되면 오류가 난다 하지만 setter를 통해서 값을 넣게 되면 그에대한 유효성 검사를 할 수 있다.

2. 내부 구현 은닉(유지보수성)
   만약 나중에 HP를 저장하는 방식을 int에서 float로 변경하거나 암호화해서 저장해야 한다면? public변수였다면 이 변수를 사용하는 모든 코드를 다 고쳐야 한다. 하지만 getter/setter를 쓴다면 하뭇 내부만 바꾸면 된다.

#### 가상함수에서 내부를 그냥 구현하지 않은것과 =0붙이는것의 차이?

virtual void func() {} : 일반 가상함수인 경우, 강제성이 없다, 이 클래스로 객체를 만들 수 있다.

virtual void func() = 0 : 순수 가상 함수인경우 강제성이 매우 강하다. 자식 클래스에서 이 함수를 구현하지 않으면 컴파일이 안된다, 이 클래스로 객체를 만들 수 없다.

#### 상속받을때 왜 public으로 받는가

```
class Dog : public Animal {};
```

왜 부모의 클래스를 상속받을때 public을 쓸까? 다른것을 쓰면 안될까?
-> public을 쓰는 이유는 부모에서 공개한 정보(public)은 나도 공개하고 부모에서 protected한것은 나도 하겠다고 선언하는 것이다. 거의 대부분의 경우 public을 사용한다.

#### size_t 자료형

- 가장 큰 양수 정수, 부호가 없는 정수를 나타날때 사용
- 배열의 크기나 메모리 주소계산처럼 절대 음수가 될수없고 아주 클수도 있는 숫자에 사용

#### 소멸자

- 객체(인스턴스)가 죽을 때(메모리에서 사라질때)마지막으로 한 번 호출되는 함수
- 클래스 이름앞에 ~붙임
- 동적할당받았던(new) 메모리 해제(delete)하거나 열어둔 파일 닫는등 뒷정리 청소라고 보면됨
- 객체가 사라지는 시점에 알아서 자동으로 호출

#### virtual ~Adventure() {}

- 가상 소멸자
- 뒷정리청소
- 부모 소멸자에 virtual이 없으면 어떤 파생클래스에서 소멸하는지를 몰라서 부모만 청소한다.
- 부모 소멸자에 virtual이있으면 자식부터 소멸시키고 부모를 소멸시킨다.
